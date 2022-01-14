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

#ifdef __cplusplus
extern "C" {
#endif

#include "osal/Error.h"

#include <stdint.h>   // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)
#include <sys/time.h> // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)
#include <time.h>     // NOLINT(modernize-deprecated-headers,hicpp-deprecated-headers)

struct tm osalTimeToTm(time_t value);
struct tm osalTimespecToTm(struct timespec value);
struct tm osalTimevalToTm(struct timeval value);

time_t osalTmToTime(struct tm value);
time_t osalTimespecToTime(struct timespec value);
time_t osalTimevalToTime(struct timeval value);

struct timespec osalTimeToTimespec(time_t value);
struct timespec osalTmToTimespec(struct tm value);
struct timespec osalTimevalToTimespec(struct timeval value);

struct timeval osalTimeToTimeval(time_t value);
struct timeval osalTmToTimeval(struct tm value);
struct timeval osalTimespecToTimeval(struct timespec value);

enum OsalTimeStringFormat {
    eTime,          // "15:30:59"
    eDate,          // "31.12.2022"
    eTimeDate,      // "15:30:59 31.12.2022"
    eSortedDateTime // "20221231_153059"
};

OsalError osalTmToString(struct tm value, char* str, size_t size, OsalTimeStringFormat format);
OsalError osalTimeToString(time_t value, char* str, size_t size, OsalTimeStringFormat format);
OsalError osalTimespecToString(struct timespec value, char* str, size_t size, OsalTimeStringFormat format);
OsalError osalTimevalToString(struct timeval value, char* str, size_t size, OsalTimeStringFormat format);

#ifdef __cplusplus
}
#endif
