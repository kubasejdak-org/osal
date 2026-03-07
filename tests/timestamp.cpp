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

#include <osal/sleep.h>
#include <osal/sleep.hpp>
#include <osal/timestamp.h>
#include <osal/timestamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <cstdint>

TEST_CASE("Check C timestamp values after multiple delays in ms", "[unit][c][timestamp]")
{
    std::uint64_t delayMs{};

    SECTION("Delay 500ms")
    {
        constexpr std::uint64_t cDelayMs = 500;
        delayMs = cDelayMs;
    }

    SECTION("Delay 1ms")
    {
        constexpr std::uint64_t cDelayMs = 1;
        delayMs = cDelayMs;
    }

    SECTION("Delay 0ms")
    {
        constexpr std::uint64_t cDelayMs = 0;
        delayMs = cDelayMs;
    }

    auto now1 = osalTimestampMs();
    osalSleepMs(delayMs);
    auto now2 = osalTimestampMs();
    const auto cDelay2Ms = 2 * delayMs;
    osalSleepMs(cDelay2Ms);
    auto now3 = osalTimestampMs();

    constexpr std::uint64_t cMarginMs = 50;
    REQUIRE(now2 >= now1);
    REQUIRE((now2 - now1) >= delayMs);
    REQUIRE((now2 - now1) <= (delayMs + cMarginMs));
    REQUIRE(now3 >= now2);
    REQUIRE(now3 >= now1);
    REQUIRE((now3 - now2) >= cDelay2Ms);
    REQUIRE((now3 - now2) <= (cDelay2Ms + cMarginMs));
    REQUIRE((now3 - now1) >= (cDelay2Ms + delayMs));
    REQUIRE((now3 - now1) <= (cDelay2Ms + delayMs + 2 * cMarginMs));
}

TEST_CASE("Check C timestamp values after multiple delays in us", "[unit][c][timestamp]")
{
    std::uint64_t delayMs{};

    SECTION("Delay 500ms")
    {
        constexpr std::uint64_t cDelayMs = 500;
        delayMs = cDelayMs;
    }

    SECTION("Delay 1ms")
    {
        constexpr std::uint64_t cDelayMs = 1;
        delayMs = cDelayMs;
    }

    SECTION("Delay 0ms")
    {
        constexpr std::uint64_t cDelayMs = 0;
        delayMs = cDelayMs;
    }

    auto now1 = osalTimestampUs();
    osalSleepMs(delayMs);
    auto now2 = osalTimestampUs();
    const auto cDelay2Ms = 2 * delayMs;
    osalSleepMs(cDelay2Ms);
    auto now3 = osalTimestampUs();

    const std::uint64_t cMarginMs = 50;
    REQUIRE(now2 >= now1);
    REQUIRE((now2 - now1) >= osalMsToUs(delayMs));
    REQUIRE((now2 - now1) <= osalMsToUs(delayMs + cMarginMs));
    REQUIRE(now3 >= now2);
    REQUIRE(now3 >= now1);
    REQUIRE((now3 - now2) >= osalMsToUs(cDelay2Ms));
    REQUIRE((now3 - now2) <= osalMsToUs(cDelay2Ms + cMarginMs));
    REQUIRE((now3 - now1) >= osalMsToUs(cDelay2Ms + delayMs));
    REQUIRE((now3 - now1) <= osalMsToUs(cDelay2Ms + delayMs + 2 * cMarginMs));
}

TEST_CASE("Check C timestamp values after multiple delays in ns", "[unit][c][timestamp]")
{
    std::uint64_t delayMs{};

    SECTION("Delay 500ms")
    {
        constexpr std::uint64_t cDelayMs = 500;
        delayMs = cDelayMs;
    }

    SECTION("Delay 1ms")
    {
        constexpr std::uint64_t cDelayMs = 1;
        delayMs = cDelayMs;
    }

    SECTION("Delay 0ms")
    {
        constexpr std::uint64_t cDelayMs = 0;
        delayMs = cDelayMs;
    }

    auto now1 = osalTimestampNs();
    osalSleepMs(delayMs);
    auto now2 = osalTimestampNs();
    const auto cDelay2Ms = 2 * delayMs;
    osalSleepMs(cDelay2Ms);
    auto now3 = osalTimestampNs();

    const std::uint64_t cMarginMs = 50;
    REQUIRE(now2 >= now1);
    REQUIRE((now2 - now1) >= osalMsToNs(delayMs));
    REQUIRE((now2 - now1) <= osalMsToNs(delayMs + cMarginMs));
    REQUIRE(now3 >= now2);
    REQUIRE(now3 >= now1);
    REQUIRE((now3 - now2) >= osalMsToNs(cDelay2Ms));
    REQUIRE((now3 - now2) <= osalMsToNs(cDelay2Ms + cMarginMs));
    REQUIRE((now3 - now1) >= osalMsToNs(cDelay2Ms + delayMs));
    REQUIRE((now3 - now1) <= osalMsToNs(cDelay2Ms + delayMs + 2 * cMarginMs));
}

