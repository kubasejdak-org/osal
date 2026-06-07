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

#include "osal/Semaphore.h"

#include "osal/Error.h"

#include <FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <cstdint>
#include <cstring>
#include <limits>

OsalError osalSemaphoreCreate(OsalSemaphore* semaphore, unsigned int initValue)
{
    if (semaphore == nullptr)
        return OsalError::InvalidArgument;

    semaphore->initialized = false;

    SemaphoreHandle_t handle{};
#if configSUPPORT_STATIC_ALLOCATION
    handle = xSemaphoreCreateCountingStatic(std::numeric_limits<BaseType_t>::max(), initValue, &semaphore->impl.buffer);
#elif configSUPPORT_DYNAMIC_ALLOCATION
    handle = xSemaphoreCreateCounting(std::numeric_limits<BaseType_t>::max(), initValue);
#endif

    if (handle == nullptr)
        return OsalError::OsError;

    semaphore->impl.handle = handle;
    semaphore->initialized = true;
    return {};
}

OsalError osalSemaphoreDestroy(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    vSemaphoreDelete(semaphore->impl.handle);
    std::memset(semaphore, 0, sizeof(OsalSemaphore));
    return {};
}

OsalError osalSemaphoreWait(OsalSemaphore* semaphore)
{
    auto error = osalSemaphoreTimedWait(semaphore, portMAX_DELAY);
    configASSERT(error == OsalError{});
    return error;
}

OsalError osalSemaphoreTryWait(OsalSemaphore* semaphore)
{
    auto error = osalSemaphoreTimedWait(semaphore, 0);
    return (error == OsalError::Timeout) ? OsalError::Locked : error;
}

OsalError osalSemaphoreTryWaitIsr(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    if (xSemaphoreTakeFromISR(semaphore->impl.handle, nullptr) == pdFALSE)
        return OsalError::Locked;

    return {};
}

OsalError osalSemaphoreTimedWait(OsalSemaphore* semaphore, uint32_t timeoutMs)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    TickType_t tickTimeout = (timeoutMs == portMAX_DELAY) ? portMAX_DELAY : (timeoutMs / portTICK_PERIOD_MS);
    if (xSemaphoreTake(semaphore->impl.handle, tickTimeout) == pdFALSE)
        return OsalError::Timeout;

    return {};
}

OsalError osalSemaphoreSignal(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    if (xSemaphoreGive(semaphore->impl.handle) == pdFALSE)
        return OsalError::OsError;

    return {};
}

OsalError osalSemaphoreSignalIsr(OsalSemaphore* semaphore)
{
    if (semaphore == nullptr || !semaphore->initialized)
        return OsalError::InvalidArgument;

    if (xSemaphoreGiveFromISR(semaphore->impl.handle, nullptr) == pdFALSE)
        return OsalError::OsError;

    return {};
}
