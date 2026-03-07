/////////////////////////////////////////////////////////////////////////////////////
///
/// @file
/// @author Kuba Sejdak
/// @copyright MIT License
///
/// Copyright (c) 2019 Kuba Sejdak (kuba.sejdak@gmail.com)
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

#include "osal/Error.hpp"

#include "osal/Error.h"

#include <string>
#include <system_error>

namespace osal {

struct ErrorCategory : std::error_category {
    [[nodiscard]] const char* name() const noexcept override;
    [[nodiscard]] std::string message(int value) const override;
};

const char* ErrorCategory::name() const noexcept
{
    return "osal";
}

std::string ErrorCategory::message(int value) const
{
    switch (static_cast<OsalError>(value)) {
        case OsalError::eOk:                   return "no error";
        case OsalError::eInvalidArgument:      return "invalid argument";
        case OsalError::eOsError:              return "OS error";
        case OsalError::eThreadNotJoined:      return "thread not joined";
        case OsalError::eThreadAlreadyStarted: return "thread already started";
        case OsalError::eNotOwner:             return "not owner";
        case OsalError::eNotLocked:            return "not locked";
        case OsalError::eLocked:               return "locked";
        case OsalError::eTimeout:              return "timeout";
        default:                               return "(unrecognized error)";
    }
}

} // namespace osal

// NOLINTNEXTLINE(readability-identifier-naming)
std::error_code make_error_code(OsalError error)
{
    static const osal::ErrorCategory cErrorCategory{};
    return {static_cast<int>(error), cErrorCategory};
}
