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

#include <osal/Error.hpp>
#include <osal/Mutex.hpp>
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <utility>

TEST_CASE("Mutex creation and destruction in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex;
    auto error = mutex.lock();
    REQUIRE(!error);

    error = mutex.unlock();
    REQUIRE(!error);

    osal::Mutex mutex2(type);
    error = mutex2.lock();
    REQUIRE(!error);

    error = mutex2.unlock();
    REQUIRE(!error);
}

TEST_CASE("Moving mutex around in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;
    auto error = mutex.lock();
    REQUIRE(!error);

    osal::Mutex mutex2(std::move(mutex));

    error = mutex.unlock(); // NOLINT
    REQUIRE(error == OsalError::eInvalidArgument);

    error = mutex2.unlock();
    REQUIRE(!error);
}

TEST_CASE("Lock called from two threads in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE(!error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        if (auto error = mutex.lock())
            REQUIRE(!error);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        if (auto error = mutex.unlock())
            REQUIRE(!error);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = mutex.unlock();
    REQUIRE(!error);
}

TEST_CASE("TryLock called from second threads in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE(!error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (mutex.tryLock() != OsalError::eOk)
            osal::sleep(10ms);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        if (auto error = mutex.unlock())
            REQUIRE(!error);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = mutex.unlock();
    REQUIRE(!error);

    thread.join();
}

TEST_CASE("TryLock and unlock called from ISR in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.tryLockIsr();
    REQUIRE(!error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        while (mutex.tryLockIsr() != OsalError::eOk)
            osal::sleep(10ms);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        if (auto error = mutex.unlockIsr())
            REQUIRE(!error);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = mutex.unlock();
    REQUIRE(!error);

    thread.join();
}

TEST_CASE("Recursive tryLock called from ISR in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex(OsalMutexType::eRecursive);

    auto error = mutex.tryLockIsr();
    REQUIRE(error == OsalError::eInvalidArgument);
}

TEST_CASE("TimedLock called from second thread, timeout in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE(!error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        auto error = mutex.timedLock(100ms);
        if (error != OsalError::Timeout)
            REQUIRE(error == OsalError::Timeout);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);
    };

    osal::Thread thread(func);
    thread.join();

    error = mutex.unlock();
    REQUIRE(!error);
}

TEST_CASE("TimedLock called from second thread, success in C++", "[unit][cpp][mutex]")
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

    osal::Mutex mutex(type);

    auto error = mutex.lock();
    REQUIRE(!error);

    auto func = [&mutex] {
        auto start = osal::timestamp();

        if (auto error = mutex.timedLock(100ms))
            REQUIRE(!error);

        auto end = osal::timestamp();
        if ((end - start) > 100ms)
            REQUIRE((end - start) <= 100ms);

        if (auto error = mutex.unlock())
            REQUIRE(!error);
    };

    osal::Thread thread(func);

    osal::sleep(50ms);
    error = mutex.unlock();
    REQUIRE(!error);
}

TEST_CASE("Timeout used with mutexes in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;
    auto error = mutex.lock();
    REQUIRE(!error);

    osal::Timeout timeout = 100ms;
    error = mutex.timedLock(timeout);
    REQUIRE(error == OsalError::eTimeout);
    REQUIRE(timeout.isExpired());

    error = mutex.unlock();
    REQUIRE(!error);
}
