/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright MIT License
///
/// Copyright (c) 2022 Kuba Sejdak (kuba.sejdak@gmail.com)
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

#include "osal/time.hpp"

#include "osal/time.h"

#include <array>
#include <cstddef>
#include <ctime>
#include <string>

namespace osal {

std::tm toTm(std::time_t value)
{
    return osalTimeToTm(value);
}

std::tm toTm(timespec value)
{
    return osalTimespecToTm(value);
}

std::tm toTm(timeval value)
{
    return osalTimevalToTm(value);
}

std::time_t toTime(std::tm value)
{
    return osalTmToTime(value);
}

std::time_t toTime(timespec value)
{
    return osalTimespecToTime(value);
}

std::time_t toTime(timeval value)
{
    return osalTimevalToTime(value);
}

timespec toTimespec(std::time_t value)
{
    return osalTimeToTimespec(value);
}

timespec toTimespec(std::tm value)
{
    return osalTmToTimespec(value);
}

timespec toTimespec(timeval value)
{
    return osalTimevalToTimespec(value);
}

timeval toTimeval(std::time_t value)
{
    return osalTimeToTimeval(value);
}

timeval toTimeval(std::tm value)
{
    return osalTmToTimeval(value);
}

timeval toTimeval(timespec value)
{
    return osalTimespecToTimeval(value);
}

static constexpr std::size_t cMaxStringSize = 20;

std::string toString(std::tm value, OsalTimeStringFormat format)
{
    std::array<char, cMaxStringSize> str{};
    osalTmToString(value, str.data(), cMaxStringSize, format);
    return str.data();
}

std::string toString(std::time_t value, OsalTimeStringFormat format)
{
    std::array<char, cMaxStringSize> str{};
    osalTimeToString(value, str.data(), cMaxStringSize, format);
    return str.data();
}

std::string toString(timespec value, OsalTimeStringFormat format)
{
    std::array<char, cMaxStringSize> str{};
    osalTimespecToString(value, str.data(), cMaxStringSize, format);
    return str.data();
}

std::string toString(timeval value, OsalTimeStringFormat format)
{
    std::array<char, cMaxStringSize> str{};
    osalTimevalToString(value, str.data(), cMaxStringSize, format);
    return str.data();
}

} // namespace osal
