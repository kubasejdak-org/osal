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

#include "osal/time.h" // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

#include "osal/Error.h"

#include <ctime>
#include <string>

// This declaration is needed, because embedded platforms do not define timegm() function. Platform fills this gap
// by providing its own implementation, but still <ctime> header doesn't provide this prototype.
extern "C" time_t timegm(struct tm* tm); // NOLINT

struct tm osalTimeToTm(time_t value)
{
    tm result{};
    gmtime_r(&value, &result);
    return result;
}

struct tm osalTimespecToTm(struct timespec value)
{
    return osalTimeToTm(osalTimespecToTime(value));
}

struct tm osalTimevalToTm(struct timeval value)
{
    return osalTimeToTm(osalTimevalToTime(value));
}

time_t osalTmToTime(struct tm value)
{
    return timegm(&value);
}

time_t osalTimespecToTime(struct timespec value)
{
    return value.tv_sec;
}

time_t osalTimevalToTime(struct timeval value)
{
    return value.tv_sec;
}

struct timespec osalTimeToTimespec(time_t value)
{
    return {value, 0};
}

struct timespec osalTmToTimespec(struct tm value)
{
    return {osalTmToTime(value), 0};
}

struct timespec osalTimevalToTimespec(struct timeval value)
{
    return {osalTimevalToTime(value), 0};
}

struct timeval osalTimeToTimeval(time_t value)
{
    return {value, 0};
}

struct timeval osalTmToTimeval(struct tm value)
{
    return {osalTmToTime(value), 0};
}

struct timeval osalTimespecToTimeval(struct timespec value)
{
    return {osalTimespecToTime(value), 0};
}

OsalError osalTmToString(struct tm value, char* str, size_t size, OsalTimeStringFormat format)
{
    std::string formatStr;
    std::size_t requiredSize{};

    switch (format) {
        case OsalTimeStringFormat::eTime:
            formatStr = "%T";
            requiredSize = 9; // NOLINT
            break;
        case OsalTimeStringFormat::eDate:
            formatStr = "%d.%m.%Y";
            requiredSize = 11; // NOLINT
            break;
        case OsalTimeStringFormat::eTimeDate:
            formatStr = "%T %d.%m.%Y";
            requiredSize = 20; // NOLINT
            break;
        case OsalTimeStringFormat::eSortedDateTime:
            formatStr = "%Y%m%d_%H%M%S";
            requiredSize = 16; // NOLINT
            break;
    }

    if (size < requiredSize)
        return OsalError::eInvalidArgument;

    (void) std::strftime(str, requiredSize, formatStr.c_str(), &value);
    return {};
}

OsalError osalTimeToString(time_t value, char* str, size_t size, OsalTimeStringFormat format)
{
    return osalTmToString(osalTimeToTm(value), str, size, format);
}

OsalError osalTimespecToString(struct timespec value, char* str, size_t size, OsalTimeStringFormat format)
{
    return osalTmToString(osalTimespecToTm(value), str, size, format);
}

OsalError osalTimevalToString(struct timeval value, char* str, size_t size, OsalTimeStringFormat format)
{
    return osalTmToString(osalTimevalToTm(value), str, size, format);
}
