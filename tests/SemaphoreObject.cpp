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
#include <osal/Semaphore.hpp>
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <utility>

TEST_CASE("Semaphore creation and destruction in C++", "[unit][cpp][semaphore]")
{
    unsigned int initialValue{};

    SECTION("Initial value 0")
    {
        initialValue = 0;
    }

    SECTION("Initial value 1")
    {
        initialValue = 1;
    }

    SECTION("Initial value 15")
    {
        constexpr unsigned int cInitialValue = 15;
        initialValue = cInitialValue;
    }

    osal::Semaphore semaphore(initialValue);

    if (initialValue != 0) {
        auto error = semaphore.wait();
        CHECK_FALSE(error);
    }

    auto error = semaphore.signal();
    CHECK_FALSE(error);
}

TEST_CASE("Moving semaphore around in C++", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);
    auto error = semaphore.wait();
    CHECK_FALSE(error);

    osal::Semaphore semaphore2(std::move(semaphore));

    error = semaphore.signal(); // NOLINT
    CHECK(error == OsalError::InvalidArgument);

    error = semaphore2.signal();
    CHECK_FALSE(error);
}

TEST_CASE("Wait and signal from one thread in C++, start with 1", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);

    auto error = semaphore.wait();
    CHECK_FALSE(error);

    error = semaphore.signal();
    CHECK_FALSE(error);
}

TEST_CASE("Wait and signal from one thread in C++, start with 0", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(0);

    auto error = semaphore.signal();
    CHECK_FALSE(error);

    error = semaphore.wait();
    CHECK_FALSE(error);
}

TEST_CASE("Combination of wait and signal calls from one thread in C++", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(4);

    // 3.
    auto error = semaphore.wait();
    CHECK_FALSE(error);

    // 2.
    error = semaphore.wait();
    CHECK_FALSE(error);

    // 1.
    error = semaphore.wait();
    CHECK_FALSE(error);

    // 2.
    error = semaphore.signal();
    CHECK_FALSE(error);

    // 1.
    error = semaphore.wait();
    CHECK_FALSE(error);

    // 2.
    error = semaphore.signal();
    CHECK_FALSE(error);

    // 3.
    error = semaphore.signal();
    CHECK_FALSE(error);

    int count{};
    while (!semaphore.tryWait())
        ++count;

    CHECK(count == 3);
}

TEST_CASE("Increment semaphore to value bigger than the initial one in C++", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(4);
    int firstCount{};
    while (!semaphore.tryWait())
        ++firstCount;

    constexpr int cFirstExpectedCount = 4;
    CHECK(firstCount == cFirstExpectedCount);

    constexpr int cSecondExpectedCount = 16;
    for (int i = 0; i < cSecondExpectedCount; ++i) {
        auto error = semaphore.signal();
        CHECK_FALSE(error);
    }

    int secondCount{};
    while (!semaphore.tryWait())
        ++secondCount;

    CHECK(secondCount == cSecondExpectedCount);
}

TEST_CASE("Wait called from two threads in C++", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);

    auto error = semaphore.wait();
    REQUIRE_FALSE(error);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        REQUIRE_FALSE(semaphore.wait());

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        REQUIRE_FALSE(semaphore.signal());
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = semaphore.signal();
    CHECK_FALSE(error);
}

TEST_CASE("TryWait called from second thread in C++", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);

    auto error = semaphore.wait();
    REQUIRE_FALSE(error);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        while (semaphore.tryWait())
            osal::sleep(10ms);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        REQUIRE_FALSE(semaphore.signal());
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = semaphore.signal();
    CHECK_FALSE(error);
}

TEST_CASE("TryWait and signal called from ISR in C++", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);

    auto error = semaphore.tryWaitIsr();
    REQUIRE_FALSE(error);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        while (semaphore.tryWaitIsr())
            osal::sleep(10ms);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);

        REQUIRE_FALSE(semaphore.signal());
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = semaphore.signalIsr();
    CHECK_FALSE(error);
}

TEST_CASE("Multiple wait called one thread in C++", "[unit][cpp][semaphore]")
{
    unsigned int initialValue{};

    SECTION("Initial value 0")
    {
        initialValue = 0;
    }

    SECTION("Initial value 4")
    {
        constexpr unsigned int cInitialValue = 4;
        initialValue = cInitialValue;
    }

    SECTION("Initial value 15")
    {
        constexpr unsigned int cInitialValue = 15;
        initialValue = cInitialValue;
    }

    osal::Semaphore semaphore(initialValue);

    for (unsigned int i = 0; i < initialValue; ++i) {
        auto error = semaphore.wait();
        CHECK_FALSE(error);
    }

    auto error = semaphore.tryWait();
    CHECK(error == OsalError::Locked);
}

TEST_CASE("Multiple tryWait called one thread in C++", "[unit][cpp][semaphore]")
{
    unsigned int initialValue{};

    SECTION("Initial value 0")
    {
        initialValue = 0;
    }

    SECTION("Initial value 4")
    {
        constexpr unsigned int cInitialValue = 4;
        initialValue = cInitialValue;
    }

    SECTION("Initial value 15")
    {
        constexpr unsigned int cInitialValue = 15;
        initialValue = cInitialValue;
    }

    osal::Semaphore semaphore(initialValue);

    for (unsigned int i = 0; i < initialValue; ++i) {
        auto error = semaphore.tryWait();
        CHECK_FALSE(error);
    }

    auto error = semaphore.tryWait();
    CHECK(error == OsalError::Locked);
}

TEST_CASE("Multiple tryWait called from ISR in C++", "[unit][cpp][semaphore]")
{
    unsigned int initialValue{};

    SECTION("Initial value 0")
    {
        initialValue = 0;
    }

    SECTION("Initial value 4")
    {
        constexpr unsigned int cInitialValue = 4;
        initialValue = cInitialValue;
    }

    SECTION("Initial value 15")
    {
        constexpr unsigned int cInitialValue = 15;
        initialValue = cInitialValue;
    }

    osal::Semaphore semaphore(initialValue);

    for (unsigned int i = 0; i < initialValue; ++i) {
        auto error = semaphore.tryWaitIsr();
        CHECK_FALSE(error);
    }

    auto error = semaphore.tryWaitIsr();
    CHECK(error == OsalError::Locked);
}

TEST_CASE("TimedWait called from second thread in C++, timeout", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);

    auto error = semaphore.wait();
    REQUIRE_FALSE(error);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        auto error = semaphore.timedWait(100ms);
        REQUIRE(error == OsalError::Timeout);

        auto end = osal::timestamp();
        REQUIRE((end - start) >= 100ms);
    };

    osal::Thread thread(func);
    CHECK_FALSE(thread.join());

    error = semaphore.signal();
    CHECK_FALSE(error);
}

TEST_CASE("TimedWait called from second thread in C++, success", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(1);

    auto error = semaphore.wait();
    REQUIRE_FALSE(error);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        REQUIRE_FALSE(semaphore.timedWait(100ms));

        auto end = osal::timestamp();
        REQUIRE((end - start) <= 100ms);

        REQUIRE_FALSE(semaphore.signal());
    };

    osal::Thread thread(func);
    osal::sleep(50ms);

    error = semaphore.signal();
    CHECK_FALSE(error);
}

TEST_CASE("Timeout used with semaphores", "[unit][cpp][semaphore]")
{
    osal::Semaphore semaphore(0);

    osal::Timeout timeout = 100ms;
    auto error = semaphore.timedWait(timeout);
    CHECK(error == OsalError::Timeout);
    CHECK(timeout.isExpired());
}
