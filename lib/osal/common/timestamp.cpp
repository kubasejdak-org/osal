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

static constexpr std::uint64_t cOsalMsInSec = 1000;
static constexpr std::uint64_t cOsalUsInSec = 1000000;
static constexpr std::uint64_t cOsalNsInSec = 1000000000;
static constexpr std::uint64_t cOsalUsInMs = 1000;
static constexpr std::uint64_t cOsalNsInMs = 1000000;
static constexpr std::uint64_t cOsalNsInUs = 1000;

uint64_t osalMsToSec(uint64_t milliseconds)
{
    return milliseconds / cOsalMsInSec;
}

uint64_t osalUsToSec(uint64_t microseconds)
{
    return microseconds / cOsalUsInSec;
}

uint64_t osalNsToSec(uint64_t nanoseconds)
{
    return nanoseconds / cOsalNsInSec;
}

uint64_t osalSecToMs(uint64_t seconds)
{
    return seconds * cOsalMsInSec;
}

uint64_t osalUsToMs(uint64_t microseconds)
{
    return microseconds / cOsalUsInMs;
}

uint64_t osalNsToMs(uint64_t nanoseconds)
{
    return nanoseconds / cOsalNsInMs;
}

uint64_t osalSecToUs(uint64_t seconds)
{
    return seconds * cOsalUsInSec;
}

uint64_t osalMsToUs(uint64_t milliseconds)
{
    return milliseconds * cOsalUsInMs;
}

uint64_t osalNsToUs(uint64_t nanoseconds)
{
    return nanoseconds / cOsalNsInUs;
}

uint64_t osalSecToNs(uint64_t seconds)
{
    return seconds * cOsalNsInSec;
}

uint64_t osalMsToNs(uint64_t milliseconds)
{
    return milliseconds * cOsalNsInMs;
}

uint64_t osalUsToNs(uint64_t microseconds)
{
    return microseconds * cOsalNsInUs;
}
