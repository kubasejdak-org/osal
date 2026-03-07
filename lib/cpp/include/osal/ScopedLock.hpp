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
#include "osal/Mutex.hpp"
#include "osal/Timeout.hpp"

#include <system_error>

namespace osal {

/// Helper RAII type to perform automatic lock/unlock on the specified mutex.
/// @note Objects of this class should not be shared across threads.
class ScopedLock {
public:
    /// Constructor.
    /// @param mutex        Mutex for which all the operations should be performed
    /// @note This constructor automatically locks the underlying mutex.
    explicit ScopedLock(Mutex& mutex);

    /// Constructor.
    /// @param mutex        Mutex for which all the operations should be performed
    /// @param timeout      Timeout to wait for the operation.
    /// @note This constructor automatically locks the underlying mutex.
    ScopedLock(Mutex& mutex, Timeout timeout);

    /// Copy constructor.
    /// @note This constructor is deleted, because ScopedLock is not meant to be copy-constructed.
    ScopedLock(const ScopedLock&) = delete;

    /// Move constructor.
    /// @note This constructor is deleted, because ScopedLock is not meant to be move-constructed.
    ScopedLock(ScopedLock&& other) noexcept = delete;

    /// Destructor.
    /// @note Destructor automatically unlocks the underlying mutex.
    ~ScopedLock();

    /// Copy assignment operator.
    /// @note This operator is deleted, because ScopedLock is not meant to be copy-assigned.
    ScopedLock& operator=(const ScopedLock&) = delete;

    /// Move assignment operator.
    /// @note This operator is deleted, because ScopedLock is not meant to be move-assigned.
    ScopedLock& operator=(ScopedLock&&) = delete;

    /// Returns flag indicating if the underlying mutex is locked.
    /// @return Flag indicating if the underlying mutex is locked.
    /// @retval true        Underlying mutex is locked.
    /// @retval false       Underlying mutex is not locked.
    operator bool() const { return isAcquired(); } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /// Returns flag indicating if the underlying mutex is locked.
    /// @return Flag indicating if the underlying mutex is locked.
    /// @retval true        Underlying mutex is locked.
    /// @retval false       Underlying mutex is not locked.
    [[nodiscard]] bool isAcquired() const { return m_locked; }

private:
    /// Locks the underlying mutex. If it is currently locked any thread, then the calling thread will block until mutex
    /// is released. If mutex is recursive, then it can be locked multiple times by the same thread.
    /// @return Error code of the operation.
    /// @note If mutex is non-recursive, then calling this function twice by the same thread will result in a deadlock.
    std::error_code lock();

    /// Locks the underlying mutex. If it is currently locked any thread, then the calling thread will block until mutex
    /// is released or the specified time elapses. If mutex is recursive, then it can be locked multiple times by the
    /// same thread.
    /// @param timeout      Timeout to wait for the operation.
    /// @return Error code of the operation.
    std::error_code timedLock(Timeout timeout);

    /// Unlocks the given mutex.
    /// @return Error code of the operation.
    /// @note Unlocking mutex that was already locked by another thread invokes undefined behavior.
    std::error_code unlock();

private:
    Mutex& m_mutex;
    bool m_locked{};
};

} // namespace osal
