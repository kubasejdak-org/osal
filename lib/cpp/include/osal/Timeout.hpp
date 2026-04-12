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

#include "osal/sleep.hpp"
#include "osal/timestamp.hpp"

#include <cassert>
#include <chrono>
#include <type_traits>

// NOLINTNEXTLINE(google-global-names-in-headers)
using namespace std::chrono_literals;

namespace osal {
namespace detail {

/// Helper type to perform SFINAE to prevent using std::chrono unit in Timeout, which is smaller than Duration,
/// than Duration type.
template <typename T>
using NotLessThanDuration = std::enable_if_t<std::is_same_v<std::common_type_t<T, Duration>, Duration>, bool>;

} // namespace detail

/// Represents an universal timeout object, that can tell if the given timeout has already expired.
/// Upon construction Timeout object calculates timestamp value, which determines deadline for the given operation.
/// Calling "isExpired()" simply compares current timestamp value with the calculated one.
/// @note This class is supposed to replace all occurrences of the raw std::uint32_t timeout values in OSAL and HAL
///       APIs.
class Timeout {
public:
    /// Constructor.
    /// @tparam Representation      Signed arithmetic type representing the number of ticks in the clock's duration.
    /// @tparam Period              A std::ratio type representing the tick period of the clock, in seconds.
    /// @param duration             Maximal time duration in std::chrono duration value, that should last
    ///                             before timeout is considered as expired.
    /// @param forceExpire          Flag forcing timeout to be immediately expired after construction.
    template <typename Representation,
              typename Period,
              typename = detail::NotLessThanDuration<std::chrono::duration<Representation, Period>>>
    // NOLINETNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
    Timeout(const std::chrono::duration<Representation, Period>& duration, bool forceExpire = false)
        : m_duration(duration)
        , m_infinity(duration == Duration::max())
    {
        if (!isInfinity())
            m_expireTimestamp = timestamp() + (forceExpire ? Duration::zero() : duration);

        assert(duration >= Duration::zero());
    }

    /// Conversion operator to Duration.
    /// @return Duration value representing time left in std::chrono unit to the deadline timestamp.
    operator Duration() const { return timeLeft(); }

    /// Returns the std::chrono duration value used to initialize timeout.
    /// @return std::chrono duration value used to initialize timeout.
    [[nodiscard]] Duration duration() const { return m_duration; }

    /// Checks, if the deadline has been reached.
    /// @return Boolean flag indicating if the deadline has been reached.
    /// @retval true                 Deadline has been reached.
    /// @retval false                Deadline has not been reached.
    [[nodiscard]] bool isExpired() const { return !isInfinity() && (timeLeft() == Duration::zero()); }

    /// Checks, if given timeout represents infinity.
    /// @return Boolean flag indicating if given timeout represents infinity.
    /// @retval true                 Timeout represents infinity.
    /// @retval false                Timeout does not represent infinity.
    [[nodiscard]] bool isInfinity() const { return m_infinity; }

    /// Returns the std::chrono duration value representing time left to the deadline timestamp.
    /// @return std::chrono duration value representing time left to the deadline timestamp.
    [[nodiscard]] Duration timeLeft() const
    {
        if (isInfinity())
            return Duration::max();

        auto now = timestamp();
        if (now > m_expireTimestamp)
            return Duration::zero();

        return std::chrono::duration_cast<Duration>(m_expireTimestamp - now);
    }

    /// Resets internal state of the timeout. Deadline is recalculated as if timeout was created during this call.
    void reset()
    {
        Timeout other(duration());
        m_expireTimestamp = other.m_expireTimestamp;
    }

    /// Returns helper constant representing zero timeout.
    /// @return Helper constant representing zero timeout.
    static Timeout none() { return {Duration::zero()}; }

    /// Returns helper constant representing infinity timeout.
    /// @return Helper constant representing infinity timeout.
    static Timeout infinity() { return {Duration::max()}; }

private:
    Duration m_duration;
    bool m_infinity;
    std::chrono::time_point<Clock, Duration> m_expireTimestamp;
};

/// Blocks current thread until given timeout is expired.
/// @param timeout                  Timeout to be checked.
inline void sleepUntilExpired(const Timeout& timeout)
{
    if (!timeout.isExpired())
        sleep(timeout.timeLeft());
}

/// Coverts duration of the given timeout to raw milliseconds.
/// @param timeout                  Timeout to be used.
/// @return Duration of the given timeout expressed in raw milliseconds.
inline std::uint32_t durationMs(const osal::Timeout& timeout)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(timeout.duration()).count();
}

} // namespace osal
