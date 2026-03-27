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

#include "osal/Semaphore.h"

#include "osal/Error.h"

#include <cassert>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>

OsalError osalSemaphoreCreate(OsalSemaphore* semaphore, unsigned int initialValue)
{
    if (semaphore == nullptr)
        return OsalError::InvalidArgument;

    sem_t handle{};
    [[maybe_unused]] auto result = sem_init(&handle, 0, initialValue);
    assert(result == 0);

    semaphore->impl.handle = handle;
    semaphore->initialized = true;
    return {};
}

OsalError osalSemaphoreDestroy(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    [[maybe_unused]] auto result = sem_destroy(&semaphore->impl.handle);
    assert(result == 0);

    std::memset(semaphore, 0, sizeof(OsalSemaphore));
    return {};
}

OsalError osalSemaphoreWait(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    [[maybe_unused]] auto result = sem_wait(&semaphore->impl.handle);
    assert(result == 0);
    return {};
}

OsalError osalSemaphoreTryWait(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    auto result = sem_trywait(&semaphore->impl.handle);
    if (result == -1)
        return OsalError::Locked;

    assert(result == 0);
    return {};
}

OsalError osalSemaphoreTryWaitIsr(OsalSemaphore* semaphore)
{
    return osalSemaphoreTryWait(semaphore);
}

OsalError osalSemaphoreTimedWait(OsalSemaphore* semaphore, uint32_t timeoutMs)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    auto toTimespec = [](const auto& timePoint) {
        auto secs = std::chrono::time_point_cast<std::chrono::seconds>(timePoint);
        auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(timePoint)
                - std::chrono::time_point_cast<std::chrono::nanoseconds>(secs);

        return timespec{int(secs.time_since_epoch().count()), int(ns.count())};
    };

    auto ts = toTimespec(std::chrono::system_clock::now() + std::chrono::milliseconds{timeoutMs});
    auto result = sem_timedwait(&semaphore->impl.handle, &ts);
    if ((result == -1) && (errno == ETIMEDOUT))
        return OsalError::Timeout;

    assert(result == 0);
    return {};
}

OsalError osalSemaphoreSignal(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    [[maybe_unused]] auto result = sem_post(&semaphore->impl.handle);
    assert(result == 0);
    return {};
}

OsalError osalSemaphoreSignalIsr(OsalSemaphore* semaphore)
{
    return osalSemaphoreSignal(semaphore);
}
