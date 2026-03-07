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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

/// Returns the timestamp relative to the call to osalInit() function in ms.
/// @return Timestamp relative to the call to osalInit() function in ms.
/// @note osalInit() has to be called in order to have correct values returned by this function.
uint64_t osalTimestampMs();

/// Returns the timestamp relative to the call to osalInit() function in us.
/// @return Timestamp relative to the call to osalInit() function in us.
/// @note osalInit() has to be called in order to have correct values returned by this function.
uint64_t osalTimestampUs();

/// Returns the timestamp relative to the call to osalInit() function in ns.
/// @return Timestamp relative to the call to osalInit() function in ns.
/// @note osalInit() has to be called in order to have correct values returned by this function.
uint64_t osalTimestampNs();

/// Converts milliseconds to seconds.
/// @param milliseconds         Milliseconds to be converted.
/// @return Milliseconds expressed in a number of seconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalMsToSec(uint64_t milliseconds);

/// Converts microseconds to seconds.
/// @param microseconds         Microseconds to be converted.
/// @return Microseconds expressed in a number of seconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalUsToSec(uint64_t microseconds);

/// Converts nanoseconds to seconds.
/// @param nanoseconds          Nanoseconds to be converted.
/// @return Nanoseconds expressed in a number of seconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalNsToSec(uint64_t nanoseconds);

/// Converts seconds to milliseconds.
/// @param seconds              Seconds to be converted.
/// @return Seconds expressed in a number of milliseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalSecToMs(uint64_t seconds);

/// Converts microseconds to milliseconds.
/// @param microseconds         Microseconds to be converted.
/// @return Microseconds expressed in a number of milliseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalUsToMs(uint64_t microseconds);

/// Converts nanoseconds to milliseconds.
/// @param nanoseconds          Nanoseconds to be converted.
/// @return Nanoseconds expressed in a number of milliseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalNsToMs(uint64_t nanoseconds);

/// Converts seconds to microseconds.
/// @param seconds              Seconds to be converted.
/// @return Seconds expressed in a number of microseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalSecToUs(uint64_t seconds);

/// Converts milliseconds to microseconds.
/// @param milliseconds         Milliseconds to be converted.
/// @return Milliseconds expressed in a number of microseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalMsToUs(uint64_t milliseconds);

/// Converts nanoseconds to microseconds.
/// @param nanoseconds          Nanoseconds to be converted.
/// @return Nanoseconds expressed in a number of microseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalNsToUs(uint64_t nanoseconds);

/// Converts seconds to nanoseconds.
/// @param seconds              Seconds to be converted.
/// @return Seconds expressed in a number of nanoseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalSecToNs(uint64_t seconds);

/// Converts milliseconds to nanoseconds.
/// @param milliseconds         Milliseconds to be converted.
/// @return Milliseconds expressed in a number of nanoseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalMsToNs(uint64_t milliseconds);

/// Converts microseconds to nanoseconds.
/// @param microseconds         Microseconds to be converted.
/// @return Microseconds expressed in a number of nanoseconds.
/// @note This function doesn't check for overflows or conversion loses.
uint64_t osalUsToNs(uint64_t microseconds);

#ifdef __cplusplus
}
#endif
