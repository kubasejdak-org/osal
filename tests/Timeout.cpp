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

#include <osal/Timeout.hpp>
#include <osal/sleep.hpp>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iterator>
#include <vector>

TEST_CASE("Creation of timeout", "[unit][cpp][timeout]")
{
    osal::Timeout t1(3s);
    osal::Timeout t2(4ms);
    osal::Timeout t3(1ms);
    osal::Timeout t4(8ms);
    osal::Timeout t5 = 100ms;
    osal::Timeout t6(t2);
    osal::Timeout t7 = t4;
    osal::Timeout t8(100ms, true);

    CHECK(t1.duration() == 3s);
    CHECK(!t1.isInfinity());
    CHECK(t2.duration() == 4ms);
    CHECK(!t2.isInfinity());
    CHECK(t3.duration() == 1ms);
    CHECK(!t3.isInfinity());
    CHECK(t4.duration() == 8ms);
    CHECK(!t4.isInfinity());
    CHECK(t5.duration() == 100ms);
    CHECK(!t5.isInfinity());
    CHECK(t6.duration() == t2.duration());
    CHECK(!t6.isInfinity());
    CHECK(t7.duration() == t4.duration());
    CHECK(!t7.isInfinity());
    CHECK(t8.duration() == 100ms);
    CHECK(!t8.isInfinity());

    // Make all timeouts expired.
    constexpr auto cDelay = 5s;
    osal::sleep(cDelay);

    CHECK(t1.isExpired());
    CHECK(t1.isExpired());
    CHECK(t1.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t2.isExpired());
    CHECK(t2.isExpired());
    CHECK(t2.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t3.isExpired());
    CHECK(t3.isExpired());
    CHECK(t3.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t4.isExpired());
    CHECK(t4.isExpired());
    CHECK(t4.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t5.isExpired());
    CHECK(t5.isExpired());
    CHECK(t5.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t6.isExpired());
    CHECK(t6.isExpired());
    CHECK(t6.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t7.isExpired());
    CHECK(t7.isExpired());
    CHECK(t7.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(t8.isExpired());
    CHECK(t8.isExpired());
    CHECK(t8.timeLeft() == std::chrono::milliseconds::zero());
}

TEST_CASE("Creation of special timeout values", "[unit][cpp][timeout]")
{
    auto t1 = osal::Timeout::none();
    auto t2 = osal::Timeout::infinity();

    CHECK(t1.duration() == std::chrono::milliseconds::zero());
    CHECK(!t1.isInfinity());
    CHECK(t2.duration() == std::chrono::milliseconds::max());
    CHECK(t2.isInfinity());

    // Make all timeouts expired.
    constexpr auto cDelay = 5s;
    osal::sleep(cDelay);

    CHECK(t1.isExpired());
    CHECK(t1.isExpired());
    CHECK(t1.timeLeft() == std::chrono::milliseconds::zero());
    CHECK(!t2.isExpired());
    CHECK(!t2.isExpired());
    CHECK(t2.timeLeft() == std::chrono::milliseconds::max());
}

TEST_CASE("Timeout is monotonic", "[unit][cpp][timeout]")
{
    constexpr auto cTimeout = 5s;
    osal::Timeout t(cTimeout);

    std::vector<std::uint64_t> timesLeft;
    while (!t.isExpired()) {
        constexpr auto cDelay = 100ms;
        osal::sleep(cDelay);
        timesLeft.push_back(t.timeLeft().count());
    }

    CHECK(t.timeLeft().count() == 0);
    CHECK(std::ranges::is_sorted(timesLeft, std::greater{}));
}

TEST_CASE("Timeout used as a function argument", "[unit][cpp][timeout]")
{
    constexpr auto cTimeout = 250ms;
    osal::Timeout timeout = cTimeout;

    auto start = osal::timestamp();

    SECTION("Measuring time elapsed until deadline is reached")
    {
        auto func = [](osal::Timeout timeout) {
            while (!timeout.isExpired()) {
                constexpr auto cDelay = 100ms;
                osal::sleep(cDelay);
            }
        };

        func(timeout);
    }

    SECTION("Timeout is propagated deep in the callstack")
    {
        auto dummySleepFunc = [=] { osal::sleep(cTimeout + 10ms); };

        auto func1 = [=](osal::Timeout timeout) {
            dummySleepFunc();
            return timeout.isExpired();
        };

        auto func2 = [=](osal::Timeout timeout) {
            bool expired = func1(timeout);
            CHECK(expired);
            CHECK(timeout.isExpired());
            return expired;
        };

        auto func3 = [=](osal::Timeout timeout) {
            bool expired = func2(timeout);
            CHECK(expired);
            CHECK(timeout.isExpired());
            return expired;
        };

        auto func4 = [=](osal::Timeout timeout) {
            bool expired = func3(timeout);
            CHECK(expired);
            CHECK(timeout.isExpired());
            return expired;
        };

        auto func5 = [=](osal::Timeout timeout) {
            bool expired = func4(timeout);
            CHECK(expired);
            CHECK(timeout.isExpired());
            return expired;
        };

        bool expired = func5(timeout);
        CHECK(expired);
    }

    auto end = osal::timestamp();
    [[maybe_unused]] auto diff = (end - start).count();

    CHECK(timeout.isExpired());
    CHECK(timeout.timeLeft() == std::chrono::milliseconds::zero());
    CHECK((end - start) >= cTimeout);
}

TEST_CASE("Resetting timeout", "[unit][cpp][timeout]")
{
    osal::Timeout t1 = 400ms;
    osal::Timeout t2 = 100ms;

    constexpr int cIterationsCount = 10;
    for (int i = 0; i < cIterationsCount; ++i) {
        // Make all timeouts expired.
        constexpr auto cDelay = 500ms;
        osal::sleep(cDelay);

        CHECK(t1.isExpired());
        CHECK(t2.isExpired());

        t1.reset();
        CHECK(!t1.isExpired());

        t2.reset();
        CHECK(!t2.isExpired());
    }
}

TEST_CASE("Sleeping until timeout is expired", "[unit][cpp][timeout]")
{
    SECTION("Sleeping for timeout expiration is required")
    {
        osal::Timeout timeout = 400ms;
        CHECK(!timeout.isExpired());

        osal::sleepUntilExpired(timeout);
        CHECK(timeout.isExpired());
    }

    SECTION("Sleeping for timeout expiration is not required")
    {
        osal::Timeout timeout = 0ms;
        CHECK(timeout.isExpired());

        osal::sleepUntilExpired(timeout);
        CHECK(timeout.isExpired());
    }
}

TEST_CASE("Converting duration to raw milliseconds", "[unit][cpp][timeout]")
{
    osal::Timeout t1(2s);
    osal::Timeout t2(16min);
    osal::Timeout t3(546ms);
    osal::Timeout t4(13ms);
    osal::Timeout t5(0ms);

    CHECK(osal::durationMs(t1) == (2 * 1000));
    CHECK(osal::durationMs(t2) == (16 * 60 * 1000));
    CHECK(osal::durationMs(t3) == 546);
    CHECK(osal::durationMs(t4) == 13);
    CHECK(osal::durationMs(t5) == 0);
}
