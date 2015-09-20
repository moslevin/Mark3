/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/

#ifndef __UT_PLATFORM_H__
#define __UT_PLATFORM_H__

#include "mark3.h"
#include "kerneltypes.h"
#include "kernel.h"
#include "unit_test.h"

//---------------------------------------------------------------------------
#if KERNEL_USE_IDLE_FUNC
#define STACK_SIZE_APP      (300)	//!< Size of the main app's stack
#else
#define STACK_SIZE_APP      (192)
#endif

#define STACK_SIZE_IDLE		(192)	//!< Size of the idle thread stack

//---------------------------------------------------------------------------
#define UART_SIZE_RX		(12)	//!< UART RX Buffer size
#define UART_SIZE_TX		(12)	//!< UART TX Buffer size

//---------------------------------------------------------------------------
class MyUnitTest : public UnitTest
{
public:
    void PrintTestResult();
};

//---------------------------------------------------------------------------
typedef void (*TestFunc)(void);

//---------------------------------------------------------------------------
typedef struct
{
    const char *szName;
    MyUnitTest *pclTestCase;
    TestFunc pfTestFunc;
} MyTestCase;

//---------------------------------------------------------------------------
#define TEST(x)        \
static MyUnitTest TestObj_##x;   \
static void TestFunc_##x(void) \
{ \
    MyUnitTest *pclCurrent = &TestObj_##x;

#define TEST_END \
    pclCurrent->Complete(); \
    pclCurrent->PrintTestResult(); \
}

//---------------------------------------------------------------------------
#define EXPECT_TRUE(x)      pclCurrent->Start(); pclCurrent->ExpectTrue(x)
#define EXPECT_FALSE(x)     pclCurrent->Start(); pclCurrent->ExpectFalse(x)
#define EXPECT_EQUALS(x,y)  pclCurrent->Start(); pclCurrent->ExpectEquals((int32_t)(x), (int32_t)(y))
#define EXPECT_GT(x,y)      pclCurrent->Start(); pclCurrent->ExpectGreaterThan((int32_t)(x), (int32_t)(y))
#define EXPECT_LT(x,y)      pclCurrent->Start(); pclCurrent->ExpectLessThan((int32_t)(x), (int32_t)(y))
#define EXPECT_GTE(x,y)     pclCurrent->Start(); pclCurrent->ExpectGreaterThanEquals((int32_t)(x), (int32_t)(y))
#define EXPECT_LTE(x,y)     pclCurrent->Start(); pclCurrent->ExpectLessThanEquals((int32_t)(x), (int32_t)(y))

//---------------------------------------------------------------------------
#define EXPECT_FAIL_TRUE(x)      pclCurrent->Start(); pclCurrent->ExpectFailTrue(x)
#define EXPECT_FAIL_FALSE(x)     pclCurrent->Start(); pclCurrent->ExpectFailFalse(x)
#define EXPECT_FAIL_EQUALS(x,y)  pclCurrent->Start(); pclCurrent->ExpectFailEquals((int32_t)(x), (int32_t)(y))
#define EXPECT_FAIL_GT(x,y)      pclCurrent->Start(); pclCurrent->ExpectFailGreaterThan((int32_t)(x), (int32_t)(y))
#define EXPECT_FAIL_LT(x,y)      pclCurrent->Start(); pclCurrent->ExpectFailLessThan((int32_t)(x), (int32_t)(y))
#define EXPECT_FAIL_GTE(x,y)     pclCurrent->Start(); pclCurrent->ExpectFailGreaterThanEquals((int32_t)(x), (int32_t)(y))
#define EXPECT_FAIL_LTE(x,y)     pclCurrent->Start(); pclCurrent->ExpectFailLessThanEquals((int32_t)(x), (int32_t)(y))

//---------------------------------------------------------------------------
#define TEST_NAME_EVALUATE(name)       #name
#define TEST_NAME_STRINGIZE(name)      TEST_NAME_EVALUATE(name)

//---------------------------------------------------------------------------
#define TEST_CASE_START MyTestCase astTestCases[] = {
#define TEST_CASE(x)    { TEST_NAME_STRINGIZE(x), &TestObj_##x, TestFunc_##x }
#define TEST_CASE_END   { 0, 0, 0 } };

//---------------------------------------------------------------------------
extern MyTestCase astTestCases[];
extern void run_tests();

//---------------------------------------------------------------------------
void PrintString(const char *szStr_);


#endif //__UT_PLATFORM_H__

