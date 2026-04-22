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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "osal/Error.h"

#include <stddef.h>
#include <sys/time.h>
#include <time.h>

/// Converts time_t value to struct tm.
/// @param value            Value to be converted.
/// @return struct tm created from time_t.
struct tm osalTimeToTm(time_t value);

/// Converts struct timespec value to struct tm.
/// @param value            Value to be converted.
/// @return struct tm created from struct timespec.
struct tm osalTimespecToTm(struct timespec value);

/// Converts struct timeval value to struct tm.
/// @param value            Value to be converted.
/// @return struct tm created from struct timeval.
struct tm osalTimevalToTm(struct timeval value);

/// Converts struct tm value to time_t.
/// @param value            Value to be converted.
/// @return time_t created from struct tm.
time_t osalTmToTime(struct tm value);

/// Converts struct timespec value to time_t.
/// @param value            Value to be converted.
/// @return time_t created from struct timespec.
time_t osalTimespecToTime(struct timespec value);

/// Converts struct timeval value to time_t.
/// @param value            Value to be converted.
/// @return time_t created from struct timeval.
time_t osalTimevalToTime(struct timeval value);

/// Converts time_t value to struct timespec.
/// @param value            Value to be converted.
/// @return struct timespec created from time_t.
struct timespec osalTimeToTimespec(time_t value);

/// Converts struct tm value to struct timespec.
/// @param value            Value to be converted.
/// @return struct timespec created from struct tm.
struct timespec osalTmToTimespec(struct tm value);

/// Converts struct timeval value to struct timespec.
/// @param value            Value to be converted.
/// @return struct timespec created from struct timeval.
struct timespec osalTimevalToTimespec(struct timeval value);

/// Converts time_t value to struct timeval.
/// @param value            Value to be converted.
/// @return struct timeval created from time_t.
struct timeval osalTimeToTimeval(time_t value);

/// Converts struct tm value to struct timeval.
/// @param value            Value to be converted.
/// @return struct timeval created from struct tm.
struct timeval osalTmToTimeval(struct tm value);

/// Converts struct timespec value to struct timeval.
/// @param value            Value to be converted.
/// @return struct timeval created from struct timespec.
struct timeval osalTimespecToTimeval(struct timespec value);

/// Represents possible string representations of date/time objects.
enum OsalTimeStringFormat {
    Time,          // "15:30:59"
    Date,          // "31.12.2022"
    TimeDate,      // "15:30:59 31.12.2022"
    SortedDateTime // "20221231_153059"
};

/// Converts struct tm to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param str              Output buffer where formatted string should be stored.
/// @param size             Size of the output buffer.
/// @param format           Date/time format to be used.
/// @return Error code of the operation.
OsalError osalTmToString(struct tm value, char* str, size_t size, OsalTimeStringFormat format);

/// Converts time_t to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param str              Output buffer where formatted string should be stored.
/// @param size             Size of the output buffer.
/// @param format           Date/time format to be used.
/// @return Error code of the operation.
OsalError osalTimeToString(time_t value, char* str, size_t size, OsalTimeStringFormat format);

/// Converts struct timespec to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param str              Output buffer where formatted string should be stored.
/// @param size             Size of the output buffer.
/// @param format           Date/time format to be used.
/// @return Error code of the operation.
OsalError osalTimespecToString(struct timespec value, char* str, size_t size, OsalTimeStringFormat format);

/// Converts struct timeval to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param str              Output buffer where formatted string should be stored.
/// @param size             Size of the output buffer.
/// @param format           Date/time format to be used.
/// @return Error code of the operation.
OsalError osalTimevalToString(struct timeval value, char* str, size_t size, OsalTimeStringFormat format);

#ifdef __cplusplus
}
#endif
