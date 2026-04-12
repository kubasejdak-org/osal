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
#include <osal/Thread.hpp>
#include <osal/sleep.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <utility>

TEST_CASE("Thread creation and destruction in C++", "[unit][cpp][thread]")
{
    constexpr unsigned int cParam = 0xdeadbeef;
    bool launched = false;
    auto func = [&](unsigned int arg) {
        osal::sleep(1s);

        REQUIRE(arg == cParam);
        launched = true;
    };

    auto func2 = [&]() {
        osal::sleep(1s);

        launched = true;
    };

    SECTION("Create thread via constructor")
    {
        osal::Thread thread(func, cParam);

        auto error = thread.join();
        CHECK_FALSE(error);
    }

    SECTION("Create thread via start() method")
    {
        osal::Thread thread;
        auto error = thread.start(func, cParam);
        REQUIRE_FALSE(error);

        error = thread.join();
        CHECK_FALSE(error);
    }

    SECTION("Start thread multiple times: via constructor and via start() method")
    {
        osal::Thread thread(func, cParam);

        auto error = thread.start(func, cParam);
        CHECK(error == OsalError::ThreadAlreadyStarted);

        error = thread.join();
        CHECK_FALSE(error);

        osal::Thread thread2(func2);

        error = thread2.start(func2);
        CHECK(error == OsalError::ThreadAlreadyStarted);

        error = thread2.join();
        CHECK_FALSE(error);
    }

    SECTION("Start thread multiple times: 2 times via start() method")
    {
        osal::Thread thread;
        auto error = thread.start(func, cParam);
        REQUIRE_FALSE(error);

        error = thread.start(func, cParam);
        CHECK(error == OsalError::ThreadAlreadyStarted);

        error = thread.join();
        CHECK_FALSE(error);
    }

    CHECK(launched);
}

TEST_CASE("Named thread creation and destruction", "[unit][cpp][thread]")
{
    std::string getThreadName;
    osal::Semaphore startSemaphore{0};
    osal::Semaphore stopSemaphore{0};

    auto func = [&] {
        CHECK_FALSE(startSemaphore.wait());
        getThreadName = osal::thread::name();
        CHECK_FALSE(stopSemaphore.signal());
    };

    std::string_view setThreadName = "0123456789ABCDE";

    osal::Thread thread(setThreadName, func);

    CHECK_FALSE(startSemaphore.signal());
    CHECK_FALSE(stopSemaphore.wait());
    CHECK_THAT(getThreadName, Catch::Matchers::Equals(setThreadName.data()));

    auto error = thread.join();
    CHECK_FALSE(error);
}

TEST_CASE("Thread creation with custom stack", "[unit][cpp][thread]")
{
    bool launched = false;
    auto func = [&]() {
        osal::sleep(1s);

        launched = true;
    };

    SECTION("Create thread with custom stack")
    {
        std::array<char, cOsalThreadDefaultStackSize> stack{};
        osal::Thread thread;

        auto error = thread.setStack(stack.data());
        CHECK_FALSE(error);

        error = thread.start(func);
        REQUIRE_FALSE(error);

        error = thread.join();
        CHECK_FALSE(error);

        CHECK(launched);
    }

    SECTION("Create thread with nullptr stack")
    {
        osal::Thread thread;
        auto error = thread.setStack(nullptr);
        CHECK(error == OsalError::InvalidArgument);
    }
}

TEST_CASE("Thread creation with variadic arguments", "[unit][cpp][thread]")
{
    bool launched = false;
    int counter = 0;
    int expectedCounter = 0;

    SECTION("No arguments")
    {
        auto func = [&] {
            osal::sleep(100ms);
            launched = true;
        };

        osal::Thread thread(func);
    }

    SECTION("1 argument")
    {
        auto func = [&](int a) {
            osal::sleep(100ms);
            counter += a;
            launched = true;
        };

        osal::Thread thread(func, 1);
        expectedCounter = 1;
    }

    SECTION("7 arguments")
    {
        auto func = [&](int a, int b, int c, int d, int e, int f, int g) {
            osal::sleep(100ms);
            counter += (a + b + c + d + e + f + g);
            launched = true;
        };

        osal::Thread thread(func, 1, 2, 3, 4, 1, 2, 3);
        constexpr int cExpectedCounter = 16;
        expectedCounter = cExpectedCounter;
    }

    CHECK(launched);
    CHECK(counter == expectedCounter);
}

TEST_CASE("Thread creation in C++ with different priorities", "[unit][cpp][thread]")
{
    bool launched = false;

    auto func = [&] {
        osal::sleep(100ms);
        launched = true;
    };

    SECTION("Lowest priority")
    {
        osal::Thread<OsalThreadPriority::Lowest> thread(func);
    }

    SECTION("Low priority")
    {
        osal::Thread<OsalThreadPriority::Low> thread(func);
    }

    SECTION("Normal priority")
    {
        osal::Thread<OsalThreadPriority::Normal> thread(func);
    }

    SECTION("High priority")
    {
        osal::Thread<OsalThreadPriority::High> thread(func);
    }

    SECTION("Highest priority")
    {
        osal::Thread<OsalThreadPriority::Highest> thread(func);
    }

    CHECK(launched);
}

TEST_CASE("Thread creation in C++ with different priorities using helper types", "[unit][cpp][thread]")
{
    bool launched = false;

    auto func = [&] {
        osal::sleep(100ms);
        launched = true;
    };

    SECTION("Lowest priority")
    {
        osal::LowestPrioThread<> thread(func);
    }

    SECTION("Low priority")
    {
        osal::LowPrioThread<> thread(func);
    }

    SECTION("Normal priority")
    {
        osal::NormalPrioThread<> thread(func);
    }

    SECTION("High priority")
    {
        osal::HighPrioThread<> thread(func);
    }

    SECTION("Highest priority")
    {
        osal::HighestPrioThread<> thread(func);
    }

    CHECK(launched);
}

