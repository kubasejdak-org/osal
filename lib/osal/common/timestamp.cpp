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

/// @num ConversionsToSec
/// Represents the time conversion helpers to seconds.
enum ConversionsToSec {
    eOsalMsInSec = 1000,
    eOsalUsInSec = 1000000,
    eOsalNsInSec = 1000000000
};

/// @num ConversionsToMs
/// Represents the time conversion helpers to milliseconds.
enum ConversionsToMs {
    eOsalUsInMs = 1000,
    eOsalNsInMs = 1000000
};

/// @num ConversionsToUs
/// Represents the time conversion helpers to microseconds.
enum ConversionsToUs {
    eOsalNsInUs = 1000
};

uint64_t osalMsToSec(uint64_t milliseconds)
{
    return milliseconds / eOsalMsInSec;
}

uint64_t osalUsToSec(uint64_t microseconds)
{
    return microseconds / eOsalUsInSec;
}

uint64_t osalNsToSec(uint64_t nanoseconds)
{
    return nanoseconds / eOsalNsInSec;
}

uint64_t osalSecToMs(uint64_t seconds)
{
    return seconds * eOsalMsInSec;
}

uint64_t osalUsToMs(uint64_t microseconds)
{
    return microseconds / eOsalUsInMs;
}

uint64_t osalNsToMs(uint64_t nanoseconds)
{
    return nanoseconds / eOsalNsInMs;
}

uint64_t osalSecToUs(uint64_t seconds)
{
    return seconds * eOsalUsInSec;
}

uint64_t osalMsToUs(uint64_t milliseconds)
{
    return milliseconds * eOsalUsInMs;
}

uint64_t osalNsToUs(uint64_t nanoseconds)
{
    return nanoseconds / eOsalNsInUs;
}

uint64_t osalSecToNs(uint64_t seconds)
{
    return seconds * eOsalNsInSec;
}

uint64_t osalMsToNs(uint64_t milliseconds)
{
    return milliseconds * eOsalNsInMs;
}

uint64_t osalUsToNs(uint64_t microseconds)
{
    return microseconds * eOsalNsInUs;
}
