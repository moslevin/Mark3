/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   kernel++.cpp 

    \brief  Test harness

*/

//---------------------------------------------------------------------------
#include "mark3.h"
#include "kernelaware.h"
#include "../unit_test.h"
#include "../ut_platform.h"

//---------------------------------------------------------------------------
// Global objects
static ProfileTimer clProfiler100m;      //!< Profiling timer
static ProfileTimer clProfiler10m;       //!< Profiling timer
static ProfileTimer clProfiler1m;        //!< Profiling timer
static ProfileTimer clProfiler1;
static ProfileTimer clProfiler2;
static ProfileTimer clProfiler3;
static MessageQueue clMsgQ;              //!< Message Queue for timers

//---------------------------------------------------------------------------

void MemSet( void *pvData_, unsigned char u8Value_, unsigned short u16Count_ )
{
    unsigned char *pu8Data = (unsigned char*)pvData_;
    unsigned short i;
    for (i = 0; i < u16Count_; i++)
    {
        pu8Data[i]= u8Value_;
    }
}

static Timer clTimer;
static Timer clTimer1;
static Timer clTimer2;
static Timer clTimer3;
static Semaphore clTimerSem;
//---------------------------------------------------------------------------
void TCallback( Thread *pclOwner_, void *data_ )
{
    clTimerSem.Post();
}

//---------------------------------------------------------------------------
static void TCallbackMulti1( Thread *pclOwner_, void *data_ )
{
    // Send a message to a queue
    Message *pclMsg = GlobalMessagePool::Pop();
    pclMsg->SetCode(0);
    clMsgQ.Send(pclMsg);
}
//---------------------------------------------------------------------------
static void TCallbackMulti2( Thread *pclOwner_, void *data_ )
{
    // Send a message to a queue
    Message *pclMsg = GlobalMessagePool::Pop();
    pclMsg->SetCode(1);
    clMsgQ.Send(pclMsg);
}
//---------------------------------------------------------------------------
static void TCallbackMulti3( Thread *pclOwner_, void *data_ )
{
    // Send a message to a queue
    Message *pclMsg = GlobalMessagePool::Pop();
    pclMsg->SetCode(2);
    clMsgQ.Send(pclMsg);
}

volatile uint32_t aulDelta[3];
//---------------------------------------------------------------------------
TEST(ut_timer_sanity_multi)
{
    uint32_t i;
    clProfiler1m.Init();
    clProfiler10m.Init();
    clProfiler100m.Init();
    clProfiler1.Init();
    clProfiler2.Init();
    clProfiler3.Init();

    clTimerSem.Init(0, 1);

    clMsgQ.Init();

    Profiler::Init();
    Profiler::Start();

    // use prime numbers for extra random interaction.
    clTimer1.Start(true, 7, TCallbackMulti1, NULL);
    clProfiler1.Start();
    clTimer2.Start(true, 13, TCallbackMulti2, NULL);
    clProfiler2.Start();
    clTimer3.Start(true, 19, TCallbackMulti3, NULL);
    clProfiler3.Start();
    
    bool bDone = false;
    for (i = 0; i < 10000; i++)
    {
        Message *pclMsg;
        pclMsg = clMsgQ.Receive();
        switch  (pclMsg->GetCode()) 
        {
            case 0:
                clProfiler1.Stop();
                aulDelta[0] = clProfiler1.GetCurrent() * 8;
                clProfiler1.Start();
                if ((aulDelta[0] < (6 * 16000L)) || (aulDelta[0] > (8 * 16000L)))
                {
                    EXPECT_TRUE(0);
                    bDone = true;
                }
                break;
            case 1:
                clProfiler2.Stop();
                aulDelta[1] = clProfiler2.GetCurrent() * 8;
                clProfiler2.Start();
                if ((aulDelta[1] < (12 * 16000L)) || (aulDelta[1] > (14*16000L)))
                {
                    EXPECT_TRUE(0);
                    bDone = true;
                }
                break;
            case 2:
                clProfiler3.Stop();
                aulDelta[2] = clProfiler3.GetCurrent() * 8;
                clProfiler3.Start();
                if ((aulDelta[2] < (18 * 16000L)) || (aulDelta[2] > (20*16000L)))
                {
                    EXPECT_TRUE(0);
                    bDone = true;
                }
                break;
            default:
                break;    
        }
        GlobalMessagePool::Push(pclMsg);
        if (bDone)
        {
            break;
        }
    }
    
    if (!bDone)
    {
        EXPECT_TRUE(1);
    }

    clTimer1.Stop();
    clTimer2.Stop();
    clTimer3.Stop();
    clProfiler1.Stop();
    clProfiler2.Stop();
    clProfiler3.Stop();
}
TEST_END
//---------------------------------------------------------------------------
volatile uint32_t u32Delta = 0;

TEST(ut_timer_sanity_precision)
{
    clProfiler1m.Init();
    clProfiler10m.Init();
    clProfiler100m.Init();
    clProfiler1.Init();
    clProfiler2.Init();
    clProfiler3.Init();

    clTimerSem.Init(0, 1);

    clMsgQ.Init();

    Profiler::Init();
    Profiler::Start();

    uint32_t i;
    bool bPass = true;
    // 1ms repeated counter
    clTimer.Start(true, 1, TCallback, NULL);
    for (i = 0; i < 10000; i++)
    {
        clProfiler1m.Start();

        clTimerSem.Pend();
        clProfiler1m.Stop();
        u32Delta = clProfiler1m.GetCurrent() * 8;
        if ((u32Delta < 12000) || (u32Delta > 20000))
        {
            // Write error...
            bPass = false;
            break;
        }
    }
    clTimer.Stop();
    EXPECT_TRUE(bPass);
    
    bPass = true;
    // 10ms repeated counter
    clTimer.Start(true, 10, TCallback, NULL);
    for (i = 0; i < 1000; i++)
    {
        clProfiler10m.Start();
        clTimerSem.Pend();
        clProfiler10m.Stop();
        u32Delta = clProfiler10m.GetCurrent() * 8;
        if ((u32Delta < 155000) || (u32Delta > 165000))
        {
            // Write error...
            bPass = false;
            break;
        }
    }
    clTimer.Stop();
    EXPECT_TRUE(bPass);
    bPass = true;

    // 100ms repeated counter
    clTimer.Start(true, 100, TCallback, NULL);
    for (i = 0; i < 100; i++)
    {
        clProfiler100m.Start();
        clTimerSem.Pend();
        clProfiler100m.Stop();
        u32Delta = clProfiler100m.GetCurrent() * 8;
        if ((u32Delta < 1595000) || (u32Delta > 1605000))
        {
            // Write error...
            bPass = false;
            break;
        }
    }
    clTimer.Stop();
    EXPECT_TRUE(bPass);
}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
  TEST_CASE(ut_timer_sanity_precision),
  TEST_CASE(ut_timer_sanity_multi),
TEST_CASE_END

