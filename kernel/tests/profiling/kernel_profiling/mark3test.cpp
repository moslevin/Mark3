#include "kerneltypes.h"
#include "mark3cfg.h"
#include "kernel.h"
#include "thread.h"
#include "driver.h"
#include "drvATMegaUART.h"
#include "profile.h"
#include "kernelprofile.h"
#include "ksemaphore.h"
#include "mutex.h"
#include "message.h"
#include "timerlist.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

extern "C" void __cxa_pure_virtual()
{
}

namespace {
using namespace Mark3;

class UnitTest
{
public:
    void SetName(const char* szName_) { m_szName = szName_; }
    void                     Start() { m_bIsActive = 1; }
    void                     Pass()
    {
        if (m_bComplete) {
            return;
        }

        if (m_bIsActive) {
            m_bIsActive = 0;
            m_u32Iterations++;
            m_bStatus = 1;
        }
    }

    void Fail()
    {
        if (m_bComplete) {
            return;
        }

        if (m_bIsActive) {
            m_bIsActive = 0;
            m_u32Iterations++;
            m_bStatus = 0;
        }
    }

    void        Complete() { m_bComplete = 1; }
    const char* GetName() { return m_szName; }
    uint8_t     GetResult() { return m_bStatus; }
private:
    const char* m_szName;
    bool        m_bIsActive;
    bool        m_bComplete;
    bool        m_bStatus;
    uint32_t    m_u32Iterations;
};

//---------------------------------------------------------------------------
volatile uint8_t u8TestVal;

//---------------------------------------------------------------------------
ATMegaUART clUART;
uint8_t    aucTxBuf[32];

//---------------------------------------------------------------------------
#define TEST_STACK1_SIZE (384)
#define TEST_STACK2_SIZE (32)
#define TEST_STACK3_SIZE (32)
#define MAIN_STACK_SIZE (384)
#define IDLE_STACK_SIZE (384)

//---------------------------------------------------------------------------
ProfileTimer clProfileOverhead;

ProfileTimer clSemInitTimer;
ProfileTimer clSemPostTimer;
ProfileTimer clSemPendTimer;

ProfileTimer clMutexInitTimer;
ProfileTimer clMutexClaimTimer;
ProfileTimer clMutexReleaseTimer;

ProfileTimer clThreadInitTimer;
ProfileTimer clThreadStartTimer;
ProfileTimer clThreadExitTimer;
ProfileTimer clContextSwitchTimer;

ProfileTimer clSemaphoreFlyback;
ProfileTimer clSchedulerTimer;

//---------------------------------------------------------------------------
Semaphore clSemaphore;
Mutex     clMutex;

//---------------------------------------------------------------------------
Thread clMainThread;
Thread clIdleThread;

Thread clTestThread1;

//---------------------------------------------------------------------------
uint8_t aucMainStack[MAIN_STACK_SIZE];
uint8_t awIdleStack[IDLE_STACK_SIZE];
uint8_t awTestStack1[TEST_STACK1_SIZE];

//---------------------------------------------------------------------------
void IdleMain(void* unused)
{
    while (1) {
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
uint16_t KUtil_Strlen(const char* szStr_)
{
    char*    pcData = (char*)szStr_;
    uint16_t u16Len = 0;

    while (*pcData++) {
        u16Len++;
    }
    return u16Len;
}

//---------------------------------------------------------------------------
void KUtil_Ultoa(uint32_t u8Data_, char* szText_)
{
    uint32_t u8Mul;
    uint32_t u8Max;

    // Find max index to print...
    u8Mul = 10;
    u8Max = 1;
    while ((u8Mul < u8Data_) && (u8Max < 15)) {
        u8Max++;
        u8Mul *= 10;
    }

    szText_[u8Max] = 0;
    while (u8Max--) {
        szText_[u8Max] = '0' + (u8Data_ % 10);
        u8Data_ /= 10;
    }
}

//---------------------------------------------------------------------------
void ProfileInit()
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
void ProfileOverhead()
{
    uint16_t i;
    for (i = 0; i < 100; i++) {
        clProfileOverhead.Start();
        clProfileOverhead.Stop();
    }
}

//---------------------------------------------------------------------------
void Semaphore_Flyback(Semaphore* pclSem_)
{
    clSemaphoreFlyback.Start();
    pclSem_->Pend();
    clSemaphoreFlyback.Stop();

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void Semaphore_Profiling()
{
    Semaphore clSem;

    uint16_t i;

    for (i = 0; i < 100; i++) {
        clSemInitTimer.Start();
        clSem.Init(0, 1000);
        clSemInitTimer.Stop();
    }

    for (i = 0; i < 100; i++) {
        clSemPostTimer.Start();
        clSem.Post();
        clSemPostTimer.Stop();
    }

    for (i = 0; i < 100; i++) {
        clSemPendTimer.Start();
        clSem.Pend();
        clSemPendTimer.Stop();
    }

    clSem.Init(0, 1);
    for (i = 0; i < 100; i++) {
        clTestThread1.Init(awTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntryFunc)Semaphore_Flyback, (void*)&clSem);
        clTestThread1.Start();

        clSem.Post();
    }

    return;
}

//---------------------------------------------------------------------------
void Mutex_Profiling()
{
    uint16_t i;
    Mutex    clMutex;

    for (i = 0; i < 10; i++) {
        clMutexInitTimer.Start();
        clMutex.Init();
        clMutexInitTimer.Stop();
    }

    for (i = 0; i < 100; i++) {
        clMutexClaimTimer.Start();
        clMutex.Claim();
        clMutexClaimTimer.Stop();

        clMutexReleaseTimer.Start();
        clMutex.Release();
        clMutexReleaseTimer.Stop();
    }
}

//---------------------------------------------------------------------------
void Thread_ProfilingThread()
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
void Thread_Profiling()
{
    uint16_t i;

    for (i = 0; i < 100; i++) {
        // Profile the amount of time it takes to initialize a representative
        // test thread, simulating an "average" system thread.  Create the
        // thread at a higher priority than the current thread.
        clThreadInitTimer.Start();
        clTestThread1.Init(awTestStack1, TEST_STACK1_SIZE, 2, (ThreadEntryFunc)Thread_ProfilingThread, NULL);
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
    for (i = 0; i < 100; i++) {
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

    for (i = 0; i < 100; i++) {
        // Profile the scheduler.  Running at priority 1, we'll get
        // the worst-case scheduling time (not necessarily true of all
        // schedulers, but true of ours).
        clSchedulerTimer.Start();
        Scheduler::Schedule();
        clSchedulerTimer.Stop();
    }
}

//---------------------------------------------------------------------------
void PrintWait(Driver* pclDriver_, uint16_t u16Size_, const char* data)
{
    uint16_t u16Written = 0;

    while (u16Written < u16Size_) {
        u16Written += pclDriver_->Write((u16Size_ - u16Written), (uint8_t*)(&data[u16Written]));
        if (u16Written != u16Size_) {
            Thread::Sleep(5);
        }
    }
}

//---------------------------------------------------------------------------
void ProfilePrint(ProfileTimer* pclProfile, const char* szName_)
{
    Driver*  pclUART = DriverList::FindByPath("/dev/tty");
    char     szBuf[16];
    uint32_t u32Val = pclProfile->GetAverage() - clProfileOverhead.GetAverage();
    u32Val *= 8;
    for (int i = 0; i < 16; i++) {
        szBuf[i] = 0;
    }
    szBuf[0] = '0';

    PrintWait(pclUART, KUtil_Strlen(szName_), szName_);
    PrintWait(pclUART, 2, ": ");
    KUtil_Ultoa(u32Val, szBuf);
    PrintWait(pclUART, KUtil_Strlen(szBuf), szBuf);
    PrintWait(pclUART, 1, "\n");
}

//---------------------------------------------------------------------------
void ProfilePrintResults()
{
    ProfilePrint(&clMutexInitTimer, "MI");
    ProfilePrint(&clMutexClaimTimer, "MC");
    ProfilePrint(&clMutexReleaseTimer, "MR");
    ProfilePrint(&clSemInitTimer, "SI");
    ProfilePrint(&clSemPendTimer, "SPo");
    ProfilePrint(&clSemPostTimer, "SPe");
    ProfilePrint(&clSemaphoreFlyback, "SF");
    ProfilePrint(&clThreadExitTimer, "TE");
    ProfilePrint(&clThreadInitTimer, "TI");
    ProfilePrint(&clThreadStartTimer, "TS");
    ProfilePrint(&clContextSwitchTimer, "CS");
    ProfilePrint(&clSchedulerTimer, "SC");
}

//---------------------------------------------------------------------------
void AppMain(void* unused)
{
    UartDriver* pclUART = static_cast<UartDriver*>(DriverList::FindByPath("/dev/tty"));

    ProfileInit();

    pclUART->SetBuffers(NULL, 0, aucTxBuf, 32);
    pclUART->EnableRx(false);
    pclUART->SetBaudRate(57600);

    pclUART->Open();
    pclUART->Write(6, (uint8_t*)"START\n");

    for (uint32_t i = 0; i < 100; i++) {
        //---[ API Profiling ]-----------------------------
        Profiler::Start();
        ProfileOverhead();
        pclUART->Write(1, (uint8_t*)".");
        Semaphore_Profiling();
        pclUART->Write(1, (uint8_t*)".");
        Mutex_Profiling();
        pclUART->Write(1, (uint8_t*)".");
        Thread_Profiling();
        pclUART->Write(1, (uint8_t*)".");
        Scheduler_Profiling();
        pclUART->Write(1, (uint8_t*)".");
        Profiler::Stop();
        pclUART->Write(2, (uint8_t*)"\r\n");
    }
    ProfilePrintResults();
    Thread::Sleep(1000);

    typedef void (*myFunc)(void);
    myFunc reboot = 0;
    reboot();
}
} // anonymous namespace

using namespace Mark3;

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    clMainThread.Init(aucMainStack, MAIN_STACK_SIZE, 1, AppMain, NULL);

    clIdleThread.Init(awIdleStack, IDLE_STACK_SIZE, 0, IdleMain, NULL);

    clMainThread.Start();
    clIdleThread.Start();

    clUART.SetName("/dev/tty");
    clUART.Init();

    DriverList::Add(&clUART);

    Kernel::Start();
}
