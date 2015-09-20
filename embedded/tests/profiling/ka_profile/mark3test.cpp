
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
#include "kernelaware.h"

extern "C" void __cxa_pure_virtual() { }
//---------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
static volatile uint8_t u8TestVal;

//---------------------------------------------------------------------------
#define TEST_STACK1_SIZE            (384)
#define TEST_STACK2_SIZE            (32)
#define TEST_STACK3_SIZE            (32)
#define MAIN_STACK_SIZE            (384)
#define IDLE_STACK_SIZE            (384)

//---------------------------------------------------------------------------
static Semaphore clSemaphore;
static Mutex clMutex;

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
static void ProfileInit()
{
    
}

//---------------------------------------------------------------------------
static void ProfileOverhead()
{

    CS_ENTER();
    KernelAware::ProfileInit("Overhead");
    KernelAware::ProfileStart();
    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

}

//---------------------------------------------------------------------------
static void Semaphore_Flyback( Semaphore *pclSem_ )
{        
    KernelAware::ProfileInit("SemFlyback");
    KernelAware::ProfileStart();

    pclSem_->Pend();

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
static void Semaphore_Profiling()
{
    Semaphore clSem;

    CS_ENTER();
    KernelAware::ProfileInit("SemInit");
    KernelAware::ProfileStart();

    clSem.Init(0,1000);

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

    CS_ENTER();
    KernelAware::ProfileInit("SemPost");
    KernelAware::ProfileStart();

    clSem.Post();

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

    CS_ENTER();
    KernelAware::ProfileInit("SemPend");
    KernelAware::ProfileStart();

    clSem.Pend();

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

    clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)Semaphore_Flyback, (void*)&clSem);
    clTestThread1.Start();
    clSem.Post();

    return;
}

//---------------------------------------------------------------------------
static void Mutex_Profiling()
{
    uint16_t i;
    Mutex clMutex;
    
    CS_ENTER();
    KernelAware::ProfileInit("MutexInit");
    KernelAware::ProfileStart();

    clMutex.Init();

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

    CS_ENTER();
    KernelAware::ProfileInit("MutexClaim");
    KernelAware::ProfileStart();

    clMutex.Claim();

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

    CS_ENTER();
    KernelAware::ProfileInit("MutexRelease");
    KernelAware::ProfileStart();

    clMutex.Release();

    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    CS_EXIT();

}

//---------------------------------------------------------------------------
static void Thread_ProfilingThread()
{
    // Stop the "thread start" profiling timer, which was started from the 
    // main app thread
    KernelAware::ProfileStop();
    KernelAware::ProfileReport();
    
    // Start the "thread exit" profiling timer, which will be stopped after
    // returning back to the main app thread

    KernelAware::ProfileInit("ThreadExit");
    KernelAware::ProfileStart();
    Scheduler::GetCurrentThread()->Exit();

}

//---------------------------------------------------------------------------
static void Thread_Profiling()
{

        // Profile the amount of time it takes to initialize a representative
        // test thread, simulating an "average" system thread.  Create the 
        // thread at a higher priority than the current thread.
        KernelAware::ProfileInit("ThreadInit");
        CS_ENTER();
        KernelAware::ProfileStart();
        clTestThread1.Init(aucTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntry_t)Thread_ProfilingThread, NULL);
        KernelAware::ProfileStop();
        CS_EXIT();
        KernelAware::ProfileReport();

        // Profile the time it takes from calling "start" to the time when the
        // thread becomes active
        KernelAware::ProfileInit("ThreadStart");

        CS_ENTER();
        KernelAware::ProfileStart();
        clTestThread1.Start(); //-- Switch to the test thread --
        CS_EXIT();

        CS_ENTER();
        // Stop the thread-exit profiling timer, which was started from the
        // test thread
        KernelAware::ProfileStop();
        KernelAware::ProfileReport();
        CS_EXIT();

        Scheduler::SetScheduler(0);
        // Context switch profiling - this is equivalent to what's actually
        // done within the AVR-implementation.
        CS_ENTER();
        KernelAware::ProfileInit("ContextSwitch");
        KernelAware::ProfileStart();
        {
            Thread_SaveContext();
            g_pclNext = g_pclCurrent;
            Thread_RestoreContext();
        }
        KernelAware::ProfileStop();
        KernelAware::ProfileReport();
        CS_EXIT();

        Scheduler::SetScheduler(1);
}

//---------------------------------------------------------------------------
void Scheduler_Profiling()
{

        // Profile the scheduler.  Running at priority 1, we'll get
        // the worst-case scheduling time (not necessarily true of all 
        // schedulers, but true of ours).
    CS_ENTER();
    KernelAware::ProfileInit("Scheduler");
    KernelAware::ProfileStart();
    Scheduler::Schedule();
    KernelAware::ProfileStop();
    CS_EXIT();
    KernelAware::ProfileReport();
}

//---------------------------------------------------------------------------
static void AppMain( void *unused )
{

    ProfileInit();

    while(1)
    {
        //---[ API Profiling ]-----------------------------
        ProfileOverhead();
        Semaphore_Profiling();
        Mutex_Profiling();
        Thread_Profiling();
        Scheduler_Profiling();
        KernelAware::ExitSimulator();
    }
}
