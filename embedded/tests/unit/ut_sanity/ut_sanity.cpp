#include "kerneltypes.h"
#include "mark3cfg.h"
#include "kernel.h"
#include "thread.h"
#include "driver.h"
#include "drvUART.h"
#include "profile.h"
#include "kernelprofile.h"
#include "ksemaphore.h"
#include "mutex.h"
#include "message.h"
#include "timerlist.h"
#include "../unit_test.h"
#include "../ut_platform.h"
#include "kernelaware.h"

//---------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
static volatile uint8_t u8TestVal;

//---------------------------------------------------------------------------
static Mutex clMutex;

//---------------------------------------------------------------------------
#define TEST_STACK1_SIZE            (220)
#define TEST_STACK2_SIZE            (220)
#define TEST_STACK3_SIZE            (220)

static MessageQueue clMsgQ1;
static MessageQueue clMsgQ2;

static Thread clTestThread2;
static Thread clTestThread3;
static Thread clTestThread1;

static K_WORD aucTestStack1[TEST_STACK1_SIZE];
static K_WORD aucTestStack2[TEST_STACK2_SIZE];
static K_WORD aucTestStack3[TEST_STACK3_SIZE];

//---------------------------------------------------------------------------
void TestSemThread(Semaphore *pstSem_)
{
    pstSem_->Pend();    
    if (u8TestVal != 0x12)
    {
        u8TestVal = 0xFF;
    }
    else
    {
        u8TestVal = 0x21;
    }

    pstSem_->Pend();    
    if (u8TestVal != 0x32)
    {
        u8TestVal = 0xFF;
    }
    else
    {
        u8TestVal = 0x23;
    }

    pstSem_->Pend();

    if (u8TestVal != 0x45)
    {
        u8TestVal = 0xFF;
    }
    else
    {
        u8TestVal = 0x54;
    }
    pstSem_->Pend();
}
//---------------------------------------------------------------------------
// Binary semaphore test:
//  Create a worker thread at a higher priority, which pends on a semaphore
//  that we hold.  The main thread and the new thread alternate pending/posting
//  the semaphore, while modifying/verifying a global variable.
//---------------------------------------------------------------------------
//void UT_SemaphoreTest(void)
TEST(ut_sanity_sem)
{
    Semaphore clSemaphore;

    clSemaphore.Init(0, 1);

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 1, (ThreadEntry_t)TestSemThread, (void*)&clSemaphore);
    clTestThread1.Start();

    u8TestVal = 0x12;

    clSemaphore.Post();    

    EXPECT_EQUALS(u8TestVal, 0x21);

    u8TestVal = 0x32;
    clSemaphore.Post();

    EXPECT_EQUALS(u8TestVal, 0x23);

    u8TestVal = 0x45;
    clSemaphore.Post();

    EXPECT_EQUALS(u8TestVal, 0x54);

    clTestThread1.Stop();
    clTestThread1.Exit();

    clSemaphore.Init(0,1);

}
TEST_END

//---------------------------------------------------------------------------
void TimedSemaphoreThread_Short( Semaphore *pstSem_ )
{
    Scheduler::GetCurrentThread()->Sleep(10);
    pstSem_->Post();
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void TimedSemaphoreThread_Long( Semaphore *pstSem_ )
{
    Scheduler::GetCurrentThread()->Sleep(20);
    pstSem_->Post();
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
//void UT_TimedSemaphoreTest(void)
TEST(ut_sanity_timed_sem)
{
    Semaphore clSem;

    Scheduler::GetCurrentThread()->SetPriority(3);
    
    clSem.Init(0,1);
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TimedSemaphoreThread_Short, (void*)&clSem );
    clTestThread1.Start();
    
// Test 1 - block on a semaphore, wait on thread that will post before expiry

    EXPECT_TRUE(clSem.Pend(15));
    
// Test 2 - block on a semaphore, wait on thread that will post after expiry
    clSem.Init(0,1);
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TimedSemaphoreThread_Long, (void*)&clSem );
    clTestThread1.Start();
        
    EXPECT_FALSE(clSem.Pend(15));

    Scheduler::GetCurrentThread()->SetPriority(1);
}
TEST_END

//---------------------------------------------------------------------------

