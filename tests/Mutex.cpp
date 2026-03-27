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

TEST_CASE("Mutex creation and destruction", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::InvalidArgument);
}

TEST_CASE("Invalid parameters to mutex creation and destruction functions", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    auto error = osalMutexCreate(nullptr, type);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexDestroy(nullptr);
    REQUIRE(error == OsalError::InvalidArgument);
}

TEST_CASE("Lock and unlock from one thread", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexLock(&mutex);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexUnlock(&mutex);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Invalid arguments passed to mutex functions in one thread", "[unit][c][mutex]")
{
    auto error = osalMutexLock(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexTryLock(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexTryLockIsr(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexTimedLock(nullptr, 3);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexUnlock(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexUnlockIsr(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);
}

TEST_CASE("Lock called from two threads", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexLock(&mutex);
    REQUIRE(error == OsalError::eOk);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        auto error = osalMutexLock(&mutex);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        error = osalMutexUnlock(&mutex);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalMutexUnlock(&mutex);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TryLock called from second thread", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexLock(&mutex);
    REQUIRE(error == OsalError::eOk);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (osalMutexTryLock(&mutex) != OsalError::eOk)
            osal::sleep(10ms);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        auto error = osalMutexUnlock(&mutex);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalMutexUnlock(&mutex);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TryLock and unlock called from ISR", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexTryLockIsr(&mutex);
    REQUIRE(error == OsalError::eOk);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (osalMutexTryLockIsr(&mutex) != OsalError::eOk)
            osal::sleep(10ms);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        auto error = osalMutexUnlock(&mutex);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalMutexUnlockIsr(&mutex);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Recursive tryLock called from ISR", "[unit][c][mutex]")
{
    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, OsalMutexType::eRecursive);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexTryLockIsr(&mutex);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TimedLock called from second thread, timeout", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexLock(&mutex);
    REQUIRE(error == OsalError::eOk);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        constexpr std::uint32_t cTimeoutMs = 100;
        auto error = osalMutexTimedLock(&mutex, cTimeoutMs);
        if (error != OsalError::Timeout)
            REQUIRE(error == OsalError::Timeout);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);
    };

    osal::Thread thread(func);
    thread.join();

    error = osalMutexUnlock(&mutex);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TimedLock called from second thread, success", "[unit][c][mutex]")
{
    OsalMutexType type{};

    SECTION("Non recursive mutex")
    {
        type = OsalMutexType::eNonRecursive;
    }

    SECTION("Recursive mutex")
    {
        type = OsalMutexType::eRecursive;
    }

    SECTION("Default mutex")
    {
        type = cOsalMutexDefaultType;
    }

    OsalMutex mutex{};
    auto error = osalMutexCreate(&mutex, type);
    REQUIRE(error == OsalError::eOk);

    error = osalMutexLock(&mutex);
    REQUIRE(error == OsalError::eOk);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        constexpr std::uint32_t cTimeoutMs = 100;
        if (auto error = osalMutexTimedLock(&mutex, cTimeoutMs))
            REQUIRE(!error);

        auto end = osal::timestamp();
        if ((end - start) > 100ms)
            REQUIRE((end - start) <= 100ms);

        if (auto error = osalMutexUnlock(&mutex))
            REQUIRE(!error);
    };

    osal::Thread thread(func);
    osal::sleep(50ms);

    error = osalMutexUnlock(&mutex);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalMutexDestroy(&mutex);
    REQUIRE(error == OsalError::eOk);
}
