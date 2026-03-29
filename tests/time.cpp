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

#include <osal/time.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)
#include <osal/time.hpp>
#include <sys/time.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <ctime>
#include <string>

static constexpr int cDatesCount = 5;     // NOLINT
static constexpr int cDate1 = 1643407982; // NOLINT
static constexpr int cDate2 = 871571145;  // NOLINT
static constexpr int cDate3 = 1009832777; // NOLINT
static constexpr int cDate4 = 21902400;   // NOLINT
static constexpr int cDate5 = 650146800;  // NOLINT

TEST_CASE("Convert to struct tm", "[unit][c][time]")
{
    std::array<std::tm, cDatesCount> tms{};

    SECTION("time_t to struct tm")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        std::transform(times.begin(), times.end(), tms.begin(), osalTimeToTm);
    }

    SECTION("struct timespec to struct tm")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        std::transform(timespecs.begin(), timespecs.end(), tms.begin(), osalTimespecToTm);
    }

    SECTION("struct timeval to struct tm")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        std::transform(timevals.begin(), timevals.end(), tms.begin(), osalTimevalToTm);
    }

    CHECK(tms[0].tm_mday == 28);
    CHECK(tms[0].tm_mon == 0);
    CHECK(tms[0].tm_year == 122);
    CHECK(tms[0].tm_hour == 22);
    CHECK(tms[0].tm_min == 13);
    CHECK(tms[0].tm_sec == 2);
    CHECK(tms[0].tm_wday == 5);
    CHECK(tms[0].tm_yday == 27);

    CHECK(tms[1].tm_mday == 14);
    CHECK(tms[1].tm_mon == 7);
    CHECK(tms[1].tm_year == 97);
    CHECK(tms[1].tm_hour == 15);
    CHECK(tms[1].tm_min == 5);
    CHECK(tms[1].tm_sec == 45);
    CHECK(tms[1].tm_wday == 4);
    CHECK(tms[1].tm_yday == 225);

    CHECK(tms[2].tm_mday == 31);
    CHECK(tms[2].tm_mon == 11);
    CHECK(tms[2].tm_year == 101);
    CHECK(tms[2].tm_hour == 21);
    CHECK(tms[2].tm_min == 6);
    CHECK(tms[2].tm_sec == 17);
    CHECK(tms[2].tm_wday == 1);
    CHECK(tms[2].tm_yday == 364);

    CHECK(tms[3].tm_mday == 11);
    CHECK(tms[3].tm_mon == 8);
    CHECK(tms[3].tm_year == 70);
    CHECK(tms[3].tm_hour == 12);
    CHECK(tms[3].tm_min == 0);
    CHECK(tms[3].tm_sec == 0);
    CHECK(tms[3].tm_wday == 5);
    CHECK(tms[3].tm_yday == 253);

    CHECK(tms[4].tm_mday == 8);
    CHECK(tms[4].tm_mon == 7);
    CHECK(tms[4].tm_year == 90);
    CHECK(tms[4].tm_hour == 20);
    CHECK(tms[4].tm_min == 20);
    CHECK(tms[4].tm_sec == 0);
    CHECK(tms[4].tm_wday == 3);
    CHECK(tms[4].tm_yday == 219);
}

TEST_CASE("Convert to time_t", "[unit][c][time]")
{
    std::array<std::time_t, cDatesCount> times{};

    SECTION("struct tm to time_t")
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

    SECTION("struct timespec to time_t")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        std::transform(timespecs.begin(), timespecs.end(), times.begin(), osalTimespecToTime);
    }

    SECTION("struct timeval to time_t")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        std::transform(timevals.begin(), timevals.end(), times.begin(), osalTimevalToTime);
    }

    CHECK(times[0] == cDate1);
    CHECK(times[1] == cDate2);
    CHECK(times[2] == cDate3);
    CHECK(times[3] == cDate4);
    CHECK(times[4] == cDate5);
}

