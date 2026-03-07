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

#include "osal/ScopedLock.hpp"

#include "osal/Mutex.hpp"
#include "osal/Timeout.hpp"

#include <system_error>

namespace osal {

ScopedLock::ScopedLock(Mutex& mutex)
    : m_mutex(mutex)
{
    [[maybe_unused]] auto _ = lock();
}

ScopedLock::ScopedLock(Mutex& mutex, Timeout timeout)
    : m_mutex(mutex)
{
    [[maybe_unused]] auto _ = timedLock(timeout);
}

ScopedLock::~ScopedLock()
{
    [[maybe_unused]] auto _ = unlock();
}

std::error_code ScopedLock::lock()
{
    auto error = m_mutex.lock();
    if (!error)
        m_locked = true;

    return error;
}

std::error_code ScopedLock::timedLock(Timeout timeout)
{
    auto error = m_mutex.timedLock(timeout);
    if (!error)
        m_locked = true;

    return error;
}

std::error_code ScopedLock::unlock()
{
    auto error = m_mutex.unlock();
    if (!error)
        m_locked = false;

    return error;
}

} // namespace osal
