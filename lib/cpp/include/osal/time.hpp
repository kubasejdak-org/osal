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

#include "osal/time.h" // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

#include <sys/time.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

#include <ctime>
#include <string>

namespace osal {

/// Converts std::time_t value to std::tm.
/// @param value            Value to be converted.
/// @return std::tm created from std::time_t.
std::tm toTm(std::time_t value);

/// Converts std::timespec value to std::tm.
/// @param value            Value to be converted.
/// @return std::tm created from std::timespec.
std::tm toTm(timespec value);

/// Converts std::timeval value to std::tm.
/// @param value            Value to be converted.
/// @return std::tm created from std::timeval.
std::tm toTm(timeval value);

/// Converts std::tm value to std::time_t.
/// @param value            Value to be converted.
/// @return std::time_t created from std::tm.
std::time_t toTime(std::tm value);

/// Converts std::timespec value to std::time_t.
/// @param value            Value to be converted.
/// @return std::time_t created from std::timespec.
std::time_t toTime(timespec value);

/// Converts std::timeval value to std::time_t.
/// @param value            Value to be converted.
/// @return std::time_t created from std::timeval.
std::time_t toTime(timeval value);

/// Converts std::time_t value to std::timespec.
/// @param value            Value to be converted.
/// @return std::timespec created from std::time_t.
timespec toTimespec(std::time_t value);

/// Converts std::tm value to std::timespec.
/// @param value            Value to be converted.
/// @return std::timespec created from std::tm.
timespec toTimespec(std::tm value);

/// Converts std::timeval value to std::timespec.
/// @param value            Value to be converted.
/// @return std::timespec created from std::timeval.
timespec toTimespec(timeval value);

/// Converts std::time_t value to std::timeval.
/// @param value            Value to be converted.
/// @return std::timeval created from std::time_t.
timeval toTimeval(std::time_t value);

/// Converts std::tm value to std::timeval.
/// @param value            Value to be converted.
/// @return std::timeval created from std::tm.
timeval toTimeval(std::tm value);

/// Converts std::timespec value to std::timeval.
/// @param value            Value to be converted.
/// @return std::timeval created from std::timespec.
timeval toTimeval(timespec value);

/// Converts std::tm to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param format           Date/time format to be used.
/// @return String representation of std::tm.
std::string toString(std::tm value, OsalTimeStringFormat format = OsalTimeStringFormat::TimeDate);

/// Converts std::time_t to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param format           Date/time format to be used.
/// @return String representation of std::time_t.
std::string toString(std::time_t value, OsalTimeStringFormat format = OsalTimeStringFormat::TimeDate);

/// Converts std::timespec to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param format           Date/time format to be used.
/// @return String representation of std::timespec.
std::string toString(timespec value, OsalTimeStringFormat format = OsalTimeStringFormat::TimeDate);

/// Converts std::timeval to a string representation using given string format.
/// @param value            Value to be converted to string.
/// @param format           Date/time format to be used.
/// @return String representation of std::timeval.
std::string toString(timeval value, OsalTimeStringFormat format = OsalTimeStringFormat::TimeDate);

} // namespace osal
