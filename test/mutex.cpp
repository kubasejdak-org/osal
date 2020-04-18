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

#include <osal/mutex.h>
#include <osal/sleep.hpp>
#include <osal/thread.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Mutex creation and destruction", "[unit][c][mutex]")
{
    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, OsalMutexType::eNonRecursive);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eInvalidArgument);

    OsalMutex mutex2{};
    error = osalMutexCreate(&mutex2, OsalMutexType::eRecursive);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex2);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex2);
    REQUIRE(error == OsalError::eInvalidArgument);

    OsalMutex mutex3{};
    error = osalMutexCreate(&mutex3, cOsalMutexDefaultType);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex3);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex3);
    REQUIRE(error == OsalError::eInvalidArgument);
}

TEST_CASE("Invalid parameters to mutex creation and destruction functions", "[unit][c][mutex]")
{
    auto error = osalMutexCreate(nullptr, OsalMutexType::eNonRecursive);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexCreate(nullptr, OsalMutexType::eRecursive);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexCreate(nullptr, cOsalMutexDefaultType);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexDestroy(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    OsalMutex mutex{};
    error = osalMutexCreate(&mutex, static_cast<OsalMutexType>(4));
    REQUIRE(error == OsalError::eInvalidArgument);
}
