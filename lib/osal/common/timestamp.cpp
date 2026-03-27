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

#include "osal/timestamp.h"

#include <cstdint>

/// @enum ConversionsToSec
/// Represents the time conversion helpers to seconds.
enum ConversionsToSec {
    OsalMsInSec = 1000,
    OsalUsInSec = 1000000,
    OsalNsInSec = 1000000000
};

/// @enum ConversionsToMs
/// Represents the time conversion helpers to milliseconds.
enum ConversionsToMs {
    OsalUsInMs = 1000,
    OsalNsInMs = 1000000
};

/// @enum ConversionsToUs
/// Represents the time conversion helpers to microseconds.
enum ConversionsToUs {
    OsalNsInUs = 1000
};

uint64_t osalMsToSec(uint64_t milliseconds)
{
    return milliseconds / OsalMsInSec;
}

uint64_t osalUsToSec(uint64_t microseconds)
{
    return microseconds / OsalUsInSec;
}

uint64_t osalNsToSec(uint64_t nanoseconds)
{
    return nanoseconds / OsalNsInSec;
}

uint64_t osalSecToMs(uint64_t seconds)
{
    return seconds * OsalMsInSec;
}

uint64_t osalUsToMs(uint64_t microseconds)
{
    return microseconds / OsalUsInMs;
}

uint64_t osalNsToMs(uint64_t nanoseconds)
{
    return nanoseconds / OsalNsInMs;
}

uint64_t osalSecToUs(uint64_t seconds)
{
    return seconds * OsalUsInSec;
}

uint64_t osalMsToUs(uint64_t milliseconds)
{
    return milliseconds * OsalUsInMs;
}

uint64_t osalNsToUs(uint64_t nanoseconds)
{
    return nanoseconds / OsalNsInUs;
}

uint64_t osalSecToNs(uint64_t seconds)
{
    return seconds * OsalNsInSec;
}

uint64_t osalMsToNs(uint64_t milliseconds)
{
    return milliseconds * OsalNsInMs;
}

uint64_t osalUsToNs(uint64_t microseconds)
{
    return microseconds * OsalNsInUs;
}
