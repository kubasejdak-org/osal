/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2022-2022, Kuba Sejdak <kuba.sejdak@gmail.com>
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this
///    list of conditions and the following disclaimer.
///
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "osal/time.h" // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

#include <ctime>
#include <string>

namespace osal {

std::tm toTm(std::time_t value);
std::tm toTm(timespec value);
std::tm toTm(timeval value);

std::time_t toTime(std::tm value);
std::time_t toTime(timespec value);
std::time_t toTime(timeval value);

timespec toTimespec(std::time_t value);
timespec toTimespec(std::tm value);
timespec toTimespec(timeval value);

timeval toTimeval(std::time_t value);
timeval toTimeval(std::tm value);
timeval toTimeval(timespec value);

std::string toString(std::tm value, OsalTimeStringFormat format = OsalTimeStringFormat::eTimeDate);
std::string toString(std::time_t value, OsalTimeStringFormat format = OsalTimeStringFormat::eTimeDate);
std::string toString(timespec value, OsalTimeStringFormat format = OsalTimeStringFormat::eTimeDate);
std::string toString(timeval value, OsalTimeStringFormat format = OsalTimeStringFormat::eTimeDate);

} // namespace osal
