
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

extern "C" void __cxa_pure_virtual() { }
//---------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

class UnitTest
{
public:
    void SetName( const char *szName_ ) { m_szName = szName_; }    
    void Start() { m_bIsActive = 1; }
    
    void Pass() 
    { 
        if (m_bComplete)
        {
            return;
        }
        
        if (m_bIsActive)
        { 
            m_bIsActive = 0; 
            m_u32Iterations++;
            m_bStatus = 1;
        }        
    }    
    
    void Fail()
    {
        if (m_bComplete)
        {
            return;
        }
        
        if (m_bIsActive)
        {
            m_bIsActive = 0;
            m_u32Iterations++;
            m_bStatus = 0;
        }
    }
    
    void Complete()
    {
        m_bComplete = 1;
    }
    
    const char *GetName(){ return m_szName; }
    
    uint8_t GetResult() { return m_bStatus; }

private:
    const char *m_szName;
    bool m_bIsActive;
    bool m_bComplete;
    bool m_bStatus;
    uint32_t m_u32Iterations;    
};

//---------------------------------------------------------------------------
static volatile uint8_t u8TestVal;

//---------------------------------------------------------------------------
static ATMegaUART clUART;
static uint8_t aucTxBuf[32];

#define PROFILE_TEST 1
#if PROFILE_TEST

//---------------------------------------------------------------------------
#define TEST_STACK1_SIZE            (384)
#define TEST_STACK2_SIZE            (32)
#define TEST_STACK3_SIZE            (32)
#define MAIN_STACK_SIZE            (384)
#define IDLE_STACK_SIZE            (384)

//---------------------------------------------------------------------------
static ProfileTimer clProfileOverhead;

static ProfileTimer clSemInitTimer;
static ProfileTimer clSemPostTimer;
static ProfileTimer clSemPendTimer;

static ProfileTimer clMutexInitTimer;
static ProfileTimer clMutexClaimTimer;
static ProfileTimer clMutexReleaseTimer;

static ProfileTimer clThreadInitTimer;
static ProfileTimer clThreadStartTimer;
static ProfileTimer clThreadExitTimer;
static ProfileTimer clContextSwitchTimer;

static ProfileTimer clSemaphoreFlyback;
static ProfileTimer clSchedulerTimer;
#endif

//---------------------------------------------------------------------------
static Semaphore clSemaphore;
static Mutex clMutex;

#define UNIT_TEST 0
#if UNIT_TEST

//---------------------------------------------------------------------------
#define TEST_STACK1_SIZE            (256)
#define TEST_STACK2_SIZE            (192)
#define TEST_STACK3_SIZE            (192)
#define MAIN_STACK_SIZE            (256)
#define IDLE_STACK_SIZE            (256)

static UnitTest clSemTest;
static UnitTest clSleepTest;
static UnitTest clMutexTest;

static MessageQueue clMsgQ1;
static MessageQueue clMsgQ2;
static UnitTest    clMsgQTest;

static UnitTest clRoundRobinTest;
static UnitTest clQuantumTest;
static UnitTest clTimerTest;

static Thread clTestThread2;
static Thread clTestThread3;

static uint8_t aucTestStack2[TEST_STACK2_SIZE];
static uint8_t aucTestStack3[TEST_STACK3_SIZE];

#endif

//---------------------------------------------------------------------------
static Thread clMainThread;
static Thread clIdleThread;

static Thread clTestThread1;

//---------------------------------------------------------------------------
static uint8_t aucMainStack[MAIN_STACK_SIZE];
static uint8_t aucIdleStack[IDLE_STACK_SIZE];
static uint8_t aucTestStack1[TEST_STACK1_SIZE];

//---------------------------------------------------------------------------
static void AppMain( void *unused );
static void IdleMain( void *unused );

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    clMainThread.Init(  aucMainStack,
                        MAIN_STACK_SIZE,
                        1,                       
                        (ThreadEntry_t)AppMain,
                        NULL );
                        
    clIdleThread.Init(  aucIdleStack,
                        MAIN_STACK_SIZE,
                        0,
                        (ThreadEntry_t)IdleMain,
                        NULL );

    clMainThread.Start();
    clIdleThread.Start();

    clUART.SetName("/dev/tty");
    clUART.Init();
    
    DriverList::Add( &clUART );
    
    Kernel::Start();
}

