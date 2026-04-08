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

#include <osal/Error.h>
#include <osal/Semaphore.hpp>
#include <osal/Thread.h>
#include <osal/sleep.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <set>
#include <string_view>
#include <tuple>

TEST_CASE("Thread creation and destruction", "[unit][c][thread]")
{
    auto func = [](void* /*unused*/) {};

    OsalThread thread{};
    auto error
        = osalThreadCreate(&thread, {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr}, func, nullptr);
    REQUIRE_FALSE(error);

    error = osalThreadJoin(&thread);
    CHECK_FALSE(error);

    error = osalThreadDestroy(&thread);
    CHECK_FALSE(error);

    error = osalThreadDestroy(&thread);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Named thread creation and destruction", "[unit][c][thread]")
{
    constexpr std::size_t cGetNameSize = 16;

    struct ThreadData {
        std::array<char, cGetNameSize> getThreadName{};
        osal::Semaphore startSemaphore{0};
        osal::Semaphore stopSemaphore{0};
    };

    ThreadData threadData;

    auto func = [](void* arg) {
        auto* threadData = static_cast<ThreadData*>(arg);

        CHECK_FALSE(threadData->startSemaphore.wait());
        osalThreadName(threadData->getThreadName.data(), threadData->getThreadName.size());
        CHECK_FALSE(threadData->stopSemaphore.signal());
    };

    std::string_view setThreadName = "0123456789ABCDE";

    OsalThread thread{};
    auto error = osalThreadCreateEx(&thread,
                                    {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr},
                                    func,
                                    &threadData,
                                    setThreadName.data());
    REQUIRE_FALSE(error);

    CHECK_FALSE(threadData.startSemaphore.signal());
    CHECK_FALSE(threadData.stopSemaphore.wait());
    std::string_view getThreadName{threadData.getThreadName.data(), std::strlen(threadData.getThreadName.data())};
    CHECK_THAT(getThreadName.data(), Catch::Matchers::Equals(setThreadName.data()));

    error = osalThreadJoin(&thread);
    CHECK_FALSE(error);

    error = osalThreadDestroy(&thread);
    CHECK_FALSE(error);

    error = osalThreadDestroy(&thread);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Thread creation with invalid arguments", "[unit][c][thread]")
{
    auto func = [](void* /*unused*/) {};

    OsalThread thread{};
    auto error
        = osalThreadCreate(nullptr, {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr}, func, nullptr);
    CHECK(error == OsalError::InvalidArgument);

    error = osalThreadCreate(&thread,
                             {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr},
                             nullptr,
                             nullptr);
    CHECK(error == OsalError::InvalidArgument);

    error = osalThreadCreateEx(&thread,
                               {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr},
                               func,
                               nullptr,
                               "0123456789ABCDEF");
    CHECK(error == OsalError::InvalidArgument);

    constexpr int cInvalidPriority = 5;
    error = osalThreadCreate(&thread,
                             {static_cast<OsalThreadPriority>(cInvalidPriority), cOsalThreadDefaultStackSize, nullptr},
                             func,
                             nullptr);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Multiple thread joins", "[unit][c][thread]")
{
    auto func = [](void* /*unused*/) {};

    OsalThread thread{};
    auto error
        = osalThreadCreate(&thread, {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr}, func, nullptr);
    REQUIRE_FALSE(error);

    error = osalThreadJoin(&thread);
    CHECK_FALSE(error);

    error = osalThreadJoin(&thread);
    CHECK(error == OsalError::InvalidArgument);

    error = osalThreadDestroy(&thread);
    CHECK_FALSE(error);
}

TEST_CASE("Join invalid thread", "[unit][c][thread]")
{
    OsalThread thread{};

    auto error = osalThreadJoin(&thread);
    CHECK(error == OsalError::InvalidArgument);

    error = osalThreadJoin(nullptr);
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Destroy thread without join", "[unit][c][thread]")
{
    auto func = [](void* /*unused*/) {};

    OsalThread thread{};
    auto error
        = osalThreadCreate(&thread, {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr}, func, nullptr);
    REQUIRE_FALSE(error);

    error = osalThreadDestroy(&thread);
    CHECK(error == OsalError::ThreadNotJoined);

    error = osalThreadJoin(&thread);
    CHECK_FALSE(error);

    error = osalThreadDestroy(&thread);
    CHECK_FALSE(error);
}

TEST_CASE("Launch 5 threads and check their results", "[unit][c][thread]")
{
    constexpr int cIterationsCount = 100;
    constexpr std::size_t cThreadsCount = 5;
    std::array<OsalThread, cThreadsCount> threads{};
    std::array<int, cThreadsCount> counters{};

    auto func = [](void* arg) {
        auto& counter = *static_cast<int*>(arg);
        for (counter = 0; counter < cIterationsCount; ++counter)
            ;
    };

    for (std::size_t i = 0; i < threads.size(); ++i) {
        auto error = osalThreadCreate(&threads[i],
                                      {cOsalThreadDefaultPriority, cOsalThreadDefaultStackSize, nullptr},
                                      func,
                                      &counters[i]);
        REQUIRE_FALSE(error);
    }

    for (std::size_t i = 0; i < threads.size(); ++i) {
        auto error = osalThreadJoin(&threads[i]);
        CHECK_FALSE(error);

        auto counter = counters[i];
        CHECK(counter == cIterationsCount);

        error = osalThreadDestroy(&threads[i]);
        CHECK_FALSE(error);
    }
}

TEST_CASE("Launch 5 threads with different priorities and check their results", "[unit][c][thread]")
{
    using ThreadArgs = std::tuple<unsigned int&, bool&, bool&>;

    constexpr std::size_t cThreadsCount = 5;
    std::array<OsalThread, cThreadsCount> threads{};
    std::array<unsigned int, cThreadsCount> counters{};
    bool start{};
    bool stop{};

    auto func = [](void* arg) {
        auto& params = *static_cast<ThreadArgs*>(arg);
        auto& counter = std::get<0>(params);
        auto& start = std::get<1>(params);
        auto& stop = std::get<2>(params);

        while (!start)
            osalThreadYield();

        while (!stop) {
            ++counter;
            osalThreadYield();
        }
    };

    std::array<ThreadArgs, cThreadsCount> args
        = {std::make_tuple(std::ref(counters[0]), std::ref(start), std::ref(stop)),
           std::make_tuple(std::ref(counters[1]), std::ref(start), std::ref(stop)),
           std::make_tuple(std::ref(counters[2]), std::ref(start), std::ref(stop)),
           std::make_tuple(std::ref(counters[3]), std::ref(start), std::ref(stop)),
           std::make_tuple(std::ref(counters[4]), std::ref(start), std::ref(stop))};

    for (std::size_t i = 0; i < threads.size(); ++i) {
        auto priority = static_cast<OsalThreadPriority>(i / 2);
        auto error = osalThreadCreate(&threads[i], {priority, cOsalThreadDefaultStackSize, nullptr}, func, &args[i]);
        REQUIRE_FALSE(error);
    }

    start = true; // NOLINT
    osal::sleep(5s);
    stop = true; // NOLINT

    for (auto& thread : threads) {
        auto error = osalThreadJoin(&thread);
        CHECK_FALSE(error);

        error = osalThreadDestroy(&thread);
        CHECK_FALSE(error);
    }

    for (auto counter : counters)
        CHECK(counter != 0);
}

TEST_CASE("Create threads with all priorities", "[unit][c][thread]")
{
    auto func = [](void* /*unused*/) { osal::sleep(1s); };

    for (int i = OsalThreadPriority::Lowest; i <= OsalThreadPriority::Highest; ++i) {
        OsalThread thread{};
        auto priority = static_cast<OsalThreadPriority>(i / 2);
        auto error = osalThreadCreate(&thread, {priority, cOsalThreadDefaultStackSize, nullptr}, func, nullptr);
        REQUIRE_FALSE(error);

        error = osalThreadJoin(&thread);
        CHECK_FALSE(error);

        error = osalThreadDestroy(&thread);
        CHECK_FALSE(error);
    }
}

TEST_CASE("Check if thread ids are unique and constant", "[unit][c][thread]")
{
    using ThreadArgs = std::tuple<std::uint32_t&, bool&>;

    constexpr std::size_t cThreadsCount = 5;
    std::array<OsalThread, cThreadsCount> threads{};
    std::array<std::uint32_t, cThreadsCount> ids{};
    bool start{};

    auto func = [](void* arg) {
        auto& params = *static_cast<ThreadArgs*>(arg);
        auto& id = std::get<0>(params);
        auto& start = std::get<1>(params);

        while (!start)
            osalThreadYield();

        id = osalThreadId();
        constexpr int cIterationsCount = 1000;
        for (int i = 0; i < cIterationsCount; ++i) {
            auto tmpId = osalThreadId();
            if (tmpId != id)
                REQUIRE(tmpId == id);
            osalThreadYield();
        }
    };

    std::array<ThreadArgs, cThreadsCount> args = {std::make_tuple(std::ref(ids[0]), std::ref(start)),
                                                  std::make_tuple(std::ref(ids[1]), std::ref(start)),
                                                  std::make_tuple(std::ref(ids[2]), std::ref(start)),
                                                  std::make_tuple(std::ref(ids[3]), std::ref(start)),
                                                  std::make_tuple(std::ref(ids[4]), std::ref(start))};

    for (std::size_t i = 0; i < threads.size(); ++i) {
        auto priority = static_cast<OsalThreadPriority>(i / 2);
        auto error = osalThreadCreate(&threads[i], {priority, cOsalThreadDefaultStackSize, nullptr}, func, &args[i]);
        REQUIRE_FALSE(error);
    }

    start = true; // NOLINT

    for (auto& thread : threads) {
        auto error = osalThreadJoin(&thread);
        CHECK_FALSE(error);

        error = osalThreadDestroy(&thread);
        CHECK_FALSE(error);
    }

    std::set<std::uint32_t> uniqueIds;
    for (auto id : ids)
        uniqueIds.insert(id);

    CHECK(uniqueIds.size() == cThreadsCount);
}
