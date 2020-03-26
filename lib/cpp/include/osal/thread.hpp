/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright BSD 2-Clause License
///
/// Copyright (c) 2019-2020, Kuba Sejdak <kuba.sejdak@gmail.com>
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

#include "osal/error.hpp"
#include "osal/thread.h"

#include <cstdint>
#include <functional>
#include <system_error>
#include <utility>

namespace osal {

template <OsalThreadPriority priority = cOsalThreadDefaultPriority, std::size_t stackSize = cOsalThreadDefaultStackSize>
class Thread {
public:
    Thread() = default;

    template <typename... Args>
    Thread(OsalThreadFunction function, Args&&... args) // NOLINT
        : Thread(nullptr, function, std::forward<Args>(args)...)
    {}

    template <typename... Args>
    Thread(void* stack, OsalThreadFunction function, Args&&... args)
    {
        start(stack, function, std::forward<Args>(args)...);
    }

    Thread(const Thread&) = delete;

    Thread(Thread&& other) noexcept
    {
        std::swap(m_thread, other.m_thread);
        m_functionWrapper = std::move(other.m_functionWrapper);
        std::swap(m_started, other.m_started);
    }

    ~Thread() { osalThreadDestroy(&m_thread); }

    Thread& operator=(const Thread&) = delete;

    Thread& operator=(Thread&& other) = delete;

    template <typename... Args>
    std::error_code start(OsalThreadFunction function, Args&&... args)
    {
        return start(nullptr, function, std::forward<Args>(args)...);
    }

    template <typename... Args>
    std::error_code start(void* stack, OsalThreadFunction function, Args&&... args)
    {
        if (m_started)
            return OsalError::eThreadAlreadyStarted;

        auto threadFunction = std::bind(std::forward<OsalThreadFunction>(function), std::forward<Args>(args)...);
        m_functionWrapper = [threadFunction = threadFunction](void* /*unused*/) { threadFunction(); };
        auto error = osalThreadCreate(&m_thread,
                                      {priority, stackSize, stack},
                                      m_functionWrapper.target<void(void*)>(),
                                      nullptr);

        m_started = (error == OsalError::eOk);
        return error;
    }

    void join() { osalThreadJoin(&m_thread); }

    static void yield() { osalThreadYield(); }

    static std::uint32_t id() { return osalThreadId(); }

private:
    OsalThread m_thread{};
    std::function<void(void*)> m_functionWrapper;
    bool m_started{};
};

} // namespace osal