TEST_CASE("Convert to struct timespec", "[unit][c][time]")
{
    std::array<timespec, cDatesCount> timespecs{};

    SECTION("struct tm to struct timespec")
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

        std::transform(tms.begin(), tms.end(), timespecs.begin(), osalTmToTimespec);
    }

    SECTION("time_t to struct timespec")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        std::transform(times.begin(), times.end(), timespecs.begin(), osalTimeToTimespec);
    }

    SECTION("struct timeval to time_t")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        std::transform(timevals.begin(), timevals.end(), timespecs.begin(), osalTimevalToTimespec);
    }

    CHECK(timespecs[0].tv_sec == cDate1);
    CHECK(timespecs[0].tv_nsec == 0);
    CHECK(timespecs[1].tv_sec == cDate2);
    CHECK(timespecs[1].tv_nsec == 0);
    CHECK(timespecs[2].tv_sec == cDate3);
    CHECK(timespecs[2].tv_nsec == 0);
    CHECK(timespecs[3].tv_sec == cDate4);
    CHECK(timespecs[3].tv_nsec == 0);
    CHECK(timespecs[4].tv_sec == cDate5);
    CHECK(timespecs[4].tv_nsec == 0);
}

TEST_CASE("Convert to struct timeval", "[unit][c][time]")
{
    std::array<timeval, cDatesCount> timevals{};

    SECTION("struct tm to struct timeval")
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

        std::transform(tms.begin(), tms.end(), timevals.begin(), osalTmToTimeval);
    }

    SECTION("time_t to struct timeval")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        std::transform(times.begin(), times.end(), timevals.begin(), osalTimeToTimeval);
    }

    SECTION("struct timespec to struct timeval")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        std::transform(timespecs.begin(), timespecs.end(), timevals.begin(), osalTimespecToTimeval);
    }

    CHECK(timevals[0].tv_sec == cDate1);
    CHECK(timevals[0].tv_usec == 0);
    CHECK(timevals[1].tv_sec == cDate2);
    CHECK(timevals[1].tv_usec == 0);
    CHECK(timevals[2].tv_sec == cDate3);
    CHECK(timevals[2].tv_usec == 0);
    CHECK(timevals[3].tv_sec == cDate4);
    CHECK(timevals[3].tv_usec == 0);
    CHECK(timevals[4].tv_sec == cDate5);
    CHECK(timevals[4].tv_usec == 0);
}