void TestSleepThread(void *pvArg_)
{
    while(1)
    {
        u8TestVal = 0xAA;
    }
}

//---------------------------------------------------------------------------
// Sleep Test
//  Verify that thread sleeping works as expected.  Check that the lower
//  priority thread is able to execute, setting the global variable to a
//  target value.
//---------------------------------------------------------------------------
//void UT_SleepTest(void)
TEST(ut_sanity_sleep)
{
    Scheduler::GetCurrentThread()->SetPriority(3);
    
    u8TestVal = 0x00;
    
    // Create a lower-priority thread that sets the test value to a known
    // cookie.
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestSleepThread, NULL);
    clTestThread1.Start();
    
    // Sleep, when we wake up check the test value
    Thread::Sleep(5);

    EXPECT_EQUALS(u8TestVal, 0xAA);

    clTestThread1.Exit();
    
    Scheduler::GetCurrentThread()->SetPriority(1);
    
}
TEST_END

//---------------------------------------------------------------------------
void TestMutexThread(Mutex *pclMutex_)
{
    pclMutex_->Claim();
    
    if (u8TestVal != 0xDC)
    {
        u8TestVal = 0xAA;
    }
    else
    {
        u8TestVal = 0xAC;
    }
    pclMutex_->Release();

    Scheduler::GetCurrentThread()->Exit();
}


//---------------------------------------------------------------------------
void TestTimedMutexThreadShort(Mutex *pclMutex_)
{
    pclMutex_->Claim();
    Scheduler::GetCurrentThread()->Sleep(10);
    pclMutex_->Release();

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void TestTimedMutexThreadLong(Mutex *pclMutex_)
{
    pclMutex_->Claim();
    Scheduler::GetCurrentThread()->Sleep(20);
    pclMutex_->Release();

    Scheduler::GetCurrentThread()->Exit();
}


//---------------------------------------------------------------------------
// Mutex test
//  Create a mutex and claim it.  While the mutex is owned, create a new
//  thread at a higher priority, which tries to claim the mutex itself.
//  use a global variable to verify that the threads do not proceed outside
//  of the control.
//---------------------------------------------------------------------------
//void UT_MutexTest(void)
TEST(ut_sanity_mutex)
{
    clMutex.Init();

    u8TestVal = 0x10;
    clMutex.Claim();

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestMutexThread, (void*)&clMutex );
    clTestThread1.Start();
    
    u8TestVal = 0xDC;
    
    clMutex.Release();

    EXPECT_EQUALS(u8TestVal, 0xAC);

    clMutex.Init();
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestTimedMutexThreadShort, (void*)&clMutex );
    clTestThread1.Start();
    
    EXPECT_TRUE(clMutex.Claim(15));

    clMutex.Init();

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestTimedMutexThreadLong, (void*)&clMutex );
    clTestThread1.Start();
    
    EXPECT_FALSE(clMutex.Claim(15));
}
TEST_END

//---------------------------------------------------------------------------
void TimedMessageThread(MessageQueue *pclMsgQ_)
{
    Message *pclMsg = GlobalMessagePool::Pop();
    
    pclMsg->SetData(0);
    pclMsg->SetCode(0);
    
    Scheduler::GetCurrentThread()->Sleep(10);
    
    pclMsgQ_->Send(pclMsg);
    
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
//void UT_TimedMessageTest(void)
TEST(ut_sanity_timed_msg)
{
    Message *pclMsg;

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TimedMessageThread, (void*)&clMsgQ1);
    clTestThread1.Start();
    
    pclMsg = clMsgQ1.Receive(15);
    
    if (!pclMsg)
    {
        EXPECT_TRUE(0);
    }
    else
    {
        EXPECT_TRUE(1);
        GlobalMessagePool::Push(pclMsg);
    }

    pclMsg = clMsgQ1.Receive(10);
    
    if (pclMsg)
    {
        EXPECT_TRUE(0);
        GlobalMessagePool::Push(pclMsg);
    }
    else
    {
        EXPECT_TRUE(1);
    }
}
TEST_END

