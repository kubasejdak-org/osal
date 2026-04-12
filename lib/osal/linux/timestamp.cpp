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

#include <chrono>
#include <cstdint>

namespace {

/// Internal value of system time latched during timestamp module initialization.
const auto cInitTime = std::chrono::steady_clock::now();

/// Returns the time since the timestamp module was initialized.
/// @tparam Unit        Unit in which time should be represented (converted to).
/// @return Returns the time since the timestamp module was initialized.
template <typename Unit>
std::uint64_t timeSinceStart()
{
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<Unit>(now - cInitTime).count();
}

} // namespace

uint64_t osalTimestampMs()
{
    return timeSinceStart<std::chrono::milliseconds>();
}

uint64_t osalTimestampUs()
{
    return timeSinceStart<std::chrono::microseconds>();
}

uint64_t osalTimestampNs()
{
    return timeSinceStart<std::chrono::nanoseconds>();
}