TEST_CASE("Convert to string", "[unit][c][time]")
{
    constexpr std::size_t cSize = 20;
    std::array<std::array<char, cSize>, cDatesCount> time{};
    std::array<std::array<char, cSize>, cDatesCount> date{};
    std::array<std::array<char, cSize>, cDatesCount> timeDate{};
    std::array<std::array<char, cSize>, cDatesCount> sortedDateTime{};

    SECTION("struct tm to string")
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

        for (std::size_t i = 0; i < tms.size(); ++i) {
            auto error = osalTmToString(tms[i], time[i].data(), cSize, OsalTimeStringFormat::Time);
            CHECK_FALSE(error);

            error = osalTmToString(tms[i], date[i].data(), cSize, OsalTimeStringFormat::Date);
            CHECK_FALSE(error);

            error = osalTmToString(tms[i], timeDate[i].data(), cSize, OsalTimeStringFormat::TimeDate);
            CHECK_FALSE(error);

            error = osalTmToString(tms[i], sortedDateTime[i].data(), cSize, OsalTimeStringFormat::SortedDateTime);
            CHECK_FALSE(error);
        }
    }

    SECTION("time_t to string")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        for (std::size_t i = 0; i < times.size(); ++i) {
            auto error = osalTimeToString(times[i], time[i].data(), cSize, OsalTimeStringFormat::Time);
            CHECK_FALSE(error);

            error = osalTimeToString(times[i], date[i].data(), cSize, OsalTimeStringFormat::Date);
            CHECK_FALSE(error);

            error = osalTimeToString(times[i], timeDate[i].data(), cSize, OsalTimeStringFormat::TimeDate);
            CHECK_FALSE(error);

            error = osalTimeToString(times[i], sortedDateTime[i].data(), cSize, OsalTimeStringFormat::SortedDateTime);
            CHECK_FALSE(error);
        }
    }

    SECTION("struct timespec to string")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timespecs.size(); ++i) {
            auto error = osalTimespecToString(timespecs[i], time[i].data(), cSize, OsalTimeStringFormat::Time);
            CHECK_FALSE(error);

            error = osalTimespecToString(timespecs[i], date[i].data(), cSize, OsalTimeStringFormat::Date);
            CHECK_FALSE(error);

            error = osalTimespecToString(timespecs[i], timeDate[i].data(), cSize, OsalTimeStringFormat::TimeDate);
            CHECK_FALSE(error);

            error = osalTimespecToString(timespecs[i],
                                         sortedDateTime[i].data(),
                                         cSize,
                                         OsalTimeStringFormat::SortedDateTime);
            CHECK_FALSE(error);
        }
    }

    SECTION("struct timeval to string")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timevals.size(); ++i) {
            auto error = osalTimevalToString(timevals[i], time[i].data(), cSize, OsalTimeStringFormat::Time);
            CHECK_FALSE(error);

            error = osalTimevalToString(timevals[i], date[i].data(), cSize, OsalTimeStringFormat::Date);
            CHECK_FALSE(error);

            error = osalTimevalToString(timevals[i], timeDate[i].data(), cSize, OsalTimeStringFormat::TimeDate);
            CHECK_FALSE(error);

            error = osalTimevalToString(timevals[i],
                                        sortedDateTime[i].data(),
                                        cSize,
                                        OsalTimeStringFormat::SortedDateTime);
            CHECK_FALSE(error);
        }
    }

    CHECK_THAT(time[0].data(), Catch::Matchers::Equals("22:13:02"));
    CHECK_THAT(date[0].data(), Catch::Matchers::Equals("28.01.2022"));
    CHECK_THAT(timeDate[0].data(), Catch::Matchers::Equals("22:13:02 28.01.2022"));
    CHECK_THAT(sortedDateTime[0].data(), Catch::Matchers::Equals("20220128_221302"));

    CHECK_THAT(time[1].data(), Catch::Matchers::Equals("15:05:45"));
    CHECK_THAT(date[1].data(), Catch::Matchers::Equals("14.08.1997"));
    CHECK_THAT(timeDate[1].data(), Catch::Matchers::Equals("15:05:45 14.08.1997"));
    CHECK_THAT(sortedDateTime[1].data(), Catch::Matchers::Equals("19970814_150545"));

    CHECK_THAT(time[2].data(), Catch::Matchers::Equals("21:06:17"));
    CHECK_THAT(date[2].data(), Catch::Matchers::Equals("31.12.2001"));
    CHECK_THAT(timeDate[2].data(), Catch::Matchers::Equals("21:06:17 31.12.2001"));
    CHECK_THAT(sortedDateTime[2].data(), Catch::Matchers::Equals("20011231_210617"));

    CHECK_THAT(time[3].data(), Catch::Matchers::Equals("12:00:00"));
    CHECK_THAT(date[3].data(), Catch::Matchers::Equals("11.09.1970"));
    CHECK_THAT(timeDate[3].data(), Catch::Matchers::Equals("12:00:00 11.09.1970"));
    CHECK_THAT(sortedDateTime[3].data(), Catch::Matchers::Equals("19700911_120000"));

    CHECK_THAT(time[4].data(), Catch::Matchers::Equals("20:20:00"));
    CHECK_THAT(date[4].data(), Catch::Matchers::Equals("08.08.1990"));
    CHECK_THAT(timeDate[4].data(), Catch::Matchers::Equals("20:20:00 08.08.1990"));
    CHECK_THAT(sortedDateTime[4].data(), Catch::Matchers::Equals("19900808_202000"));
}

