/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2020-2023, Kuba Sejdak <kuba.sejdak@gmail.com>
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this
///    list of conditions and the following disclaimer.
///
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////////////

#include "osal/Thread.h"

#include "osal/Semaphore.h"

#include <FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <algorithm>
#include <cstring>

/// Helper thread function which is used as a wrapper for OSAL thread function.
/// @param arg          Helper thread arguments.
/// @note This function is used to implement thread joining.
static void threadWrapper(void* arg)
{
    auto* params = static_cast<ThreadWrapperData*>(arg);
    params->func(params->arg);
    osalSemaphoreSignal(&params->semaphore);
}

OsalError osalThreadCreate(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg)
{
    return osalThreadCreateEx(thread, config, func, arg, nullptr);
}

OsalError
osalThreadCreateEx(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg, const char* name)
{
    if (thread == nullptr || func == nullptr)
        return OsalError::eInvalidArgument;

    if (name != nullptr && std::strlen(name) > (configMAX_TASK_NAME_LEN - 1))
        return OsalError::eInvalidArgument;

    thread->initialized = false;

    const auto cPriorityMin = 0;
    const auto cPriorityMax = configMAX_PRIORITIES - 1;
    const auto cPriorityStep = (cPriorityMax - cPriorityMin) / 4;

    int priority{};
    switch (config.priority) {
        case OsalThreadPriority::eLowest: priority = cPriorityMin; break;
        case OsalThreadPriority::eLow: priority = cPriorityMin + (cPriorityStep * 1); break;
        case OsalThreadPriority::eNormal: priority = cPriorityMin + (cPriorityStep * 2); break;
        case OsalThreadPriority::eHigh: priority = cPriorityMin + (cPriorityStep * 3); break;
        case OsalThreadPriority::eHighest: priority = cPriorityMax; break;
        default: return OsalError::eInvalidArgument;
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
        return OsalError::eOsError;
#elif configSUPPORT_DYNAMIC_ALLOCATION
    auto result
        = xTaskCreate(threadWrapper, name, config.stackSize, &thread->impl.params, priority, &thread->impl.handle);
    if (result != pdPASS)
        return OsalError::eOsError;
#endif

    thread->initialized = true;
    return OsalError::eOk;
}

OsalError osalThreadDestroy(OsalThread* thread)
{
    if (thread == nullptr || !thread->initialized)
        return OsalError::eInvalidArgument;

    vTaskDelete(thread->impl.handle);

    osalSemaphoreDestroy(&thread->impl.params.semaphore);

    std::memset(thread, 0, sizeof(OsalThread));
    return OsalError::eOk;
}

OsalError osalThreadJoin(OsalThread* thread)
{
    if (thread == nullptr || !thread->initialized)
        return OsalError::eInvalidArgument;

    return osalSemaphoreWait(&thread->impl.params.semaphore);
}

void osalThreadYield()
{
    taskYIELD(); // NOLINT
}

uint32_t osalThreadId()
{
    return uint32_t(xTaskGetCurrentTaskHandle());
}

OsalError osalThreadName(char* name, size_t size)
{
    auto* namePtr = pcTaskGetName(nullptr);
    std::strncpy(name, namePtr, size);
    name[size - 1] = '\0';
    return OsalError::eOk;
}