//---------------------------------------------------------------------------
void TestMessageTest(void *pvArg)
{
    Message *pclMesg;
    bool bPass = true;

    pclMesg = clMsgQ2.Receive();

    if (pclMesg->GetCode() != 0x11)
    {
        bPass = false;
    }

    GlobalMessagePool::Push(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    
    pclMesg->SetCode(0x22);
    
    clMsgQ1.Send(pclMesg);
    
    pclMesg = clMsgQ2.Receive();
    
    if(pclMesg->GetCode() != 0xAA)
    {
        bPass = false;
    }

    GlobalMessagePool::Push(pclMesg);
    
    pclMesg = clMsgQ2.Receive();

    if(pclMesg->GetCode() != 0xBB)
    {
        bPass = false;
    }

    GlobalMessagePool::Push(pclMesg);

    pclMesg = clMsgQ2.Receive();


    if(pclMesg->GetCode() != 0xCC)
    {
        bPass = false;
    }

    GlobalMessagePool::Push(pclMesg);

    pclMesg = GlobalMessagePool::Pop();
    if (bPass)
    {
        pclMesg->SetCode(0xDD);
    }
    else
    {
        pclMesg->SetCode(0xFF);
    }
    clMsgQ1.Send(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    if (bPass)
    {
        pclMesg->SetCode(0xEE);
    }
    else
    {
        pclMesg->SetCode(0x00);
    }
    clMsgQ1.Send(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    if (bPass)
    {
        pclMesg->SetCode(0xFF);
    }
    else
    {
        pclMesg->SetCode(0x11);
    }
    clMsgQ1.Send(pclMesg);
    
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
// Message test
//  Create a second thread that we communicate with by passing messages.
//  Ensure that messages being passed between threads are received as expected
//  in the correct FIFO order, and have the correct IDs.
//---------------------------------------------------------------------------
//void UT_MessageTest(void)
TEST(ut_sanity_msg)
{
    clMsgQ1.Init();
    clMsgQ2.Init();    
    
    Message *pclMesg;

    pclMesg = GlobalMessagePool::Pop();

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestMessageTest, NULL);
    clTestThread1.Start();
    Thread::Yield();
    
    pclMesg->SetCode(0x11);
    pclMesg->SetData(NULL);
    
    clMsgQ2.Send(pclMesg);
    
    pclMesg = clMsgQ1.Receive();
    
    EXPECT_EQUALS(pclMesg->GetCode(), 0x22);

    GlobalMessagePool::Push(pclMesg);

    pclMesg = GlobalMessagePool::Pop();
    pclMesg->SetCode(0xAA);
    clMsgQ2.Send(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    pclMesg->SetCode(0xBB);
    clMsgQ2.Send(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    pclMesg->SetCode(0xCC);
    clMsgQ2.Send(pclMesg);
    
    pclMesg = clMsgQ1.Receive();
    EXPECT_EQUALS(pclMesg->GetCode(), 0xDD);

    GlobalMessagePool::Push(pclMesg);

    pclMesg = clMsgQ1.Receive();
    EXPECT_EQUALS(pclMesg->GetCode(), 0xEE);

    GlobalMessagePool::Push(pclMesg);
    
    pclMesg = clMsgQ1.Receive();
    EXPECT_EQUALS(pclMesg->GetCode(), 0xFF);

    GlobalMessagePool::Push(pclMesg);
        
}
TEST_END

//---------------------------------------------------------------------------
void TestRRThread(volatile uint32_t *pu32Counter_)
{
    while (1)
    {
        (*pu32Counter_)++;
    }
}

//---------------------------------------------------------------------------
// Round-Robin Thread
//  Create 3 threads in the same priority group (lower than our thread), and
//  set their quantums to different values.  Verify that the ratios of their
//  "work cycles" are close to equivalent.
//---------------------------------------------------------------------------

//void UT_RoundRobinTest(void)
TEST(ut_sanity_rr)
{
    volatile uint32_t u32Counter1 = 0;
    volatile uint32_t u32Counter2 = 0;
    volatile uint32_t u32Counter3 = 0;
    uint32_t u32Delta;

    Scheduler::GetCurrentThread()->SetPriority(3);
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter1 );
    clTestThread2.Init(aucTestStack2, TEST_STACK2_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter2 );
    clTestThread3.Init(aucTestStack3, TEST_STACK3_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter3 );
    
    clTestThread1.Start();
    clTestThread2.Start();
    clTestThread3.Start();    
    
    // Sleep for a while to let the other threads execute
    Thread::Sleep(120);  // Must be modal to the worker thread quantums

    if (u32Counter1 > u32Counter2)
    {
        u32Delta = u32Counter1 - u32Counter2;
    }
    else
    {
        u32Delta = u32Counter2 - u32Counter1;
    }

    // Give or take...
    EXPECT_FALSE(u32Delta > u32Counter1/2);

    if (u32Counter1 > u32Counter3)
    {
        u32Delta = u32Counter1 - u32Counter3;
    }
    else
    {
        u32Delta = u32Counter3 - u32Counter1;
    }

    // Give or take...
    EXPECT_FALSE(u32Delta > u32Counter1/2);

    clTestThread1.Exit();
    clTestThread2.Exit();
    clTestThread3.Exit();
    
    Scheduler::GetCurrentThread()->SetPriority(1);
}
TEST_END

//---------------------------------------------------------------------------
//void UT_QuantumTest(void)
TEST(ut_sanity_quantum)
{
    volatile uint32_t u32Counter1 = 0;
    volatile uint32_t u32Counter2 = 0;
    volatile uint32_t u32Counter3 = 0;
    uint32_t u32Delta;

    Scheduler::GetCurrentThread()->SetPriority(3);
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter1 );
    clTestThread2.Init(aucTestStack2, TEST_STACK2_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter2 );
    clTestThread3.Init(aucTestStack3, TEST_STACK3_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter3 );
    
    clTestThread1.SetQuantum(10);
    clTestThread2.SetQuantum(20);
    clTestThread3.SetQuantum(30);
    
    clTestThread1.Start();
    clTestThread2.Start();
    clTestThread3.Start();    
    
    // Sleep for a while to let the other threads execute
    Thread::Sleep(180);  // Must be modal to the worker thread quantums

    // Kill the worker threads
    u32Counter2 /= 2;
    u32Counter3 /= 3;
    
    if (u32Counter1 > u32Counter2)
    {
        u32Delta = u32Counter1 - u32Counter2;
    }
    else
    {
        u32Delta = u32Counter2 - u32Counter1;
    }

    // Give or take...
    EXPECT_FALSE(u32Delta > u32Counter1/2);

    if (u32Counter1 > u32Counter3)
    {
        u32Delta = u32Counter1 - u32Counter3;
    }
    else
    {
        u32Delta = u32Counter3 - u32Counter1;
    }

    // Give or take...
    EXPECT_FALSE(u32Delta > u32Counter1/2);

    clTestThread1.Exit();
    clTestThread2.Exit();
    clTestThread3.Exit();
    
    Scheduler::GetCurrentThread()->SetPriority(1);
}
TEST_END

void TimerTestCallback(Thread *pclOwner_, void *pvData_)
{
    u8TestVal++;
}

//void UT_TimerTest(void)
TEST(ut_sanity_timer)
{
    Timer clTimer;

    u8TestVal = 0;

    clTimer.Init();
	//! Callback should fire just once in 3ms.
    clTimer.Start(1, 2, TimerTestCallback, NULL);
    
    Thread::Sleep(3);
    EXPECT_EQUALS(u8TestVal, 1);

    u8TestVal = 0;
    clTimer.Stop();

	//! Callback will fire every 10ms (use a higher resolution, since a tick-based timer will have a 
	//! fairly high jitter for low-count timers).  Not a problem with tickless-mode, however.
    clTimer.Start(1, 10, TimerTestCallback, NULL);
    
    Thread::Sleep(100);
    
    EXPECT_GTE(u8TestVal, 9);

    clTimer.Stop();
}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
  TEST_CASE(ut_sanity_sem),
  TEST_CASE(ut_sanity_timed_sem),
  TEST_CASE(ut_sanity_sleep),
  TEST_CASE(ut_sanity_mutex),
  TEST_CASE(ut_sanity_msg),
  TEST_CASE(ut_sanity_timed_msg),
  TEST_CASE(ut_sanity_rr),
  TEST_CASE(ut_sanity_quantum),
  TEST_CASE(ut_sanity_timer),
TEST_CASE_END
