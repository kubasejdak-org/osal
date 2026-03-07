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

#pragma once

#include "osal/Error.hpp"
#include "osal/Semaphore.h"
#include "osal/Timeout.hpp"

#include <system_error>

namespace osal {

/// Represents OSAL semaphore handle.
class Semaphore {
public:
    /// Constructor. Creates new semaphore with the given type.
    /// @param initialValue     Initial value of the semaphore to be created.
    explicit Semaphore(unsigned int initialValue);

    /// Copy constructor.
    /// @note This constructor is deleted, because Semaphore is not meant to be copy-constructed.
    Semaphore(const Semaphore&) = delete;

    /// Move constructor.
    /// @param other            Object to be moved from.
    Semaphore(Semaphore&& other) noexcept;

    /// Destructor.
    ~Semaphore();

    /// Copy assignment operator.
    /// @return Reference to self.
    /// @note This operator is deleted, because Semaphore is not meant to be copy-assigned.
    Semaphore& operator=(const Semaphore&) = delete;

    /// Move assignment operator.
    /// @return Reference to self.
    /// @note This operator is deleted, because Semaphore is not meant to be move-assigned.
    Semaphore& operator=(Semaphore&&) = delete;

    /// Decrements value of the given semaphore. If its value is currently 0, then the calling thread will block until
    /// semaphore is positive again.
    /// @return Error code of the operation.
    std::error_code wait();

    /// Decrements value of the given semaphore. If its value is currently 0, then it returns immediately
    /// with a proper error.
    /// @return Error code of the operation.
    std::error_code tryWait();

    /// Decrements value of the given semaphore. If its value is currently 0, then it returns immediately
    /// with a proper error.
    /// @return Error code of the operation.
    /// @note This function will never block and is supposed to be called from ISR.
    std::error_code tryWaitIsr();

    /// Decrements value of the given semaphore. If its value is currently 0, then the calling thread will block until
    /// semaphore is positive again or the specified time elapses.
    /// @param timeout          Maximal time to wait for the operation.
    /// @return Error code of the operation.
    std::error_code timedWait(Timeout timeout);

    /// Increments value of the given semaphore.
    /// @return Error code of the operation.
    /// @note There is no upper bound of the semaphore value.
    std::error_code signal();

    /// Increments value of the given semaphore.
    /// @return Error code of the operation.
    /// @note This function will never block and is supposed to be called from ISR.
    /// @note There is no upper bound of the semaphore value.
    std::error_code signalIsr();

private:
    OsalSemaphore m_semaphore{};
};

} // namespace osal
