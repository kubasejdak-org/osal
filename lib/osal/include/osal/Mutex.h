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

#ifdef __cplusplus
extern "C" {
#endif

#include "internal/MutexImpl.h"
#include "osal/Error.h"

#include <stdint.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

/// Represents possible types of the OSAL mutex. These types define how mutex will react to multiple recursive
/// locks made by the same thread.
enum OsalMutexType {
    eRecursive,
    eNonRecursive
};

/// Represents OSAL mutex handle.
/// @note Size of this structure depends on the concrete implementation. In particular, MutexImpl
///       contains objects from the target platform. Thus depending on its size is not recommended.
struct OsalMutex {
    MutexImpl impl;
    OsalMutexType type;
    bool initialized;
};

/// Helper constant with default mutex type.
static const OsalMutexType cOsalMutexDefaultType = OsalMutexType::eNonRecursive;

/// Creates new mutex with the given type.
/// @param mutex            Mutex handle to be initialized.
/// @param type             Type of the mutex to be created.
/// @return Error code of the operation.
/// @note Created mutex is in unlocked state.
OsalError osalMutexCreate(OsalMutex* mutex, OsalMutexType type);

/// Destroys mutex represented by the given handle.
/// @param mutex            Mutex handle to be destroyed.
/// @return Error code of the operation.
OsalError osalMutexDestroy(OsalMutex* mutex);

/// Locks the given mutex. If it is currently locked any thread, then the calling thread will block until mutex
/// is released. If mutex is recursive, then it can be locked multiple times by the same thread.
/// @param mutex            Mutex to be locked.
/// @return Error code of the operation.
/// @note If mutex is non-recursive, then calling this function twice by the same thread will result in a deadlock.
OsalError osalMutexLock(OsalMutex* mutex);

/// Locks the given mutex. If it is currently locked any thread, then it returns immediately with a proper error.
/// @param mutex            Mutex to be locked.
/// @return Error code of the operation.
/// @note If mutex is non-recursive, then calling this function twice by the same thread will not result in a deadlock.
OsalError osalMutexTryLock(OsalMutex* mutex);

/// Locks the given mutex. If it is currently locked any thread, then it returns immediately with a proper error.
/// @param mutex            Mutex to be locked.
/// @return Error code of the operation.
/// @note This function will never block and is supposed to be called from ISR.
OsalError osalMutexTryLockIsr(OsalMutex* mutex);

/// Locks the given mutex. If it is currently locked any thread, then the calling thread will block until mutex
/// is released or the specified time elapses. If mutex is recursive, then it can be locked multiple times by the
/// same thread.
/// @param mutex            Mutex to be locked.
/// @param timeoutMs        Maximal time in ms to wait for the operation.
/// @return Error code of the operation.
OsalError osalMutexTimedLock(OsalMutex* mutex, uint32_t timeoutMs);

/// Unlocks the given mutex.
/// @param mutex            Mutex to be unlocked.
/// @return Error code of the operation.
/// @note Unlocking mutex that was already locked by another thread invokes undefined behavior.
OsalError osalMutexUnlock(OsalMutex* mutex);

/// Unlocks the given mutex.
/// @param mutex            Mutex to be unlocked.
/// @return Error code of the operation.
/// @note This function will never block and is supposed to be called from ISR.
/// @note Unlocking mutex that was already locked by another thread invokes undefined behavior.
OsalError osalMutexUnlockIsr(OsalMutex* mutex);

#ifdef __cplusplus
}
#endif
