/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright MIT License
///
/// Copyright (c) 2020 Kuba Sejdak (kuba.sejdak@gmail.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/////////////////////////////////////////////////////////////////////////////////////

#include "osal/Thread.h"

#include "osal/Error.h"
#include "osal/Semaphore.h"

#include <FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <bit>
#include <cstdint>
#include <cstring>

namespace {

/// Helper thread function which is used as a wrapper for OSAL thread function.
/// @param arg          Helper thread arguments.
/// @note This function is used to implement thread joining.
void threadWrapper(void* arg)
{
    auto* params = static_cast<ThreadWrapperData*>(arg);
    params->func(params->arg);
    osalSemaphoreSignal(&params->semaphore);
}

} // namespace

OsalError osalThreadCreate(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg)
{
    return osalThreadCreateEx(thread, config, func, arg, nullptr);
}

OsalError
osalThreadCreateEx(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg, const char* name)
{
    if (thread == nullptr || func == nullptr)
        return OsalError::InvalidArgument;

    if (name != nullptr && std::strlen(name) > (configMAX_TASK_NAME_LEN - 1))
        return OsalError::InvalidArgument;

    thread->initialized = false;
    thread->joined = false;

    constexpr auto cPriorityMin = 0;
    constexpr auto cPriorityMax = configMAX_PRIORITIES - 1;
    constexpr auto cPriorityStep = (cPriorityMax - cPriorityMin) / 4;

    int priority{};
    switch (config.priority) {
        case OsalThreadPriority::Lowest:  priority = cPriorityMin; break;
        case OsalThreadPriority::Low:     priority = cPriorityMin + (cPriorityStep * 1); break;
        case OsalThreadPriority::Normal:  priority = cPriorityMin + (cPriorityStep * 2); break;
        case OsalThreadPriority::High:    priority = cPriorityMin + (cPriorityStep * 3); break;
        case OsalThreadPriority::Highest: priority = cPriorityMax; break;
        default:                          return OsalError::InvalidArgument;
    }

    thread->impl.params.func = func;
    thread->impl.params.arg = arg;
    osalSemaphoreCreate(&thread->impl.params.semaphore, 0);

#if configSUPPORT_STATIC_ALLOCATION
    thread->impl.stack = static_cast<StackType_t*>(config.stack);
    thread->impl.handle = xTaskCreateStatic(threadWrapper,
                                            name,
                                            config.stackSize,
                                            &thread->impl.params,
                                            priority,
                                            thread->impl.stack,
                                            &thread->impl.tcb);

    if (thread->impl.handle == nullptr)
        return OsalError::OsError;
#elif configSUPPORT_DYNAMIC_ALLOCATION
    auto result
        = xTaskCreate(threadWrapper, name, config.stackSize, &thread->impl.params, priority, &thread->impl.handle);
    if (result != pdPASS)
        return OsalError::OsError;
#endif

    thread->initialized = true;
    return {};
}

OsalError osalThreadDestroy(OsalThread* thread)
{
    if (thread == nullptr || !thread->initialized)
        return OsalError::InvalidArgument;

    if (!thread->joined)
        return OsalError::ThreadNotJoined;

    vTaskDelete(thread->impl.handle);

    osalSemaphoreDestroy(&thread->impl.params.semaphore);

    std::memset(thread, 0, sizeof(OsalThread));
    return {};
}

OsalError osalThreadJoin(OsalThread* thread)
{
    if (thread == nullptr || !thread->initialized || thread->joined)
        return OsalError::InvalidArgument;

    auto error = osalSemaphoreWait(&thread->impl.params.semaphore);
    if (error != OsalError{})
        return error;

    thread->joined = true;
    return {};
}

void osalThreadYield()
{
    taskYIELD();
}

uint32_t osalThreadId()
{
    return static_cast<std::uint32_t>(std::bit_cast<std::uintptr_t>(xTaskGetCurrentTaskHandle()));
}

OsalError osalThreadName(char* name, size_t size)
{
    auto* namePtr = pcTaskGetName(nullptr);
    std::strncpy(name, namePtr, size);
    name[size - 1] = '\0';
    return {};
}
