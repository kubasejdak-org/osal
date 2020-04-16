/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2020-2020, Kuba Sejdak <kuba.sejdak@gmail.com>
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this
///    list of conditions and the following disclaimer.
///
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "error.h"
#include "internal/mutexImpl.h"

#include <stdint.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

// clang-format off
/// @enum OsalMutexType
/// Represents possible types of the OSAL mutex. These types define how mutex will react to multiple recursive
/// locks made by the same thread.
enum OsalMutexType {
    eRecursive,
    eNonRecursive
};
// clang-format on

/// @struct OsalMutex
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

OsalError osalMutexCreate(OsalMutex* mutex, OsalMutexType type);
OsalError osalMutexDestroy(OsalMutex* mutex);
OsalError osalMutexLock(OsalMutex* mutex);
OsalError osalMutexTryLock(OsalMutex* mutex);
OsalError osalMutexTryLockIsr(OsalMutex* mutex);
OsalError osalMutexTimedLock(OsalMutex* mutex, uint32_t timeoutMs);
OsalError osalMutexUnlock(OsalMutex* mutex);
OsalError osalMutexUnlockIsr(OsalMutex* mutex);

#ifdef __cplusplus
}
#endif