//---------------------------------------------------------------------------
static void IdleMain( void *unused )
{
    while(1)
    {
#if 1
        // LPM code;
        set_sleep_mode(SLEEP_MODE_IDLE);
        cli();
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
#endif        
    }
}

//---------------------------------------------------------------------------
// Basic string routines
uint16_t KUtil_Strlen( const char *szStr_ )
{
    char *pcData = (char*)szStr_;
    uint16_t u16Len = 0;

    while (*pcData++)
    {
        u16Len++;
    }
    return u16Len;
}

//---------------------------------------------------------------------------
void KUtil_Ultoa( uint32_t u8Data_, char *szText_ )
{
    uint32_t u8Mul;
    uint32_t u8Max;

    // Find max index to print...
    u8Mul = 10;
    u8Max = 1;
    while (( u8Mul < u8Data_ ) && (u8Max < 15))
    {
        u8Max++;
        u8Mul *= 10; 
    }
    
    szText_[u8Max] = 0;
    while (u8Max--)
    {
        szText_[u8Max] = '0' + (u8Data_ % 10);
        u8Data_/=10;
    }
}

#if PROFILE_TEST
//---------------------------------------------------------------------------
static void ProfileInit()
{
    
    clProfileOverhead.Init();
    
    clSemInitTimer.Init();
    clSemPendTimer.Init();    
    clSemPostTimer.Init();
    clSemaphoreFlyback.Init();
    
    clMutexInitTimer.Init();
    clMutexClaimTimer.Init();
    clMutexReleaseTimer.Init();
    
    clThreadExitTimer.Init();
    clThreadInitTimer.Init();
    clThreadStartTimer.Init();
    clContextSwitchTimer.Init();
    
    clSchedulerTimer.Init();
}

//---------------------------------------------------------------------------
static void ProfileOverhead()
{
    uint16_t i;
    for (i = 0; i < 100; i++)
    {
        clProfileOverhead.Start();
        clProfileOverhead.Stop();
    }
}

//---------------------------------------------------------------------------
static void Semaphore_Flyback( Semaphore *pclSem_ )
{    

    clSemaphoreFlyback.Start();
    pclSem_->Pend();
    clSemaphoreFlyback.Stop();
    
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
static void Semaphore_Profiling()
{
    Semaphore clSem;

    uint16_t i;
    
    for (i = 0; i < 100; i++)
    {
        clSemInitTimer.Start();        
        clSem.Init(0, 1000);        
        clSemInitTimer.Stop();
    }

    for (i = 0; i < 100; i++)
    {        
        clSemPostTimer.Start();
        clSem.Post();        
        clSemPostTimer.Stop();    
    }
    
    for (i = 0; i < 100; i++)
    {    
        clSemPendTimer.Start();    
        clSem.Pend();        
        clSemPendTimer.Stop();    
    }
    
    clSem.Init(0, 1);
    for (i = 0; i < 100; i++)
    {
        clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)Semaphore_Flyback, (void*)&clSem);
        clTestThread1.Start();
        
        clSem.Post();
    }
    
    return;
}

//---------------------------------------------------------------------------
static void Mutex_Profiling()
{
    uint16_t i;
    Mutex clMutex;
    
    for (i = 0; i < 10; i++)
    {
        clMutexInitTimer.Start();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutex.Init();
        clMutexInitTimer.Stop();
    }
    
    for (i = 0; i < 100; i++)
    {
        clMutexClaimTimer.Start();
        clMutex.Claim();
        clMutexClaimTimer.Stop();
        
        clMutexReleaseTimer.Start();
        clMutex.Release();
        clMutexReleaseTimer.Stop();
    }
}