TEST_CASE("Convert to std::tm in C++", "[unit][cpp][time]")
{
    std::array<std::tm, cDatesCount> tms{};

    SECTION("std::time_t to std::tm")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        for (std::size_t i = 0; i < times.size(); ++i)
            tms[i] = osal::toTm(times[i]);
    }

    SECTION("struct timespec to struct tm")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timespecs.size(); ++i)
            tms[i] = osal::toTm(timespecs[i]);
    }

    SECTION("struct timeval to struct tm")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timevals.size(); ++i)
            tms[i] = osal::toTm(timevals[i]);
    }

    CHECK(tms[0].tm_mday == 28);
    CHECK(tms[0].tm_mon == 0);
    CHECK(tms[0].tm_year == 122);
    CHECK(tms[0].tm_hour == 22);
    CHECK(tms[0].tm_min == 13);
    CHECK(tms[0].tm_sec == 2);
    CHECK(tms[0].tm_wday == 5);
    CHECK(tms[0].tm_yday == 27);

    CHECK(tms[1].tm_mday == 14);
    CHECK(tms[1].tm_mon == 7);
    CHECK(tms[1].tm_year == 97);
    CHECK(tms[1].tm_hour == 15);
    CHECK(tms[1].tm_min == 5);
    CHECK(tms[1].tm_sec == 45);
    CHECK(tms[1].tm_wday == 4);
    CHECK(tms[1].tm_yday == 225);

    CHECK(tms[2].tm_mday == 31);
    CHECK(tms[2].tm_mon == 11);
    CHECK(tms[2].tm_year == 101);
    CHECK(tms[2].tm_hour == 21);
    CHECK(tms[2].tm_min == 6);
    CHECK(tms[2].tm_sec == 17);
    CHECK(tms[2].tm_wday == 1);
    CHECK(tms[2].tm_yday == 364);

    CHECK(tms[3].tm_mday == 11);
    CHECK(tms[3].tm_mon == 8);
    CHECK(tms[3].tm_year == 70);
    CHECK(tms[3].tm_hour == 12);
    CHECK(tms[3].tm_min == 0);
    CHECK(tms[3].tm_sec == 0);
    CHECK(tms[3].tm_wday == 5);
    CHECK(tms[3].tm_yday == 253);

    CHECK(tms[4].tm_mday == 8);
    CHECK(tms[4].tm_mon == 7);
    CHECK(tms[4].tm_year == 90);
    CHECK(tms[4].tm_hour == 20);
    CHECK(tms[4].tm_min == 20);
    CHECK(tms[4].tm_sec == 0);
    CHECK(tms[4].tm_wday == 3);
    CHECK(tms[4].tm_yday == 219);
}

TEST_CASE("Convert to std::time_t in C++", "[unit][cpp][time]")
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

        for (std::size_t i = 0; i < tms.size(); ++i)
            times[i] = osal::toTime(tms[i]);
    }

    SECTION("std::timespec to std::time_t")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timespecs.size(); ++i)
            times[i] = osal::toTime(timespecs[i]);
    }

    SECTION("std::timeval to std::time_t")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timevals.size(); ++i)
            times[i] = osal::toTime(timevals[i]);
    }

    CHECK(times[0] == cDate1);
    CHECK(times[1] == cDate2);
    CHECK(times[2] == cDate3);
    CHECK(times[3] == cDate4);
    CHECK(times[4] == cDate5);
}

TEST_CASE("Convert to std::timespec in C++", "[unit][cpp][time]")
{
    std::array<timespec, cDatesCount> timespecs{};

    SECTION("std::tm to std::timespec")
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

        for (std::size_t i = 0; i < tms.size(); ++i)
            timespecs[i] = osal::toTimespec(tms[i]);
    }

    SECTION("std::time_t to std::timespec")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        for (std::size_t i = 0; i < times.size(); ++i)
            timespecs[i] = osal::toTimespec(times[i]);
    }

    SECTION("std::timeval to std::time_t")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timevals.size(); ++i)
            timespecs[i] = osal::toTimespec(timevals[i]);
    }

    CHECK(timespecs[0].tv_sec == cDate1);
    CHECK(timespecs[0].tv_nsec == 0);
    CHECK(timespecs[1].tv_sec == cDate2);
    CHECK(timespecs[1].tv_nsec == 0);
    CHECK(timespecs[2].tv_sec == cDate3);
    CHECK(timespecs[2].tv_nsec == 0);
    CHECK(timespecs[3].tv_sec == cDate4);
    CHECK(timespecs[3].tv_nsec == 0);
    CHECK(timespecs[4].tv_sec == cDate5);
    CHECK(timespecs[4].tv_nsec == 0);
}

TEST_CASE("Convert to std::timeval in C++", "[unit][cpp][time]")
{
    std::array<timeval, cDatesCount> timevals{};

    SECTION("std::tm to std::timeval")
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

        for (std::size_t i = 0; i < tms.size(); ++i)
            timevals[i] = osal::toTimeval(tms[i]);
    }

    SECTION("std::time_t to std::timeval")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        for (std::size_t i = 0; i < times.size(); ++i)
            timevals[i] = osal::toTimeval(times[i]);
    }

    SECTION("std::timespec to std::timeval")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timespecs.size(); ++i)
            timevals[i] = osal::toTimeval(timespecs[i]);
    }

    CHECK(timevals[0].tv_sec == cDate1);
    CHECK(timevals[0].tv_usec == 0);
    CHECK(timevals[1].tv_sec == cDate2);
    CHECK(timevals[1].tv_usec == 0);
    CHECK(timevals[2].tv_sec == cDate3);
    CHECK(timevals[2].tv_usec == 0);
    CHECK(timevals[3].tv_sec == cDate4);
    CHECK(timevals[3].tv_usec == 0);
    CHECK(timevals[4].tv_sec == cDate5);
    CHECK(timevals[4].tv_usec == 0);
}

