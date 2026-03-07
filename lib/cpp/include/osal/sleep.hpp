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

#include <chrono>
#include <cstdint>

// NOLINTNEXTLINE(google-global-names-in-headers)
using namespace std::chrono_literals;

namespace osal {
namespace detail {

/// Internal function implementing thread suspension for time intervals in ms.
/// @param durationMs           Time to sleep in ms.
void sleepMs(std::uint64_t durationMs);

} // namespace detail

/// Suspends the current thread for the specified amount of time.
/// @tparam Representation      Signed arithmetic type representing the number of ticks in the clock's duration.
/// @tparam Period              A std::ratio type representing the tick period of the clock, in seconds.
/// @param duration             Amount of time for which current thread should be suspended.
/// @note This function can accept any time duration unit supported by std::chrono library.
template <typename Representation, typename Period>
void sleep(const std::chrono::duration<Representation, Period>& duration)
{
    detail::sleepMs(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}

} // namespace osal
