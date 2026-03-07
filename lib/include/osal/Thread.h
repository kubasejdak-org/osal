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

#include "internal/ThreadImpl.h"
#include "osal/Error.h"

#include <stddef.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)
#include <stdint.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

#include <cstddef>

/// Represents OSAL thread handle.
/// @note Size of this structure depends on the concrete implementation. In particular, ThreadImpl
///       contains objects from the target platform. Thus depending on its size is not recommended.
struct OsalThread {
    ThreadImpl impl;
    bool initialized;
};

/// Represents possible thread priorities that can be set with the OSAL API.
/// @note It is up to the concrete implementation which physical priorities will be used for each
///       enum value. The only assumption client can make, is that eLowest will set the lowest
//        possible priority in the system and eHighest will set the highest possible priority in the system.
///       All implementations will also try to set eNormal to the default priority on the target system.
enum OsalThreadPriority {
    eLowest,
    eLow,
    eNormal,
    eHigh,
    eHighest
};

/// Helper constant with default thread priority of new thread.
static const OsalThreadPriority cOsalThreadDefaultPriority = OsalThreadPriority::eNormal;

/// Helper constant with default stack size of new thread.
static const size_t cOsalThreadDefaultStackSize = static_cast<size_t>(8 * 1024);

/// Represents structure used to configuration for created thread.
/// @note Stack is not used in all configurations (e.g. Linux doesn't support it).
struct OsalThreadConfig {
    OsalThreadPriority priority;
    size_t stackSize;
    void* stack;
};

/// Represents signature of the user function that can be invoked by OSAL thread.
typedef void (*OsalThreadFunction)(void*); // NOLINT(modernize-use-using)

/// Creates and immediately starts new thread with the following configuration, main function and arguments.
/// @param thread           Thread handle to be initialized.
/// @param config           OSAL thread configuration to be used to setup new thread.
/// @param func             User function to be invoked by the new thread.
/// @param arg              User argument to be passed to the used function.
/// @return Error code of the operation.
/// @note Argument is passed as pointer, which means that it comes from the stack, then its lifetime must be
///       at least the same as created thread.
OsalError osalThreadCreate(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg);

/// Creates and immediately starts new thread with the following configuration, main function and arguments.
/// @param thread           Thread handle to be initialized.
/// @param config           OSAL thread configuration to be used to setup new thread.
/// @param func             User function to be invoked by the new thread.
/// @param arg              User argument to be passed to the used function.
/// @param name             Human readable name of the thread.
/// @return Error code of the operation.
/// @note Argument is passed as pointer, which means that it comes from the stack, then its lifetime must be
///       at least the same as created thread.
OsalError
osalThreadCreateEx(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg, const char* name);

/// Destroys thread represented by the given handle.
/// @param thread           Thread handle to be destroyed.
/// @return Error code of the operation.
/// @note Destroying thread that has not been joined can invoke undefined behavior.
OsalError osalThreadDestroy(OsalThread* thread);

/// Joins the thread represented by the given handle. If thread has not been terminated yet, then this call blocks
/// the caller until given thread is finished.
/// @param thread           Thread handle to be destroyed.
/// @return Error code of the operation.
OsalError osalThreadJoin(OsalThread* thread);

/// Invokes context switch in the scheduler on demand.
/// @note It is up to the scheduler which thread will be selected to be executed next. It is possible, that
///       it will be the same thread which called this function.
void osalThreadYield();

/// Returns numerical id of the current thread.
/// @return Numerical id of the current thread.
/// @note It is up to the concrete implementation what this number means. The only thing that caller can depend on
///       is that on the given platform this value will be unique among all created threads.
uint32_t osalThreadId();

/// Returns name of the current thread.
/// @param name             Memory block where the name should be stored.
/// @param size             Size of the given memory block.
/// @return Error code of the operation.
/// @note Returned name will be the same as the one provided in upon thread creation.
/// @note Returned name will always be NULL-terminated.
OsalError osalThreadName(char* name, size_t size);

#ifdef __cplusplus
}
#endif