TEST_CASE("Move thread around", "[unit][cpp][thread]")
{
    constexpr unsigned int cParam = 0xdeadbeef;
    bool launched = false;
    auto func = [&](unsigned int arg) {
        osal::sleep(5s);

        REQUIRE(arg == cParam);
        launched = true;
    };

    osal::Thread thread1(func, cParam);
    osal::Thread thread2(std::move(thread1));

    // NOLINTNEXTLINE(bugprone-use-after-move,clang-analyzer-cplusplus.Move,hicpp-invalid-access-moved)
    auto error = thread1.join();
    CHECK(error == OsalError::InvalidArgument);

    error = thread2.join();
    CHECK_FALSE(error);

    CHECK(launched);
}

TEST_CASE("Multiple thread joins in C++", "[unit][cpp][thread]")
{
    auto func = [] {};

    osal::Thread thread(func);

    auto error = thread.join();
    CHECK_FALSE(error);

    error = thread.join();
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Join invalid thread in C++", "[unit][cpp][thread]")
{
    osal::Thread thread;

    auto error = thread.join();
    CHECK(error == OsalError::InvalidArgument);
}

TEST_CASE("Launch 5 threads in C++ and check their results", "[unit][cpp][thread]")
{
    constexpr int cIterationsCount = 100;
    auto func = [](int& counter) {
        for (counter = 0; counter < cIterationsCount; ++counter)
            ;
    };

    constexpr std::size_t cThreadsCount = 5;
    std::array<osal::Thread<>, cThreadsCount> threads{};
    std::array<int, cThreadsCount> counters{};

    for (std::size_t i = 0; i < threads.size(); ++i) {
        auto error = threads[i].start(func, std::ref(counters[i]));
        REQUIRE_FALSE(error);
    }

    for (std::size_t i = 0; i < threads.size(); ++i) {
        auto error = threads[i].join();
        CHECK_FALSE(error);

        auto counter = counters[i];
        CHECK(counter == cIterationsCount);
    }
}

TEST_CASE("Launch 5 threads in C++ with different priorities and check their results", "[unit][cpp][thread]")
{
    constexpr std::size_t cThreadsCount = 5;
    std::array<unsigned int, cThreadsCount> counters{};
    bool start{};
    bool stop{};

    auto func = [](unsigned int& counter, bool& start, bool& stop) {
        while (!start)
            osal::thread::yield();

        while (!stop) {
            ++counter;
            osal::thread::yield();
        }
    };

    osal::LowestPrioThread<> thread1(func, std::ref(counters[0]), std::ref(start), std::ref(stop));
    osal::LowestPrioThread<> thread2(func, std::ref(counters[1]), std::ref(start), std::ref(stop));
    osal::LowPrioThread<> thread3(func, std::ref(counters[2]), std::ref(start), std::ref(stop));
    osal::LowPrioThread<> thread4(func, std::ref(counters[3]), std::ref(start), std::ref(stop));
    osal::NormalPrioThread<> thread5(func, std::ref(counters[4]), std::ref(start), std::ref(stop));

    start = true; // NOLINT(clang-analyzer-deadcode.DeadStores)
    osal::sleep(5s);
    stop = true; // NOLINT(clang-analyzer-deadcode.DeadStores)

    auto error = thread1.join();
    CHECK_FALSE(error);
    error = thread2.join();
    CHECK_FALSE(error);
    error = thread3.join();
    CHECK_FALSE(error);
    error = thread4.join();
    CHECK_FALSE(error);
    error = thread5.join();
    CHECK_FALSE(error);

    for (auto counter : counters)
        CHECK(counter != 0);
}

TEST_CASE("Check if thread ids are unique and constant in C++", "[unit][cpp][thread]")
{
    constexpr std::size_t cThreadsCount = 5;
    std::array<std::uint32_t, cThreadsCount> ids{};
    bool start{};

    auto func = [](std::uint32_t& id, bool& start) {
        while (!start)
            osal::thread::yield();

        id = osal::thread::id();
        constexpr int cIterationsCount = 1000;
        for (int i = 0; i < cIterationsCount; ++i) {
            auto tmpId = osal::thread::id();
            if (tmpId != id)
                REQUIRE(tmpId == id);

            osal::thread::yield();
        }
    };

    osal::LowestPrioThread<> thread1(func, std::ref(ids[0]), std::ref(start));
    osal::LowestPrioThread<> thread2(func, std::ref(ids[1]), std::ref(start));
    osal::LowPrioThread<> thread3(func, std::ref(ids[2]), std::ref(start));
    osal::LowPrioThread<> thread4(func, std::ref(ids[3]), std::ref(start));
    osal::NormalPrioThread<> thread5(func, std::ref(ids[4]), std::ref(start));

    start = true; // NOLINT(clang-analyzer-deadcode.DeadStores)

    auto error = thread1.join();
    CHECK_FALSE(error);
    error = thread2.join();
    CHECK_FALSE(error);
    error = thread3.join();
    CHECK_FALSE(error);
    error = thread4.join();
    CHECK_FALSE(error);
    error = thread5.join();
    CHECK_FALSE(error);

    std::set<std::uint32_t> uniqueIds;
    for (auto id : ids)
        uniqueIds.insert(id);

    CHECK(uniqueIds.size() == cThreadsCount);
}
