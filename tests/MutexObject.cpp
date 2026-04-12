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
#include <osal/Mutex.hpp>
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <utility>

TEST_CASE("Mutex creation and destruction in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex;
    auto error = mutex.lock();
    CHECK_FALSE(error);

    error = mutex.unlock();
    CHECK_FALSE(error);

    osal::Mutex mutex2(type);
    error = mutex2.lock();
    CHECK_FALSE(error);

    error = mutex2.unlock();
    CHECK_FALSE(error);
}

TEST_CASE("Moving mutex around in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;
    auto error = mutex.lock();
    CHECK_FALSE(error);

    osal::Mutex mutex2(std::move(mutex));

    error = mutex.unlock(); // NOLINT(bugprone-use-after-move,clang-analyzer-cplusplus.Move,hicpp-invalid-access-moved)
    CHECK(error == OsalError::InvalidArgument);

    error = mutex2.unlock();
    CHECK_FALSE(error);
}

TEST_CASE("Lock called from two threads in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        REQUIRE_FALSE(mutex.lock());

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        REQUIRE_FALSE(mutex.unlock());
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = mutex.unlock();
    CHECK_FALSE(error);
}

TEST_CASE("TryLock called from second threads in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (mutex.tryLock())
            osal::sleep(10ms);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        REQUIRE_FALSE(mutex.unlock());
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = mutex.unlock();
    CHECK_FALSE(error);

    CHECK_FALSE(thread.join());
}

TEST_CASE("TryLock and unlock called from ISR in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.tryLockIsr();
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (mutex.tryLockIsr())
            osal::sleep(10ms);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        REQUIRE_FALSE(mutex.unlockIsr());
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = mutex.unlock();
    CHECK_FALSE(error);

    CHECK_FALSE(thread.join());
}

TEST_CASE("Recursive tryLock called from ISR in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex(OsalMutexType::Recursive);

    auto error = mutex.tryLockIsr();
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("TimedLock called from second thread, timeout in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        auto error = mutex.timedLock(100ms);
        REQUIRE(error == OsalError::Timeout);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);
    };

    osal::Thread thread(func);
    CHECK_FALSE(thread.join());

    error = mutex.unlock();
    CHECK_FALSE(error);
}

TEST_CASE("TimedLock called from second thread, success in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE_FALSE(error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        REQUIRE_FALSE(mutex.timedLock(100ms));

        auto end = osal::timestamp();
        REQUIRE((end - start) <= 100ms);

        REQUIRE_FALSE(mutex.unlock());
    };

    osal::Thread thread(func);

    osal::sleep(50ms);
    error = mutex.unlock();
    CHECK_FALSE(error);
}

TEST_CASE("Timeout used with mutexes in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;
    auto error = mutex.lock();
    REQUIRE_FALSE(error);

    osal::Timeout timeout = 100ms;
    error = mutex.timedLock(timeout);
    CHECK(error == OsalError::Timeout);
    CHECK(timeout.isExpired());

    error = mutex.unlock();
    CHECK_FALSE(error);
}