//---------------------------------------------------------------------------
static void Thread_ProfilingThread()
{
    // Stop the "thread start" profiling timer, which was started from the 
    // main app thread
    clThreadStartTimer.Stop();
    
    // Start the "thread exit" profiling timer, which will be stopped after
    // returning back to the main app thread
    clThreadExitTimer.Start();
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
static void Thread_Profiling()
{
    uint16_t i;
    
    for (i = 0; i < 100; i++)
    {
        // Profile the amount of time it takes to initialize a representative
        // test thread, simulating an "average" system thread.  Create the 
        // thread at a higher priority than the current thread.
        clThreadInitTimer.Start();
        clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)Thread_ProfilingThread, NULL);
        clThreadInitTimer.Stop();
        
        // Profile the time it takes from calling "start" to the time when the
        // thread becomes active
        clThreadStartTimer.Start();
        
        clTestThread1.Start(); //-- Switch to the test thread --
        
        // Stop the thread-exit profiling timer, which was started from the
        // test thread
         clThreadExitTimer.Stop();         
    }
    
    Scheduler::SetScheduler(0);
    for (i = 0; i < 100; i++)
    {
        // Context switch profiling - this is equivalent to what's actually
        // done within the AVR-implementation.
        clContextSwitchTimer.Start();
        {
            Thread_SaveContext();
            g_pclNext = g_pclCurrent;
            Thread_RestoreContext();
        }
        clContextSwitchTimer.Stop();
    }
    Scheduler::SetScheduler(1);
}

//---------------------------------------------------------------------------
void Scheduler_Profiling()
{
    uint16_t i;
    
    for (i = 0; i < 100; i++)
    {
        // Profile the scheduler.  Running at priority 1, we'll get
        // the worst-case scheduling time (not necessarily true of all 
        // schedulers, but true of ours).
        clSchedulerTimer.Start();
        Scheduler::Schedule();    
        clSchedulerTimer.Stop();
    }    
}

//---------------------------------------------------------------------------
static void PrintWait( Driver *pclDriver_, uint16_t u16Size_, const char *data )
{
    uint16_t u16Written = 0;
    
    while (u16Written < u16Size_)
    {
        u16Written += pclDriver_->Write((u16Size_ - u16Written), (uint8_t*)(&data[u16Written]));
        if (u16Written != u16Size_)
        {
            Thread::Sleep(5);
        }
    }
}

//---------------------------------------------------------------------------
void ProfilePrint( ProfileTimer *pclProfile, const char *szName_ )
{
    Driver *pclUART = DriverList::FindByPath("/dev/tty");
    char szBuf[16];
    uint32_t u32Val = pclProfile->GetAverage() - clProfileOverhead.GetAverage();
    u32Val *= 8;
    for( int i = 0; i < 16; i++ )
    {
        szBuf[i] = 0;
    }
    szBuf[0] = '0';
    
    PrintWait( pclUART, KUtil_Strlen(szName_), szName_ );    
    PrintWait( pclUART, 2, ": " );
    KUtil_Ultoa(u32Val, szBuf);
    PrintWait( pclUART, KUtil_Strlen(szBuf), szBuf );
    PrintWait( pclUART, 1, "\n" );
}

//---------------------------------------------------------------------------
void ProfilePrintResults()
{
    ProfilePrint( &clMutexInitTimer, "MI");
    ProfilePrint( &clMutexClaimTimer, "MC");
    ProfilePrint( &clMutexReleaseTimer, "MR");
    ProfilePrint( &clSemInitTimer, "SI");
    ProfilePrint( &clSemPendTimer, "SPo");
    ProfilePrint( &clSemPostTimer, "SPe");
    ProfilePrint( &clSemaphoreFlyback, "SF");
    ProfilePrint( &clThreadExitTimer, "TE");
    ProfilePrint( &clThreadInitTimer, "TI");
    ProfilePrint( &clThreadStartTimer, "TS");
    ProfilePrint( &clContextSwitchTimer, "CS");
    ProfilePrint( &clSchedulerTimer, "SC");
}

#endif

#if UNIT_TEST

