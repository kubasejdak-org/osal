/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2020-2022, Kuba Sejdak <kuba.sejdak@gmail.com>
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

#include <osal/time.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)
#include <osal/time.hpp>

#include <catch2/catch.hpp>

#include <algorithm>
#include <array>

static constexpr int cDatesCount = 5;     // NOLINT
static constexpr int cDate1 = 1643407982; // NOLINT
static constexpr int cDate2 = 871571145;  // NOLINT
static constexpr int cDate3 = 1009832777; // NOLINT
static constexpr int cDate4 = 21902400;   // NOLINT
static constexpr int cDate5 = 650146800;  // NOLINT

TEST_CASE("Convert std::time_t to std::tm", "[unit][c][time]")
{
    std::array<std::tm, cDatesCount> tms{};

    SECTION("std::time_t to std::tm")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        std::transform(times.begin(), times.end(), tms.begin(), osalTimeToTm);
    }

    SECTION("std::timespec to std::tm")
    {
        std::array<std::timespec, cDatesCount> times{{{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}};

        std::transform(times.begin(), times.end(), tms.begin(), osalTimespecToTm);
    }

    SECTION("std::timeval to std::tm")
    {
        std::array<timeval, cDatesCount> times{{{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}};

        std::transform(times.begin(), times.end(), tms.begin(), osalTimevalToTm);
    }

    REQUIRE(tms[0].tm_mday == 28);
    REQUIRE(tms[0].tm_mon == 0);
    REQUIRE(tms[0].tm_year == 122);
    REQUIRE(tms[0].tm_hour == 23);
    REQUIRE(tms[0].tm_min == 13);
    REQUIRE(tms[0].tm_sec == 2);
    REQUIRE(tms[0].tm_wday == 5);
    REQUIRE(tms[0].tm_yday == 27);

    REQUIRE(tms[1].tm_mday == 14);
    REQUIRE(tms[1].tm_mon == 7);
    REQUIRE(tms[1].tm_year == 97);
    REQUIRE(tms[1].tm_hour == 17);
    REQUIRE(tms[1].tm_min == 5);
    REQUIRE(tms[1].tm_sec == 45);
    REQUIRE(tms[1].tm_wday == 4);
    REQUIRE(tms[1].tm_yday == 225);

    REQUIRE(tms[2].tm_mday == 31);
    REQUIRE(tms[2].tm_mon == 11);
    REQUIRE(tms[2].tm_year == 101);
    REQUIRE(tms[2].tm_hour == 22);
    REQUIRE(tms[2].tm_min == 6);
    REQUIRE(tms[2].tm_sec == 17);
    REQUIRE(tms[2].tm_wday == 1);
    REQUIRE(tms[2].tm_yday == 364);

    REQUIRE(tms[3].tm_mday == 11);
    REQUIRE(tms[3].tm_mon == 8);
    REQUIRE(tms[3].tm_year == 70);
    REQUIRE(tms[3].tm_hour == 13);
    REQUIRE(tms[3].tm_min == 0);
    REQUIRE(tms[3].tm_sec == 0);
    REQUIRE(tms[3].tm_wday == 5);
    REQUIRE(tms[3].tm_yday == 253);

    REQUIRE(tms[4].tm_mday == 8);
    REQUIRE(tms[4].tm_mon == 7);
    REQUIRE(tms[4].tm_year == 90);
    REQUIRE(tms[4].tm_hour == 22);
    REQUIRE(tms[4].tm_min == 20);
    REQUIRE(tms[4].tm_sec == 0);
    REQUIRE(tms[4].tm_wday == 3);
    REQUIRE(tms[4].tm_yday == 219);
}