TEST_CASE("Check C++ timestamp values after multiple delays", "[unit][cpp][timestamp]")
{
    std::chrono::milliseconds delay{};

    SECTION("Delay 1ms")
    {
        constexpr auto cDelay = 1ms;
        delay = cDelay;
    }

    SECTION("Delay 500ms")
    {
        constexpr auto cDelay = 500ms;
        delay = cDelay;
    }

    SECTION("Delay 0ms")
    {
        constexpr auto cDelay = 0ms;
        delay = cDelay;
    }

    SECTION("Delay 1s")
    {
        constexpr auto cDelay = 1s;
        delay = cDelay;
    }

    auto now1 = osal::timestamp();
    osal::sleep(delay);
    auto now2 = osal::timestamp();
    const auto cDelay2 = 2 * delay;
    osal::sleep(cDelay2);
    auto now3 = osal::timestamp();

    constexpr auto cMargin = 50ms;
    REQUIRE(now2 >= now1);
    REQUIRE((now2 - now1) >= delay);
    REQUIRE((now2 - now1) <= (delay + cMargin));
    REQUIRE(now3 >= now2);
    REQUIRE(now3 >= now1);
    REQUIRE((now3 - now2) >= cDelay2);
    REQUIRE((now3 - now2) <= (cDelay2 + cMargin));
    REQUIRE((now3 - now1) >= (cDelay2 + delay));
    REQUIRE((now3 - now1) <= (cDelay2 + delay + 2 * cMargin));
}

TEST_CASE("Check helper conversion functions", "[unit][c][timestamp]")
{
    auto result = osalMsToSec(45); // NOLINT
    REQUIRE(result == 0);

    result = osalMsToSec(5000); // NOLINT
    REQUIRE(result == 5);

    result = osalMsToSec(4500); // NOLINT
    REQUIRE(result == 4);

    result = osalUsToSec(45); // NOLINT
    REQUIRE(result == 0);

    result = osalUsToSec(5000000UL); // NOLINT
    REQUIRE(result == 5);

    result = osalUsToSec(4500000UL); // NOLINT
    REQUIRE(result == 4);

    result = osalNsToSec(45); // NOLINT
    REQUIRE(result == 0);

    result = osalNsToSec(5000000000UL); // NOLINT
    REQUIRE(result == 5);

    result = osalNsToSec(4500000000UL); // NOLINT
    REQUIRE(result == 4);

    result = osalSecToMs(45); // NOLINT
    REQUIRE(result == 45000);

    result = osalSecToMs(5); // NOLINT
    REQUIRE(result == 5000);

    result = osalSecToMs(4); // NOLINT
    REQUIRE(result == 4000);

    result = osalUsToMs(45); // NOLINT
    REQUIRE(result == 0);

    result = osalUsToMs(5000); // NOLINT
    REQUIRE(result == 5);

    result = osalUsToMs(4500); // NOLINT
    REQUIRE(result == 4);

    result = osalNsToMs(45); // NOLINT
    REQUIRE(result == 0);

    result = osalNsToMs(5000000UL); // NOLINT
    REQUIRE(result == 5);

    result = osalNsToMs(4500000UL); // NOLINT
    REQUIRE(result == 4);

    result = osalSecToUs(1); // NOLINT
    REQUIRE(result == 1000000UL);

    result = osalSecToUs(2); // NOLINT
    REQUIRE(result == 2000000UL);

    result = osalSecToUs(4); // NOLINT
    REQUIRE(result == 4000000UL);

    result = osalMsToUs(1); // NOLINT
    REQUIRE(result == 1000);

    result = osalMsToUs(2); // NOLINT
    REQUIRE(result == 2000);

    result = osalMsToUs(4); // NOLINT
    REQUIRE(result == 4000);

    result = osalNsToUs(5); // NOLINT
    REQUIRE(result == 0);

    result = osalNsToUs(2000); // NOLINT
    REQUIRE(result == 2);

    result = osalNsToUs(4600); // NOLINT
    REQUIRE(result == 4);

    result = osalSecToNs(1); // NOLINT
    REQUIRE(result == 1000000000UL);

    result = osalSecToNs(2); // NOLINT
    REQUIRE(result == 2000000000UL);

    result = osalSecToNs(4); // NOLINT
    REQUIRE(result == 4000000000UL);

    result = osalMsToNs(1); // NOLINT
    REQUIRE(result == 1000000UL);

    result = osalMsToNs(2); // NOLINT
    REQUIRE(result == 2000000UL);

    result = osalMsToNs(4); // NOLINT
    REQUIRE(result == 4000000UL);

    result = osalUsToNs(1); // NOLINT
    REQUIRE(result == 1000);

    result = osalUsToNs(2); // NOLINT
    REQUIRE(result == 2000);

    result = osalUsToNs(4); // NOLINT
    REQUIRE(result == 4000);
}