//---------------------------------------------------------------------------
void TestSemThread(Semaphore *pstSem_)
{
    pstSem_->Pend();
    
    if (u8TestVal != 0x12)
    {
        clSemTest.Fail();
    }
    u8TestVal = 0x21;

    pstSem_->Pend();
    if (u8TestVal != 0x32)
    {
        clSemTest.Fail();
    }
    u8TestVal = 0x23;
    
    pstSem_->Pend();
    if (u8TestVal != 0x45)
    {
        clSemTest.Fail();
    }
    u8TestVal = 0x54;

    Scheduler::GetCurrentThread()->Exit();
}
//---------------------------------------------------------------------------
// Binary semaphore test:
//  Create a worker thread at a higher priority, which pends on a semaphore
//  that we hold.  The main thread and the new thread alternate pending/posting
//  the semaphore, while modifying/verifying a global variable.
//---------------------------------------------------------------------------
void UT_SemaphoreTest(void)
{
    clSemaphore.Init(0, 1);
    clSemTest.Start();
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestSemThread, (void*)&clSemaphore);
    clTestThread1.Start();
    
    Thread::Yield();
    
    u8TestVal = 0x12;
    
    clSemaphore.Post();
    
    if (u8TestVal != 0x21)
    {
        clSemTest.Fail();
    }
    
    u8TestVal = 0x32;
    clSemaphore.Post();
    if (u8TestVal != 0x23)
    {
        clSemTest.Fail();
    }
    
    u8TestVal = 0x45;
    clSemaphore.Post();
    if (u8TestVal != 0x54)
    {
        clSemTest.Fail();
    } else {
        clSemTest.Pass();
    }
}
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
void UT_SleepTest(void)
{
    clSleepTest.Start();
    
    Scheduler::GetCurrentThread()->SetPriority(3);
    
    u8TestVal = 0x00;
    
    // Create a lower-priority thread that sets the test value to a known
    // cookie.
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestSleepThread, NULL);
    clTestThread1.Start();
    
    // Sleep, when we wake up check the test value
    Thread::Sleep(5);
    
    if (u8TestVal != 0xAA)
    {
        clSleepTest.Fail();
    } else {
        clSleepTest.Pass();    
    }
        
    clTestThread1.Exit();
    
    Scheduler::GetCurrentThread()->SetPriority(1);
    
}

//---------------------------------------------------------------------------
void TestMutexThread(Mutex *pclMutex_)
{
    pclMutex_->Claim();
    
    if (u8TestVal != 0xDC)
    {
        clMutexTest.Fail();
        
    }
    u8TestVal = 0xAC;
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
void UT_MutexTest(void)
{
    clMutex.Init();
    clMutexTest.Start();
    
    u8TestVal = 0x10;
    clMutex.Claim();

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestMutexThread, (void*)&clMutex );
    clTestThread1.Start();
    
    u8TestVal = 0xDC;
    
    clMutex.Release();
    
    if (u8TestVal != 0xAC)
    {
        clMutexTest.Fail();
    }
    else
    {
        clMutexTest.Pass();
    }
}