TEST_CASE("Convert to std::string in C++", "[unit][cpp][time]")
{
    std::array<std::string, cDatesCount> time{};
    std::array<std::string, cDatesCount> date{};
    std::array<std::string, cDatesCount> timeDate{};
    std::array<std::string, cDatesCount> sortedDateTime{};

    SECTION("std::tm to std::string")
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

        for (std::size_t i = 0; i < tms.size(); ++i) {
            time[i] = osal::toString(tms[i], OsalTimeStringFormat::Time);
            date[i] = osal::toString(tms[i], OsalTimeStringFormat::Date);
            timeDate[i] = osal::toString(tms[i], OsalTimeStringFormat::TimeDate);
            sortedDateTime[i] = osal::toString(tms[i], OsalTimeStringFormat::SortedDateTime);
        }
    }

    SECTION("time_t to string")
    {
        std::array<std::time_t, cDatesCount> times = {cDate1, cDate2, cDate3, cDate4, cDate5};

        for (std::size_t i = 0; i < times.size(); ++i) {
            time[i] = osal::toString(times[i], OsalTimeStringFormat::Time);
            date[i] = osal::toString(times[i], OsalTimeStringFormat::Date);
            timeDate[i] = osal::toString(times[i], OsalTimeStringFormat::TimeDate);
            sortedDateTime[i] = osal::toString(times[i], OsalTimeStringFormat::SortedDateTime);
        }
    }

    SECTION("struct timespec to string")
    {
        std::array<timespec, cDatesCount> timespecs = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timespecs.size(); ++i) {
            time[i] = osal::toString(timespecs[i], OsalTimeStringFormat::Time);
            date[i] = osal::toString(timespecs[i], OsalTimeStringFormat::Date);
            timeDate[i] = osal::toString(timespecs[i], OsalTimeStringFormat::TimeDate);
            sortedDateTime[i] = osal::toString(timespecs[i], OsalTimeStringFormat::SortedDateTime);
        }
    }

    SECTION("struct timeval to string")
    {
        std::array<timeval, cDatesCount> timevals = {
            {{cDate1, 0}, {cDate2, 0}, {cDate3, 0}, {cDate4, 0}, {cDate5, 0}}
        };

        for (std::size_t i = 0; i < timevals.size(); ++i) {
            time[i] = osal::toString(timevals[i], OsalTimeStringFormat::Time);
            date[i] = osal::toString(timevals[i], OsalTimeStringFormat::Date);
            timeDate[i] = osal::toString(timevals[i], OsalTimeStringFormat::TimeDate);
            sortedDateTime[i] = osal::toString(timevals[i], OsalTimeStringFormat::SortedDateTime);
        }
    }

    CHECK(time[0] == "22:13:02");
    CHECK(date[0] == "28.01.2022");
    CHECK(timeDate[0] == "22:13:02 28.01.2022");
    CHECK(sortedDateTime[0] == "20220128_221302");

    CHECK(time[1] == "15:05:45");
    CHECK(date[1] == "14.08.1997");
    CHECK(timeDate[1] == "15:05:45 14.08.1997");
    CHECK(sortedDateTime[1] == "19970814_150545");

    CHECK(time[2] == "21:06:17");
    CHECK(date[2] == "31.12.2001");
    CHECK(timeDate[2] == "21:06:17 31.12.2001");
    CHECK(sortedDateTime[2] == "20011231_210617");

    CHECK(time[3] == "12:00:00");
    CHECK(date[3] == "11.09.1970");
    CHECK(timeDate[3] == "12:00:00 11.09.1970");
    CHECK(sortedDateTime[3] == "19700911_120000");

    CHECK(time[4] == "20:20:00");
    CHECK(date[4] == "08.08.1990");
    CHECK(timeDate[4] == "20:20:00 08.08.1990");
    CHECK(sortedDateTime[4] == "19900808_202000");
}
