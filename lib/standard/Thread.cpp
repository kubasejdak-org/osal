/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2020-2020, Kuba Sejdak <kuba.sejdak@gmail.com>
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

#include "osal/Thread.h"

#if __has_include(<pthread.h>)
  #include <pthread.h>
  #include <sys/types.h>
  #include <unistd.h>

  constexpr bool cUsePthread = true;
  constexpr bool cUseWinThread = false;
#elif __has_include(<windows.h>)
  #include <windows.h>
  constexpr bool cUsePthread = false;
  constexpr bool cUseWinThread = true;
#else
  #error Unsupported platform
#endif 

#include <thread>

OsalError osalThreadCreate(OsalThread* thread, OsalThreadConfig config, OsalThreadFunction func, void* arg)
{
    if (thread == nullptr || func == nullptr)
        return OsalError::eInvalidArgument;

    std::thread handle(func, arg);

    if constexpr (cUsePthread) {
        const auto cPriorityMin = sched_get_priority_min(SCHED_RR);
        const auto cPriorityMax = sched_get_priority_max(SCHED_RR);
        const auto cPriorityStep = (cPriorityMax - cPriorityMin) / 4;

        int priority{};
        switch (config.priority) {
            case OsalThreadPriority::eLowest:
                priority = cPriorityMin;
                break;
            case OsalThreadPriority::eLow:
                priority = cPriorityMin + (cPriorityStep * 1);
                break;
            case OsalThreadPriority::eNormal:
                priority = cPriorityMin + (cPriorityStep * 2);
                break;
            case OsalThreadPriority::eHigh:
                priority = cPriorityMin + (cPriorityStep * 3);
                break;
            case OsalThreadPriority::eHighest:
                priority = cPriorityMax;
                break;
            default: break;
        }

        sched_param schedParam{priority};
        if (pthread_setschedparam(handle.native_handle(), SCHED_RR, &schedParam) != 0)
            return OsalError::eOsError;
    }
    else if constexpr (cUseWinThread) {
        // TODO: set priority and stack size.
    }

    thread->impl.handle.swap(handle);
    return OsalError::eOk;
}

OsalError osalThreadDestroy(OsalThread* thread)
{
    if (thread == nullptr)
        return OsalError::eInvalidArgument;

    if (thread->impl.handle.joinable())
        return OsalError::eThreadNotJoined;

    return OsalError::eOk;
}

OsalError osalThreadJoin(OsalThread* thread)
{
    if (thread == nullptr)
        return OsalError::eInvalidArgument;

    thread->impl.handle.join();
    return OsalError::eOk;
}

void osalThreadYield()
{
    std::this_thread::yield();
}

uint32_t osalThreadId()
{
    return uint32_t(std::hash<std::thread::id>{}(std::this_thread::get_id()));
}
