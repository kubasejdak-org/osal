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
#include <osal/Semaphore.h>
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Semaphore creation and destruction", "[unit][c][semaphore]")
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

    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, initialValue);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::InvalidArgument);
}

TEST_CASE("Invalid parameters to semaphore creation and destruction functions", "[unit][c][semaphore]")
{
    auto error = osalSemaphoreCreate(nullptr, 3);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalSemaphoreDestroy(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);
}

TEST_CASE("Wait and signal from one thread, start with 1", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 1);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Wait and signal from one thread, start with 0", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 0);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Invalid arguments passed to semaphore functions in one thread", "[unit][c][semaphore]")
{
    auto error = osalSemaphoreWait(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalSemaphoreTryWait(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalSemaphoreTryWaitIsr(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalSemaphoreTimedWait(nullptr, 3);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalSemaphoreSignal(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);

    error = osalSemaphoreSignalIsr(nullptr);
    REQUIRE(error == OsalError::eInvalidArgument);
}

TEST_CASE("Combination of wait and signal calls from one thread", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 4);
    REQUIRE(error == OsalError::eOk);

    // 3.
    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    // 2.
    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    // 1.
    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    // 2.
    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    // 1.
    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    // 2.
    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    // 3.
    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    int count{};
    while (osalSemaphoreTryWait(&semaphore) == OsalError::eOk)
        ++count;

    REQUIRE(count == 3);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Increment semaphore to value bigger than the initial one", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 4);
    REQUIRE(error == OsalError::eOk);

    int firstCount{};
    while (osalSemaphoreTryWait(&semaphore) == OsalError::eOk)
        ++firstCount;

    constexpr int cFirstExpectedCount = 4;
    REQUIRE(firstCount == cFirstExpectedCount);

    constexpr int cSecondExpectedCount = 16;
    for (int i = 0; i < cSecondExpectedCount; ++i) {
        error = osalSemaphoreSignal(&semaphore);
        REQUIRE(error == OsalError::eOk);
    }

    int secondCount{};
    while (osalSemaphoreTryWait(&semaphore) == OsalError::eOk)
        ++secondCount;

    REQUIRE(secondCount == cSecondExpectedCount);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Wait called from two threads", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 1);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        auto error = osalSemaphoreWait(&semaphore);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        error = osalSemaphoreSignal(&semaphore);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TryWait called from second thread", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 1);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        while (osalSemaphoreTryWait(&semaphore) != OsalError::eOk)
            osal::sleep(10ms);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        auto error = osalSemaphoreSignal(&semaphore);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TryWait and signal called from ISR", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 1);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreTryWaitIsr(&semaphore);
    REQUIRE(error == OsalError::eOk);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        while (osalSemaphoreTryWaitIsr(&semaphore) != OsalError::eOk)
            osal::sleep(10ms);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);

        auto error = osalSemaphoreSignal(&semaphore);
        if (error != OsalError::eOk)
            REQUIRE(error == OsalError::eOk);
    };

    osal::Thread thread(func);

    osal::sleep(120ms);
    error = osalSemaphoreSignalIsr(&semaphore);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Multiple wait called one thread", "[unit][c][semaphore]")
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

    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, initialValue);
    REQUIRE(error == OsalError::eOk);

    for (unsigned int i = 0; i < initialValue; ++i) {
        error = osalSemaphoreWait(&semaphore);
        REQUIRE(error == OsalError::eOk);
    }

    error = osalSemaphoreTryWait(&semaphore);
    REQUIRE(error == OsalError::Locked);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Multiple tryWait called one thread", "[unit][c][semaphore]")
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

    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, initialValue);
    REQUIRE(error == OsalError::eOk);

    for (unsigned int i = 0; i < initialValue; ++i) {
        error = osalSemaphoreTryWait(&semaphore);
        REQUIRE(error == OsalError::eOk);
    }

    error = osalSemaphoreTryWait(&semaphore);
    REQUIRE(error == OsalError::Locked);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("Multiple tryWait called from ISR", "[unit][c][semaphore]")
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

    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, initialValue);
    REQUIRE(error == OsalError::eOk);

    for (unsigned int i = 0; i < initialValue; ++i) {
        error = osalSemaphoreTryWaitIsr(&semaphore);
        REQUIRE(error == OsalError::eOk);
    }

    error = osalSemaphoreTryWaitIsr(&semaphore);
    REQUIRE(error == OsalError::Locked);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TimedWait called from second thread, timeout", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 1);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        constexpr std::uint32_t cTimeoutMs = 100;
        auto error = osalSemaphoreTimedWait(&semaphore, cTimeoutMs);
        if (error != OsalError::eTimeout)
            REQUIRE(error == OsalError::eTimeout);

        auto end = osal::timestamp();
        if ((end - start) < 100ms)
            REQUIRE((end - start) >= 100ms);
    };

    osal::Thread thread(func);
    thread.join();

    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}

TEST_CASE("TimedWait called from second thread, success", "[unit][c][semaphore]")
{
    OsalSemaphore semaphore{};
    auto error = osalSemaphoreCreate(&semaphore, 1);
    REQUIRE(error == OsalError::eOk);

    error = osalSemaphoreWait(&semaphore);
    REQUIRE(error == OsalError::eOk);

    auto func = [&semaphore] {
        auto start = osal::timestamp();

        constexpr std::uint32_t cTimeoutMs = 100;
        if (auto error = osalSemaphoreTimedWait(&semaphore, cTimeoutMs))
            REQUIRE(!error);

        auto end = osal::timestamp();
        if ((end - start) > 100ms)
            REQUIRE((end - start) <= 100ms);

        if (auto error = osalSemaphoreSignal(&semaphore))
            REQUIRE(!error);
    };

    osal::Thread thread(func);
    osal::sleep(50ms);

    error = osalSemaphoreSignal(&semaphore);
    REQUIRE(error == OsalError::eOk);

    thread.join();

    error = osalSemaphoreDestroy(&semaphore);
    REQUIRE(error == OsalError::eOk);
}
