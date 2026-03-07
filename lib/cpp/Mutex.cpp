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

#include "osal/Mutex.hpp"

#include "osal/Mutex.h"
#include "osal/Timeout.hpp"

#include <chrono>
#include <cstdint>
#include <system_error>
#include <utility>

namespace osal {

Mutex::Mutex(OsalMutexType type)
{
    osalMutexCreate(&m_mutex, type);
}

Mutex::Mutex(Mutex&& other) noexcept
{
    std::swap(m_mutex, other.m_mutex);
}

Mutex::~Mutex()
{
    if (m_mutex.initialized)
        osalMutexDestroy(&m_mutex);
}

std::error_code Mutex::lock()
{
    return osalMutexLock(&m_mutex);
}

std::error_code Mutex::tryLock()
{
    return osalMutexTryLock(&m_mutex);
}

std::error_code Mutex::tryLockIsr()
{
    return osalMutexTryLockIsr(&m_mutex);
}

std::error_code Mutex::timedLock(Timeout timeout)
{
    std::uint32_t timeoutMs = std::chrono::duration_cast<std::chrono::milliseconds>(timeout.timeLeft()).count();
    return osalMutexTimedLock(&m_mutex, timeoutMs);
}

std::error_code Mutex::unlock()
{
    return osalMutexUnlock(&m_mutex);
}

std::error_code Mutex::unlockIsr()
{
    return osalMutexUnlockIsr(&m_mutex);
}

} // namespace osal
