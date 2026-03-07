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

#include "osal/Semaphore.hpp"

#include "osal/Semaphore.h"
#include "osal/Timeout.hpp"

#include <chrono>
#include <cstdint>
#include <system_error>
#include <utility>

namespace osal {

Semaphore::Semaphore(unsigned int initialValue)
{
    osalSemaphoreCreate(&m_semaphore, initialValue);
}

Semaphore::Semaphore(Semaphore&& other) noexcept
{
    std::swap(m_semaphore, other.m_semaphore);
}

Semaphore::~Semaphore()
{
    if (m_semaphore.initialized)
        osalSemaphoreDestroy(&m_semaphore);
}

std::error_code Semaphore::wait()
{
    return osalSemaphoreWait(&m_semaphore);
}

std::error_code Semaphore::tryWait()
{
    return osalSemaphoreTryWait(&m_semaphore);
}

std::error_code Semaphore::tryWaitIsr()
{
    return osalSemaphoreTryWaitIsr(&m_semaphore);
}

std::error_code Semaphore::timedWait(Timeout timeout)
{
    std::uint32_t timeoutMs = std::chrono::duration_cast<std::chrono::milliseconds>(timeout.timeLeft()).count();
    return osalSemaphoreTimedWait(&m_semaphore, timeoutMs);
}

std::error_code Semaphore::signal()
{
    return osalSemaphoreSignal(&m_semaphore);
}

std::error_code Semaphore::signalIsr()
{
    return osalSemaphoreSignalIsr(&m_semaphore);
}

} // namespace osal
