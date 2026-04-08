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

#include <pthread.h>
#include <sched.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>

namespace {

/// Maximal size of the thread name.
constexpr std::size_t cMaxThreadName = 15;

/// Represents helper wrapper around OSAL thread function and its arguments.
/// @note This type is necessary, because OsalThreadFunction has different signature than pthread.
///       Thus special threadWrapper() function (with pthread compliant signature) is used directly in
///       call to pthread_create() and OSAL thread function is passed along with its arguments as the
///       argument.
struct ThreadWrapperData {
    OsalThreadFunction func{};
    void* param{};
};

/// Helper thread function that has signature required by pthread. It is used as a wrapper for
/// OSAL thread function.
/// @param arg          Helper thread arguments.
/// @return Result of this function is never used so it always returns nullptr.
void* threadWrapper(void* arg)
{
    auto wrapperData = std::unique_ptr<ThreadWrapperData>(static_cast<ThreadWrapperData*>(arg));
    wrapperData->func(wrapperData->param);
    return nullptr;
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

    if (name != nullptr && std::strlen(name) > cMaxThreadName)
        return OsalError::InvalidArgument;

    thread->initialized = false;

    const auto cPriorityMin = sched_get_priority_min(SCHED_RR);
    const auto cPriorityMax = sched_get_priority_max(SCHED_RR);
    const auto cPriorityStep = (cPriorityMax - cPriorityMin) / 4;

    int priority{};
    switch (config.priority) {
        case OsalThreadPriority::Lowest:  priority = cPriorityMin; break;
        case OsalThreadPriority::Low:     priority = cPriorityMin + (cPriorityStep * 1); break;
        case OsalThreadPriority::Normal:  priority = cPriorityMin + (cPriorityStep * 2); break;
        case OsalThreadPriority::High:    priority = cPriorityMin + (cPriorityStep * 3); break;
        case OsalThreadPriority::Highest: priority = cPriorityMax; break;
        default:                          return OsalError::InvalidArgument;
    }

    pthread_attr_t attr{};
    [[maybe_unused]] auto result = pthread_attr_init(&attr);
    assert(result == 0);

    result = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    assert(result == 0);

    sched_param schedParam{};
    schedParam.sched_priority = priority;
    result = pthread_attr_setschedparam(&attr, &schedParam);
    assert(result == 0);

    auto stackSize = std::max<std::size_t>(config.stackSize, PTHREAD_STACK_MIN);
    result = pthread_attr_setstacksize(&attr, stackSize);
    assert(result == 0);

    pthread_t handle{};
    auto wrapper = std::make_unique<ThreadWrapperData>();
    wrapper->func = func;
    wrapper->param = arg;
    result = pthread_create(&handle, &attr, threadWrapper, wrapper.release());
    assert(result == 0);

    if (name != nullptr && std::strcmp(name, "") != 0) {
        result = pthread_setname_np(handle, name);
        assert(result == 0);
    }

    result = pthread_attr_destroy(&attr);
    assert(result == 0);

    thread->impl.handle = handle;
    thread->initialized = true;
    return {};
}

OsalError osalThreadDestroy(OsalThread* thread)
{
    if (thread == nullptr || !thread->initialized)
        return OsalError::InvalidArgument;

    std::memset(thread, 0, sizeof(OsalThread));
    return {};
}

OsalError osalThreadJoin(OsalThread* thread)
{
    if (thread == nullptr || !thread->initialized)
        return OsalError::InvalidArgument;

    if (pthread_join(thread->impl.handle, nullptr) != 0)
        return OsalError::OsError;

    thread->initialized = false;
    return {};
}

void osalThreadYield()
{
    sched_yield();
}

uint32_t osalThreadId()
{
    return std::hash<pthread_t>{}(pthread_self());
}

OsalError osalThreadName(char* name, size_t size)
{
    if (name == nullptr || size == 0)
        return OsalError::InvalidArgument;

    std::array<char, cMaxThreadName + 1> buffer{};
    [[maybe_unused]] auto result = pthread_getname_np(pthread_self(), buffer.data(), buffer.size());
    assert(result == 0);

    std::strncpy(name, buffer.data(), size);
    name[size - 1] = '\0';
    return {};
}
