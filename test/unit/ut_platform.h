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

#pragma once

#include "mark3.h"
#include "kerneltypes.h"
#include "kernel.h"
#include "unit_test.h"

namespace Mark3
{
//---------------------------------------------------------------------------
class MyUnitTest : public UnitTest
{
public:
    void PrintTestResult();
};

//---------------------------------------------------------------------------
typedef void (*TestFunc)(MyUnitTest* pclCurrent);

//---------------------------------------------------------------------------
typedef struct {
    const char* szName;
    MyUnitTest* pclTestCase;
    TestFunc    pfTestFunc;
} MyTestCase;

//---------------------------------------------------------------------------
#define TEST(x)                                                                                                        \
    static MyUnitTest TestObj_##x;                                                                                     \
    static void       TestFunc_##x(MyUnitTest* pclCurrent)

//---------------------------------------------------------------------------
#define EXPECT_TRUE(x)                                                                                                 \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectTrue(x)
#define EXPECT_FALSE(x)                                                                                                \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFalse(x)
#define EXPECT_EQUALS(x, y)                                                                                            \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectEquals((K_INT)(x), (K_INT)(y))
#define EXPECT_GT(x, y)                                                                                                \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectGreaterThan((K_INT)(x), (K_INT)(y))
#define EXPECT_LT(x, y)                                                                                                \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectLessThan((K_INT)(x), (K_INT)(y))
#define EXPECT_GTE(x, y)                                                                                               \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectGreaterThanEquals((K_INT)(x), (K_INT)(y))
#define EXPECT_LTE(x, y)                                                                                               \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectLessThanEquals((K_INT)(x), (K_INT)(y))

//---------------------------------------------------------------------------
#define EXPECT_FAIL_TRUE(x)                                                                                            \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailTrue(x)
#define EXPECT_FAIL_FALSE(x)                                                                                           \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailFalse(x)
#define EXPECT_FAIL_EQUALS(x, y)                                                                                       \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailEquals((K_INT)(x), (K_INT)(y))
#define EXPECT_FAIL_GT(x, y)                                                                                           \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailGreaterThan((K_INT)(x), (K_INT)(y))
#define EXPECT_FAIL_LT(x, y)                                                                                           \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailLessThan((K_INT)(x), (K_INT)(y))
#define EXPECT_FAIL_GTE(x, y)                                                                                          \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailGreaterThanEquals((K_INT)(x), (K_INT)(y))
#define EXPECT_FAIL_LTE(x, y)                                                                                          \
    pclCurrent->Start();                                                                                               \
    pclCurrent->ExpectFailLessThanEquals((K_INT)(x), (K_INT)(y))

//---------------------------------------------------------------------------
#define TEST_NAME_EVALUATE(name) #name
#define TEST_NAME_STRINGIZE(name) TEST_NAME_EVALUATE(name)

//---------------------------------------------------------------------------
#define TEST_CASE_START MyTestCase astTestCases[] = {
#define TEST_CASE(x)                                                                                                   \
    {                                                                                                                  \
        TEST_NAME_STRINGIZE(x), &TestObj_##x, TestFunc_##x                                                             \
    }
#define TEST_CASE_END                                                                                                  \
    {                                                                                                                  \
        0, 0, 0                                                                                                        \
    }                                                                                                                  \
    }                                                                                                                  \
    ;

//---------------------------------------------------------------------------
extern MyTestCase astTestCases[];
void              run_tests();

//---------------------------------------------------------------------------
void PrintString(const char* szStr_);

} // namespace Mark3