//---------------------------------------------------------------------------
void TestMessageTest(void *pvArg)
{
    Thread *pstThis = Scheduler::GetCurrentThread();
    Message *pclMesg;
    
    pclMesg = clMsgQ2.Receive();

    if (pclMesg->GetCode() != 0x11)
    {
        clMsgQTest.Fail();
    }
    
    GlobalMessagePool::Push(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    
    pclMesg->SetCode(0x22);
    
    clMsgQ1.Send(pclMesg);
    
    pclMesg = clMsgQ2.Receive();
    
    if (pclMesg->GetCode() != 0xAA)
    {
        clMsgQTest.Fail();
    }

    GlobalMessagePool::Push(pclMesg);
    
    pclMesg = clMsgQ2.Receive();

    if (pclMesg->GetCode() != 0xBB)
    {
        clMsgQTest.Fail();
    }

    GlobalMessagePool::Push(pclMesg);

    pclMesg = clMsgQ2.Receive();

    if (pclMesg->GetCode() != 0xCC)
    {
        clMsgQTest.Fail();
    }

    GlobalMessagePool::Push(pclMesg);

    pclMesg = GlobalMessagePool::Pop();
    pclMesg->SetCode(0xDD);
    clMsgQ1.Send(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    pclMesg->SetCode(0xEE);
    clMsgQ1.Send(pclMesg);
    
    pclMesg = GlobalMessagePool::Pop();
    pclMesg->SetCode(0xFF);
    clMsgQ1.Send(pclMesg);
    
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
// Message test
//  Create a second thread that we communicate with by passing messages.
//  Ensure that messages being passed between threads are received as expected
//  in the correct FIFO order, and have the correct IDs.
//---------------------------------------------------------------------------
void UT_MessageTest(void)
{
    clMsgQTest.Start();
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
    
    if (pclMesg->GetCode() != 0x22)
    {
        clMsgQTest.Fail();
    }
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
    if (pclMesg->GetCode() != 0xDD)
    {
        clMsgQTest.Fail();
    }
    GlobalMessagePool::Push(pclMesg);

    pclMesg = clMsgQ1.Receive();
    if (pclMesg->GetCode() != 0xEE)
    {
        clMsgQTest.Fail();
    }
    GlobalMessagePool::Push(pclMesg);
    
    pclMesg = clMsgQ1.Receive();
    if (pclMesg->GetCode() != 0xFF)
    {
        clMsgQTest.Fail();
    }
    GlobalMessagePool::Push(pclMesg);
    clMsgQTest.Pass();
}

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
void UT_RoundRobinTest(void)
{
    volatile uint32_t u32Counter1 = 0;
    volatile uint32_t u32Counter2 = 0;
    volatile uint32_t u32Counter3 = 0;
    uint32_t u32Delta;

    clRoundRobinTest.Start();

    Scheduler::GetCurrentThread()->SetPriority(3);
    
    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter1 );
    clTestThread2.Init(aucTestStack2, TEST_STACK2_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter2 );
    clTestThread3.Init(aucTestStack3, TEST_STACK3_SIZE, 2, (ThreadEntry_t)TestRRThread, (void*)&u32Counter3 );
    
    clTestThread1.Start();
    clTestThread2.Start();
    clTestThread3.Start();    
    
    // Sleep for a while to let the other threads execute
    Thread::Sleep(120);  // Must be modal to the worker thread quantums

    // Kill the worker threads
    
    if (u32Counter1 > u32Counter2)
    {
        u32Delta = u32Counter1 - u32Counter2;
    }
    else
    {
        u32Delta = u32Counter2 - u32Counter1;
    }

    // Give or take...
    if (u32Delta > u32Counter1/2)
    {
        clRoundRobinTest.Fail();
    }

    if (u32Counter1 > u32Counter3)
    {
        u32Delta = u32Counter1 - u32Counter3;
    }
    else
    {
        u32Delta = u32Counter3 - u32Counter1;
    }

    // Give or take...
    if (u32Delta > u32Counter1/2)
    {
        clRoundRobinTest.Fail();
    }

    clTestThread1.Exit();
    clTestThread2.Exit();
    clTestThread3.Exit();
    
    clRoundRobinTest.Pass();
    Scheduler::GetCurrentThread()->SetPriority(1);
}

//---------------------------------------------------------------------------
void UT_QuantumTest(void)
{
    volatile uint32_t u32Counter1 = 0;
    volatile uint32_t u32Counter2 = 0;
    volatile uint32_t u32Counter3 = 0;
    uint32_t u32Delta;

    clQuantumTest.Start();

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
    if (u32Delta > u32Counter1/2)
    {
        clQuantumTest.Fail();
    }

    if (u32Counter1 > u32Counter3)
    {
        u32Delta = u32Counter1 - u32Counter3;
    }
    else
    {
        u32Delta = u32Counter3 - u32Counter1;
    }

    // Give or take...
    if (u32Delta > u32Counter1/2)
    {
        clQuantumTest.Fail();
    }

    clTestThread1.Exit();
    clTestThread2.Exit();
    clTestThread3.Exit();
    
    clQuantumTest.Pass();
    Scheduler::GetCurrentThread()->SetPriority(1);
}

void TimerTestCallback(Thread *pclOwner_, void *pvData_)
{
    u8TestVal++;
}

void UT_TimerTest(void)
{
    Timer clTimer;
    
    clTimerTest.Start();
    
    u8TestVal = 0;

    clTimer.Start(1, 2, TimerTestCallback, NULL);
    
    Thread::Sleep(3);
    if (u8TestVal != 1)
    {
        clTimerTest.Fail();
    }
    
    u8TestVal = 0;
    clTimer.Stop();
    
    clTimer.Start(1, 1, TimerTestCallback, NULL);
    
    Thread::Sleep(10);
    
    if (u8TestVal < 10)
    {
        clTimerTest.Fail();
    }
    
    clTimer.Stop();
    clTimerTest.Pass();
}
//---------------------------------------------------------------------------
static void UT_Init()
{
    clTimerTest.SetName("Timer");
    clQuantumTest.SetName("Quantum");
    clRoundRobinTest.SetName("RR");
    clSemTest.SetName("Sem");
    clMutexTest.SetName("Mutex");
    clSleepTest.SetName("Sleep");
    clMsgQTest.SetName("Message");
}

//---------------------------------------------------------------------------
static void UT_Print( UnitTest *pclTest_ )
{
    const char *pass = "PASS";
    const char *fail = "FAIL";
    const char *pcName;

    Driver *pclUART = DriverList::FindByPath("/dev/tty");
    
    pclUART->Write( KUtil_Strlen(pclTest_->GetName()), (uint8_t*)pclTest_->GetName() );
    if( pclTest_->GetResult())
    {
        pcName = pass;
    }
    else
    {
        pcName = fail;
    }
    pclUART->Write( 2, (uint8_t*)": " );
    pclUART->Write( KUtil_Strlen(pcName), (uint8_t*) pcName );
    pclUART->Write( 1, (uint8_t*)"\n" );    
    Thread::Sleep(100);
}

//---------------------------------------------------------------------------
static void UT_PrintResults()
{
    UT_Print(&clTimerTest);
    UT_Print(&clQuantumTest);
    UT_Print(&clRoundRobinTest);
    UT_Print(&clSemTest);
    UT_Print(&clMutexTest);
    UT_Print(&clSleepTest);
    UT_Print(&clMsgQTest);
}
#endif


//---------------------------------------------------------------------------
static void AppMain( void *unused )
{
    Driver *pclUART = DriverList::FindByPath("/dev/tty");
    
#if UNIT_TEST    
    UT_Init();
#endif
    
#if PROFILE_TEST
    ProfileInit();
#endif    

    pclUART->Control(CMD_SET_BUFFERS, NULL, 0, aucTxBuf, 32);
    {
        uint32_t u32BaudRate = 57600;
        pclUART->Control(CMD_SET_BAUDRATE, &u32BaudRate, 0, 0, 0 );
        pclUART->Control(CMD_SET_RX_DISABLE, 0, 0, 0, 0);
    }
    
    pclUART->Open();
    pclUART->Write(6,(uint8_t*)"START\n");

    while(1)
    {
#if UNIT_TEST
        //---[ Behavioral Tests ]--------------------------
        pclUART->Write(1,(uint8_t*)"a");
        UT_SemaphoreTest();
        pclUART->Write(1,(uint8_t*)"B");
        UT_SleepTest();
        pclUART->Write(1,(uint8_t*)"C");
        UT_MutexTest();
        pclUART->Write(1,(uint8_t*)"D");
        UT_MessageTest();
        pclUART->Write(1,(uint8_t*)"E");
        UT_RoundRobinTest();
        pclUART->Write(1,(uint8_t*)"F");
        UT_QuantumTest();
        pclUART->Write(1,(uint8_t*)"G");
        UT_TimerTest();        
        pclUART->Write(1,(uint8_t*)"\n");
        
        UT_PrintResults();
        Thread::Sleep(500);
#endif

#if PROFILE_TEST
        //---[ API Profiling ]-----------------------------
        Profiler::Start();
        ProfileOverhead();        
        Semaphore_Profiling();
        Mutex_Profiling();
        Thread_Profiling();
        Scheduler_Profiling();
        Profiler::Stop();
                
        ProfilePrintResults();
        Thread::Sleep(500);
#endif        
    }
}
