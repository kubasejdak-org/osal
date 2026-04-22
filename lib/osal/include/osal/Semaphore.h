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

#include "internal/SemaphoreImpl.h"
#include "osal/Error.h"

#include <stdint.h>

/// Represents OSAL semaphore handle.
/// @note Size of this structure depends on the concrete implementation. In particular, SemaphoreImpl
///       contains objects from the target platform. Thus depending on its size is not recommended.
struct OsalSemaphore {
    SemaphoreImpl impl;
    bool initialized;
};

/// Creates new semaphore with the given initial value.
/// @param semaphore        Semaphore handle to be initialized.
/// @param initialValue     Initial value of the semaphore to be created.
/// @return Error code of the operation.
OsalError osalSemaphoreCreate(OsalSemaphore* semaphore, unsigned int initialValue);

/// Destroys semaphore represented by the given handle.
/// @param semaphore        Semaphore handle to be destroyed.
/// @return Error code of the operation.
OsalError osalSemaphoreDestroy(OsalSemaphore* semaphore);

/// Decrements value of the given semaphore. If its value is currently 0, then the calling thread will block until
/// semaphore is positive again.
/// @param semaphore        Semaphore to be decremented.
/// @return Error code of the operation.
OsalError osalSemaphoreWait(OsalSemaphore* semaphore);

/// Decrements value of the given semaphore. If its value is currently 0, then it returns immediately
/// with a proper error.
/// @param semaphore        Semaphore to be decremented.
/// @return Error code of the operation.
OsalError osalSemaphoreTryWait(OsalSemaphore* semaphore);

/// Decrements value of the given semaphore. If its value is currently 0, then it returns immediately
/// with a proper error.
/// @param semaphore        Semaphore to be decremented.
/// @return Error code of the operation.
/// @note This function will never block and is supposed to be called from ISR.
OsalError osalSemaphoreTryWaitIsr(OsalSemaphore* semaphore);

/// Decrements value of the given semaphore. If its value is currently 0, then the calling thread will block until
/// semaphore is positive again or the specified time elapses.
/// @param semaphore        Semaphore to be decremented.
/// @param timeoutMs        Maximal time in ms to wait for the operation.
/// @return Error code of the operation.
OsalError osalSemaphoreTimedWait(OsalSemaphore* semaphore, uint32_t timeoutMs);

/// Increments value of the given semaphore.
/// @param semaphore        Semaphore to be incremented.
/// @return Error code of the operation.
/// @note There is no upper bound of the semaphore value.
OsalError osalSemaphoreSignal(OsalSemaphore* semaphore);

/// Increments value of the given semaphore.
/// @param semaphore        Semaphore to be incremented.
/// @return Error code of the operation.
/// @note This function will never block and is supposed to be called from ISR.
/// @note There is no upper bound of the semaphore value.
OsalError osalSemaphoreSignalIsr(OsalSemaphore* semaphore);

#ifdef __cplusplus
}
#endif
