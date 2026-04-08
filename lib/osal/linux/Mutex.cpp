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

#include "osal/Mutex.h"

#include "osal/Error.h"

#include <pthread.h>

#include <cassert>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>

OsalError osalMutexCreate(OsalMutex* mutex, OsalMutexType type)
{
    if (mutex == nullptr)
        return OsalError::InvalidArgument;

    mutex->initialized = false;

    auto mutexType = (type == OsalMutexType::Recursive) ? PTHREAD_MUTEX_RECURSIVE : PTHREAD_MUTEX_NORMAL;

    pthread_mutexattr_t attr{};
    pthread_mutexattr_init(&attr);
    [[maybe_unused]] auto result = pthread_mutexattr_settype(&attr, mutexType);
    assert(result == 0);

    pthread_mutex_t handle{};
    result = pthread_mutex_init(&handle, &attr);
    assert(result == 0);

    result = pthread_mutexattr_destroy(&attr);
    assert(result == 0);

    mutex->impl.handle = handle;
    mutex->type = type;
    mutex->initialized = true;
    return {};
}

OsalError osalMutexDestroy(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    [[maybe_unused]] auto result = pthread_mutex_destroy(&mutex->impl.handle);
    assert(result == 0);

    std::memset(mutex, 0, sizeof(OsalMutex));
    return {};
}

OsalError osalMutexLock(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    [[maybe_unused]] auto result = pthread_mutex_lock(&mutex->impl.handle);
    assert(result == 0);
    return {};
}

OsalError osalMutexTryLock(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    auto result = pthread_mutex_trylock(&mutex->impl.handle);
    switch (result) {
        case EAGAIN: [[fallthrough]];
        case EBUSY:  return OsalError::Locked;
        default:     break;
    }

    assert(result == 0);
    return {};
}

OsalError osalMutexTryLockIsr(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    if (mutex->type == OsalMutexType::Recursive)
        return OsalError::InvalidArgument;

    return osalMutexTryLock(mutex);
}

OsalError osalMutexTimedLock(OsalMutex* mutex, uint32_t timeoutMs)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    auto toTimespec = [](const auto& timePoint) {
        auto secs = std::chrono::time_point_cast<std::chrono::seconds>(timePoint);
        auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(timePoint)
                - std::chrono::time_point_cast<std::chrono::nanoseconds>(secs);

        return timespec{static_cast<time_t>(secs.time_since_epoch().count()), static_cast<long>(ns.count())};
    };

    auto ts = toTimespec(std::chrono::system_clock::now() + std::chrono::milliseconds{timeoutMs});
    auto result = pthread_mutex_timedlock(&mutex->impl.handle, &ts);
    if (result == ETIMEDOUT)
        return OsalError::Timeout;

    assert(result == 0);
    return {};
}

OsalError osalMutexUnlock(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    [[maybe_unused]] auto result = pthread_mutex_unlock(&mutex->impl.handle);
    assert(result == 0);
    return {};
}

OsalError osalMutexUnlockIsr(OsalMutex* mutex)
{
    if (mutex == nullptr || mutex->type == OsalMutexType::Recursive)
        return OsalError::InvalidArgument;

    return osalMutexUnlock(mutex);
}
