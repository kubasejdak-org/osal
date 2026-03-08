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

#include <osal/Error.h>
#include <osal/Error.hpp> // NOLINT(misc-include-cleaner)

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <string>
#include <system_error>

TEST_CASE("Errors have proper human readable messages", "[unit][cpp][error]")
{
    const std::string cUnrecognizedMsg = "osal: unrecognized error";
    constexpr int cErrorsCount = 8;

    for (int i = 1; i <= cErrorsCount; ++i) {
        std::error_code error = static_cast<OsalError>(i);
        CHECK_THAT(error.category().name(), Catch::Matchers::Equals("osal"));
        CHECK(!error.message().empty());
        CHECK(error.message() != cUnrecognizedMsg);
    }

    constexpr int cInvalidError = cErrorsCount + 1;
    std::error_code error = static_cast<OsalError>(cInvalidError);
    CHECK_THAT(error.category().name(), Catch::Matchers::Equals("osal"));
    CHECK(!error.message().empty());
    CHECK_THAT(error.message(), Catch::Matchers::Equals(cUnrecognizedMsg));
}
