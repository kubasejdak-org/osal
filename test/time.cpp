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

TEST_CASE("Convert to std::tm", "[unit][c][time]")
{
    std::array<std::tm, cDatesCount> tms{};

    SECTION("std::time_t to std::tm")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        std::transform(times.begin(), times.end(), tms.begin(), osalTimeToTm);
    }

    SECTION("std::timespec to std::tm")
    {
        std::array<timespec, cDatesCount> timespecs
            = {{{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}};

        std::transform(timespecs.begin(), timespecs.end(), tms.begin(), osalTimespecToTm);
    }

    SECTION("std::timeval to std::tm")
    {
        std::array<timeval, cDatesCount> timevals = {{{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}};

        std::transform(timevals.begin(), timevals.end(), tms.begin(), osalTimevalToTm);
    }

    REQUIRE(tms[0].tm_mday == 28);
    REQUIRE(tms[0].tm_mon == 0);
    REQUIRE(tms[0].tm_year == 122);
    REQUIRE(tms[0].tm_hour == 22);
    REQUIRE(tms[0].tm_min == 13);
    REQUIRE(tms[0].tm_sec == 2);
    REQUIRE(tms[0].tm_wday == 5);
    REQUIRE(tms[0].tm_yday == 27);

    REQUIRE(tms[1].tm_mday == 14);
    REQUIRE(tms[1].tm_mon == 7);
    REQUIRE(tms[1].tm_year == 97);
    REQUIRE(tms[1].tm_hour == 15);
    REQUIRE(tms[1].tm_min == 5);
    REQUIRE(tms[1].tm_sec == 45);
    REQUIRE(tms[1].tm_wday == 4);
    REQUIRE(tms[1].tm_yday == 225);

    REQUIRE(tms[2].tm_mday == 31);
    REQUIRE(tms[2].tm_mon == 11);
    REQUIRE(tms[2].tm_year == 101);
    REQUIRE(tms[2].tm_hour == 21);
    REQUIRE(tms[2].tm_min == 6);
    REQUIRE(tms[2].tm_sec == 17);
    REQUIRE(tms[2].tm_wday == 1);
    REQUIRE(tms[2].tm_yday == 364);

    REQUIRE(tms[3].tm_mday == 11);
    REQUIRE(tms[3].tm_mon == 8);
    REQUIRE(tms[3].tm_year == 70);
    REQUIRE(tms[3].tm_hour == 12);
    REQUIRE(tms[3].tm_min == 0);
    REQUIRE(tms[3].tm_sec == 0);
    REQUIRE(tms[3].tm_wday == 5);
    REQUIRE(tms[3].tm_yday == 253);

    REQUIRE(tms[4].tm_mday == 8);
    REQUIRE(tms[4].tm_mon == 7);
    REQUIRE(tms[4].tm_year == 90);
    REQUIRE(tms[4].tm_hour == 20);
    REQUIRE(tms[4].tm_min == 20);
    REQUIRE(tms[4].tm_sec == 0);
    REQUIRE(tms[4].tm_wday == 3);
    REQUIRE(tms[4].tm_yday == 219);
}

TEST_CASE("Convert to std::time_t", "[unit][c][time]")
{
    std::array<std::time_t, cDatesCount> times{};

    SECTION("std::tm to std::time_t")
    {
        std::array<std::tm, cDatesCount> tms{};
        tms[0].tm_mday = 28;  // NOLINT
        tms[0].tm_mon = 0;    // NOLINT
        tms[0].tm_year = 122; // NOLINT
        tms[0].tm_hour = 22;  // NOLINT
        tms[0].tm_min = 13;   // NOLINT
        tms[0].tm_sec = 2;    // NOLINT
        tms[0].tm_wday = 5;   // NOLINT
        tms[0].tm_yday = 27;  // NOLINT

        tms[1].tm_mday = 14;  // NOLINT
        tms[1].tm_mon = 7;    // NOLINT
        tms[1].tm_year = 97;  // NOLINT
        tms[1].tm_hour = 15;  // NOLINT
        tms[1].tm_min = 5;    // NOLINT
        tms[1].tm_sec = 45;   // NOLINT
        tms[1].tm_wday = 4;   // NOLINT
        tms[1].tm_yday = 225; // NOLINT

        tms[2].tm_mday = 31;  // NOLINT
        tms[2].tm_mon = 11;   // NOLINT
        tms[2].tm_year = 101; // NOLINT
        tms[2].tm_hour = 21;  // NOLINT
        tms[2].tm_min = 6;    // NOLINT
        tms[2].tm_sec = 17;   // NOLINT
        tms[2].tm_wday = 1;   // NOLINT
        tms[2].tm_yday = 364; // NOLINT

        tms[3].tm_mday = 11;  // NOLINT
        tms[3].tm_mon = 8;    // NOLINT
        tms[3].tm_year = 70;  // NOLINT
        tms[3].tm_hour = 12;  // NOLINT
        tms[3].tm_min = 0;    // NOLINT
        tms[3].tm_sec = 0;    // NOLINT
        tms[3].tm_wday = 5;   // NOLINT
        tms[3].tm_yday = 253; // NOLINT

        tms[4].tm_mday = 8;   // NOLINT
        tms[4].tm_mon = 7;    // NOLINT
        tms[4].tm_year = 90;  // NOLINT
        tms[4].tm_hour = 20;  // NOLINT
        tms[4].tm_min = 20;   // NOLINT
        tms[4].tm_sec = 0;    // NOLINT
        tms[4].tm_wday = 3;   // NOLINT
        tms[4].tm_yday = 219; // NOLINT

        std::transform(tms.begin(), tms.end(), times.begin(), osalTmToTime);
    }

    SECTION("std::timespec to std::time_t")
    {
        std::array<timespec, cDatesCount> timespecs
            = {{{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}};

        std::transform(timespecs.begin(), timespecs.end(), times.begin(), osalTimespecToTime);
    }

    SECTION("std::timeval to std::time_t")
    {
        std::array<timespec, cDatesCount> timespecs
            = {{{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}};

        std::transform(timespecs.begin(), timespecs.end(), times.begin(), osalTimespecToTime);
    }

    REQUIRE(times[0] == cDate1);
    REQUIRE(times[1] == cDate2);
    REQUIRE(times[2] == cDate3);
    REQUIRE(times[3] == cDate4);
    REQUIRE(times[4] == cDate5);
}
