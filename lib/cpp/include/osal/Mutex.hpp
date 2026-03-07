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
#include "osal/Mutex.h"
#include "osal/Timeout.hpp"

#include <system_error>

namespace osal {

/// Represents OSAL mutex handle.
class Mutex {
public:
    /// Constructor. Creates new mutex with the given type.
    /// @param type         Mutex type.
    explicit Mutex(OsalMutexType type = cOsalMutexDefaultType);

    /// Copy constructor.
    /// @note This constructor is deleted, because Mutex is not meant to be copy-constructed.
    Mutex(const Mutex&) = delete;

    /// Move constructor.
    /// @param other        Object to be moved from.
    Mutex(Mutex&& other) noexcept;

    /// Destructor.
    ~Mutex();

    /// Copy assignment operator.
    /// @return Reference to self.
    /// @note This operator is deleted, because Mutex is not meant to be copy-assigned.
    Mutex& operator=(const Mutex&) = delete;

    /// Move assignment operator.
    /// @return Reference to self.
    /// @note This operator is deleted, because Mutex is not meant to be move-assigned.
    Mutex& operator=(Mutex&&) = delete;

    /// Locks the given mutex. If it is currently locked any thread, then the calling thread will block until mutex
    /// is released. If mutex is recursive, then it can be locked multiple times by the same thread.
    /// @return Error code of the operation.
    /// @note If mutex is non-recursive, then calling this function twice by the same thread will result in a deadlock.
    std::error_code lock();

    /// Locks the given mutex. If it is currently locked any thread, then it returns immediately with a proper error.
    /// @return Error code of the operation.
    /// @note If mutex is non-recursive, then calling this function twice by the same thread will not result in a
    /// deadlock.
    std::error_code tryLock();

    /// Locks the given mutex. If it is currently locked any thread, then it returns immediately with a proper error.
    /// @return Error code of the operation.
    /// @note This function will never block and is supposed to be called from ISR.
    std::error_code tryLockIsr();

    /// Locks the given mutex. If it is currently locked any thread, then the calling thread will block until mutex
    /// is released or the specified time elapses. If mutex is recursive, then it can be locked multiple times by the
    /// same thread.
    /// @param timeout      Timeout to wait for the operation.
    /// @return Error code of the operation.
    std::error_code timedLock(Timeout timeout);

    /// Unlocks the given mutex.
    /// @return Error code of the operation.
    /// @note Unlocking mutex that was already locked by another thread invokes undefined behavior.
    std::error_code unlock();

    /// Unlocks the given mutex.
    /// @return Error code of the operation.
    /// @note This function will never block and is supposed to be called from ISR.
    /// @note Unlocking mutex that was already locked by another thread invokes undefined behavior.
    std::error_code unlockIsr();

private:
    OsalMutex m_mutex{};
};

} // namespace osal
