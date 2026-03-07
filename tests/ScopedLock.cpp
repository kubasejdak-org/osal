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
#include <osal/ScopedLock.hpp>
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create and destroy lock", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;
    osal::ScopedLock lock(mutex);
    bool locked = lock;
    bool acquired = lock.isAcquired();

    REQUIRE(locked);
    REQUIRE(acquired);

    auto error = mutex.tryLock();
    REQUIRE(error == OsalError::eLocked);
}

TEST_CASE("Create lock with timeout", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;

    {
        osal::ScopedLock lock(mutex, 100ms);
        bool locked = lock;
        REQUIRE(locked);
    }

    auto error = mutex.lock();
    REQUIRE(!error);

    {
        osal::ScopedLock lock(mutex, 100ms);
        bool locked = lock;
        REQUIRE(!locked);
    }
}

TEST_CASE("Unlock ScopedLock when destroyed", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;

    {
        osal::ScopedLock lock(mutex);
        bool locked = lock;
        REQUIRE(locked);

        auto error = mutex.tryLock();
        REQUIRE(error == OsalError::eLocked);
    }

    auto error = mutex.tryLock();
    REQUIRE(!error);

    error = mutex.unlock();
    REQUIRE(!error);
}

TEST_CASE("Non-recursive success waitLock() thread with ScopedLock in C++", "[unit][cpp][mutex]")
{
    osal::Mutex mutex;
    std::error_code threadError;

    auto func = [&mutex, &threadError] {
        osal::ScopedLock lock(mutex, 500ms);
        threadError = lock ? OsalError::eOk : OsalError::eTimeout;
    };

    osal::Thread thread;

    {
        osal::ScopedLock lock(mutex);
        bool locked = lock;
        REQUIRE(locked);

        thread.start(func);
        osal::sleep(100ms);
    }

    auto error = thread.join();
    REQUIRE(!error);
    REQUIRE(!threadError);
}
