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

#include <osal/Error.h>
#include <osal/Mutex.h>
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <cstdint>

TEST_CASE("Mutex creation and destruction", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    CHECK_FALSE(error);

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);

    error = osalMutexDestroy(&mutex);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Invalid parameters to mutex creation and destruction functions", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    auto error = osalMutexCreate(nullptr, type);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexDestroy(nullptr);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Lock and unlock from one thread", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    CHECK_FALSE(error);

    error = osalMutexLock(&mutex);
    CHECK_FALSE(error);

    error = osalMutexUnlock(&mutex);
    CHECK_FALSE(error);

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}

TEST_CASE("Invalid arguments passed to mutex functions in one thread", "[unit][c][mutex]")
{
    auto error = osalMutexLock(nullptr);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexTryLock(nullptr);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexTryLockIsr(nullptr);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexTimedLock(nullptr, 3);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexUnlock(nullptr);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexUnlockIsr(nullptr);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Lock called from two threads", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE_FALSE(error);

    error = osalMutexLock(&mutex);
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        auto error = osalMutexLock(&mutex);
        REQUIRE_FALSE(error);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        error = osalMutexUnlock(&mutex);
        REQUIRE_FALSE(error);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalMutexUnlock(&mutex);
    CHECK_FALSE(error);

    thread.join();

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}

TEST_CASE("TryLock called from second thread", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE_FALSE(error);

    error = osalMutexLock(&mutex);
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (osalMutexTryLock(&mutex))
            osal::sleep(10ms);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        auto error = osalMutexUnlock(&mutex);
        REQUIRE_FALSE(error);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalMutexUnlock(&mutex);
    CHECK_FALSE(error);

    thread.join();

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}

TEST_CASE("TryLock and unlock called from ISR", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE_FALSE(error);

    error = osalMutexTryLockIsr(&mutex);
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (osalMutexTryLockIsr(&mutex))
            osal::sleep(10ms);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        auto error = osalMutexUnlock(&mutex);
        REQUIRE_FALSE(error);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalMutexUnlockIsr(&mutex);
    CHECK_FALSE(error);

    thread.join();

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}

TEST_CASE("Recursive tryLock called from ISR", "[unit][c][mutex]")
{
    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, OsalMutexType::Recursive);
    REQUIRE_FALSE(error);

    error = osalMutexTryLockIsr(&mutex);
    CHECK(error == OsalError::InvalidArgument);

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}

TEST_CASE("TimedLock called from second thread, timeout", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE_FALSE(error);

    error = osalMutexLock(&mutex);
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        constexpr std::uint32_t cTimeoutMs = 100;
        auto error = osalMutexTimedLock(&mutex, cTimeoutMs);
        REQUIRE(error == OsalError::Timeout);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);
    };

    osal::Thread thread(func);
    thread.join();

    error = osalMutexUnlock(&mutex);
    CHECK_FALSE(error);

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}

TEST_CASE("TimedLock called from second thread, success", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::NonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::Recursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE_FALSE(error);

    error = osalMutexLock(&mutex);
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        constexpr std::uint32_t cTimeoutMs = 100;
        REQUIRE_FALSE(osalMutexTimedLock(&mutex, cTimeoutMs));

        auto end = osal::timestamp();
        REQUIRE((end - start) <= 100ms);

        REQUIRE_FALSE(osalMutexUnlock(&mutex));
    };

    osal::Thread thread(func);
    osal::sleep(50ms);

    error = osalMutexUnlock(&mutex);
    CHECK_FALSE(error);

    thread.join();

    error = osalMutexDestroy(&mutex);
    CHECK_FALSE(error);
}
