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

#include <osal/sleep.hpp>
#include <osal/timeout.hpp>

#include <catch2/catch.hpp>

#include <algorithm>

TEST_CASE("Creation of timeout", "[cpp][unit][timeout]")
{
    osal::Timeout t1(3s);
    osal::Timeout t2(4ms);
    osal::Timeout t3(1us);
    osal::Timeout t4(t2);
    osal::Timeout t5 = t3;

    constexpr auto cDelay = 5s;
    osal::sleep(cDelay);

    REQUIRE(t1.isExpired());
    REQUIRE(t1.timeLeft() == osal::Duration::zero());
    REQUIRE(t2.isExpired());
    REQUIRE(t2.timeLeft() == osal::Duration::zero());
    REQUIRE(t3.isExpired());
    REQUIRE(t3.timeLeft() == osal::Duration::zero());
    REQUIRE(t4.isExpired());
    REQUIRE(t4.timeLeft() == osal::Duration::zero());
    REQUIRE(t5.isExpired());
    REQUIRE(t5.timeLeft() == osal::Duration::zero());
}

TEST_CASE("Timeout is monotonic", "[cpp][unit][timeout]")
{
    constexpr auto cTimeout = 5s;
    osal::Timeout t(cTimeout);

    std::vector<std::uint64_t> timesLeft;
    while (!t.isExpired()) {
        constexpr auto cDelay = 100ms;
        osal::sleep(cDelay);
        timesLeft.push_back(t.timeLeft().count());
    }

    REQUIRE(t.timeLeft().count() == 0);
    REQUIRE(std::is_sorted(std::rbegin(timesLeft), std::rend(timesLeft)));
}
