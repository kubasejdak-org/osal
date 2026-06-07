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

#include "osal/Mutex.h"

#include "osal/Error.h"

#include <FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <cstdint>
#include <cstring>

OsalError osalMutexCreate(OsalMutex* mutex, OsalMutexType type)
{
    if (mutex == nullptr)
        return OsalError::InvalidArgument;

    mutex->initialized = false;

    SemaphoreHandle_t handle{};
#if configSUPPORT_STATIC_ALLOCATION
    switch (type) {
    #if configUSE_RECURSIVE_MUTEXES
        case OsalMutexType::Recursive: handle = xSemaphoreCreateRecursiveMutexStatic(&mutex->impl.buffer); break;
    #endif
        case OsalMutexType::NonRecursive: handle = xSemaphoreCreateMutexStatic(&mutex->impl.buffer); break;
        default:                          return OsalError::InvalidArgument;
    }
#elif configSUPPORT_DYNAMIC_ALLOCATION
    switch (type) {
    #if configUSE_RECURSIVE_MUTEXES
        case OsalMutexType::Recursive: handle = xSemaphoreCreateRecursiveMutex(); break;
    #endif
        case OsalMutexType::NonRecursive: handle = xSemaphoreCreateMutex(); break;
        default:                          return OsalError::InvalidArgument;
    }
#endif

    if (handle == nullptr)
        return OsalError::OsError;

    mutex->impl.handle = handle;
    mutex->initialized = true;
    mutex->type = type;
    return {};
}

OsalError osalMutexDestroy(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    vSemaphoreDelete(mutex->impl.handle);
    std::memset(mutex, 0, sizeof(OsalMutex));
    return {};
}

OsalError osalMutexLock(OsalMutex* mutex)
{
    auto error = osalMutexTimedLock(mutex, portMAX_DELAY);
    configASSERT(error == OsalError{});
    return error;
}

OsalError osalMutexTryLock(OsalMutex* mutex)
{
    auto error = osalMutexTimedLock(mutex, 0);
    return (error == OsalError::Timeout) ? OsalError::Locked : error;
}

OsalError osalMutexTryLockIsr(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    if (mutex->type == OsalMutexType::Recursive)
        return OsalError::InvalidArgument;

    if (xSemaphoreTakeFromISR(mutex->impl.handle, nullptr) == pdFALSE)
        return OsalError::Locked;

    return {};
}

OsalError osalMutexTimedLock(OsalMutex* mutex, uint32_t timeoutMs)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    BaseType_t result{};
    TickType_t tickTimeout = (timeoutMs == portMAX_DELAY) ? portMAX_DELAY : (timeoutMs / portTICK_PERIOD_MS);

    if (mutex->type == OsalMutexType::Recursive) {
#if configUSE_RECURSIVE_MUTEXES
        result = xSemaphoreTakeRecursive(mutex->impl.handle, tickTimeout);
#endif
    }
    else {
        result = xSemaphoreTake(mutex->impl.handle, tickTimeout);
    }

    if (result == pdFALSE)
        return OsalError::Timeout;

    return {};
}

OsalError osalMutexUnlock(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    BaseType_t result{};

    if (mutex->type == OsalMutexType::Recursive) {
#if configUSE_RECURSIVE_MUTEXES
        result = xSemaphoreGiveRecursive(mutex->impl.handle);
#endif
    }
    else {
        result = xSemaphoreGive(mutex->impl.handle);
    }

    if (result == pdFALSE)
        return OsalError::OsError;

    return {};
}

OsalError osalMutexUnlockIsr(OsalMutex* mutex)
{
    if (mutex == nullptr || !mutex->initialized)
        return OsalError::InvalidArgument;

    if (mutex->type == OsalMutexType::Recursive)
        return OsalError::InvalidArgument;

    if (xSemaphoreGiveFromISR(mutex->impl.handle, nullptr) == pdFALSE)
        return OsalError::OsError;

    return {};
}
