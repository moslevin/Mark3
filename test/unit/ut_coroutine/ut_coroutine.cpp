/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"

#include "mark3.h"

namespace Mark3
{
//===========================================================================
// Local Defines
//===========================================================================

//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_coroutine_priority)
{
    // arrange
    auto cr = Coroutine{};
    cr.Init(2, nullptr, nullptr);

    // act
    auto prio1 = cr.GetPriority();
    cr.SetPriority(PORT_COROUTINE_PRIORITIES - 1);
    auto prio2 = cr.GetPriority();
    cr.SetPriority(0);
    auto prio3 = cr.GetPriority();

    // assert
    EXPECT_EQUALS(prio1, 2);
    EXPECT_EQUALS(prio2, PORT_COROUTINE_PRIORITIES - 1);
    EXPECT_EQUALS(prio3, 0);
}

TEST(ut_coroutine_schedule_empty)
{
    // arrange + act
    CoScheduler::Init();
    auto cr = CoScheduler::Schedule();

    // assert
    EXPECT_EQUALS(cr, nullptr);
}

TEST(ut_coroutine_schedule_active)
{
    // arrange
    CoScheduler::Init();
    auto cr = Coroutine{};
    auto count = 0;

    static constexpr auto callback = [](Coroutine* cr, void* context) {
        auto* counter = static_cast<int*>(context);
        (*counter)++;
    };

    // act
    cr.Init(1, callback, &count);
    auto* initCr = CoScheduler::GetCoList(1)->GetHead();
    cr.Activate();
    auto* secondCr = CoScheduler::GetCoList(1)->GetHead();
    auto* firstSched = CoScheduler::Schedule();
    firstSched->Run();
    auto* finalCr = CoScheduler::GetCoList(1)->GetHead();

    // assert
    EXPECT_EQUALS(initCr, nullptr);
    EXPECT_EQUALS(secondCr, &cr);
    EXPECT_EQUALS(firstSched, &cr);
    EXPECT_EQUALS(finalCr, nullptr);
    EXPECT_EQUALS(count, 1);
}

TEST(ut_coroutine_schedule_multiple)
{
    // arrange
    CoScheduler::Init();
    auto crLow = Coroutine{};
    auto crMed = Coroutine{};
    auto crHigh = Coroutine{};

    static constexpr auto callback = [](Coroutine* cr, void* context) {
    };

    crHigh.Init(3, callback, nullptr);
    crMed.Init(2, callback, nullptr);
    crLow.Init(1, callback, nullptr);

    crMed.Activate();
    crLow.Activate();
    crHigh.Activate();

    // act
    auto* firstCr = CoScheduler::Schedule();
    firstCr->Run();
    auto* secondCr = CoScheduler::Schedule();
    secondCr->Run();
    auto* thirdCr = CoScheduler::Schedule();
    thirdCr->Run();
    auto* lastCr = CoScheduler::Schedule();

    // assert
    EXPECT_EQUALS(firstCr, &crHigh);
    EXPECT_EQUALS(secondCr, &crMed);
    EXPECT_EQUALS(thirdCr, &crLow);
    EXPECT_EQUALS(lastCr, nullptr);
}

TEST(ut_coroutine_activate_in_task)
{
    // arrange
    CoScheduler::Init();
    auto cr = Coroutine{};
    static auto count = 0;

    static constexpr auto callback = [](Coroutine* cr, void* context) {
        static bool first = true;
        if (first) {
            first = false;
            cr->Activate();
        }
        count++;
    };

    // act
    cr.Init(1, callback, &count);
    cr.Activate();
    auto* firstSched = CoScheduler::Schedule();
    firstSched->Run();
    auto* secondSched = CoScheduler::Schedule();
    secondSched->Run();
    auto* thirdSched = CoScheduler::Schedule();

    // assert
    EXPECT_EQUALS(firstSched, &cr);
    EXPECT_EQUALS(secondSched, &cr);
    EXPECT_EQUALS(thirdSched, nullptr);
}

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_coroutine_priority),
TEST_CASE(ut_coroutine_schedule_empty),
TEST_CASE(ut_coroutine_schedule_active),
TEST_CASE(ut_coroutine_schedule_multiple),
TEST_CASE(ut_coroutine_activate_in_task),
TEST_CASE_END
} // namespace Mark3
