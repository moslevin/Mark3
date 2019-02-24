#include "kerneltypes.h"
#include "mark3cfg.h"
#include "kernel.h"
#include "thread.h"
#include "driver.h"
#include "profile.h"
#include "ksemaphore.h"
#include "mutex.h"
#include "message.h"
#include "timerlist.h"
#include "ut_support.h"

extern "C" void __cxa_pure_virtual() {}

namespace
{
using namespace Mark3;

class UnitTest
{
public:
    void SetName(const char* szName_) { m_szName = szName_; }
    void Start() { m_bIsActive = 1; }
    void Pass()
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
K_WORD awMainStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
K_WORD awIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
K_WORD awTestStack1[PORT_KERNEL_DEFAULT_STACK_SIZE];

//---------------------------------------------------------------------------
void IdleMain(void* unused)
{
    while (1) {}
}

//---------------------------------------------------------------------------
// Basic string routines
uint16_t KUtil_Strlen(const char* szStr_)
{
    char*    pcData = (char*)szStr_;
    uint16_t u16Len = 0;

    while (*pcData++) { u16Len++; }
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
void Semaphore_Flyback(void* pvArg_)
{
    auto* pclSem = static_cast<Semaphore*>(pvArg_);
    clSemaphoreFlyback.Start();
    pclSem->Pend();
    clSemaphoreFlyback.Stop();

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void Semaphore_Profiling()
{
    Semaphore clSem;

    uint16_t i;

    clSemInitTimer.Start();
    for (i = 0; i < 1000; i++) { clSem.Init(0, 1000); }
    clSemInitTimer.Stop();

    clSemPostTimer.Start();
    for (i = 0; i < 1000; i++) { clSem.Post(); }
    clSemPostTimer.Stop();

    clSemPendTimer.Start();
    for (i = 0; i < 1000; i++) { clSem.Pend(); }
    clSemPendTimer.Stop();

    clSem.Init(0, 1);
    for (i = 0; i < 1000; i++) {
        clTestThread1.Init(awTestStack1, sizeof(awTestStack1), 2, Semaphore_Flyback, (void*)&clSem);
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

    clMutexInitTimer.Start();
    for (i = 0; i < 1000; i++) { clMutex.Init(); }
    clMutexInitTimer.Stop();

    clMutexClaimTimer.Start();
    for (i = 0; i < 1000; i++) {
        clMutex.Claim();
        clMutex.Release();
    }
    clMutexClaimTimer.Stop();
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
        clTestThread1.Init(awTestStack1, sizeof(awTestStack1), 2, (ThreadEntryFunc)Thread_ProfilingThread, nullptr);
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
        clContextSwitchTimer.Start();
        {
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
        u16Written += pclDriver_->Write(&data[u16Written], (u16Size_ - u16Written));
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
    for (int i = 0; i < 16; i++) { szBuf[i] = 0; }
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
    UnitTestSupport::OnStart();
    auto* pclUART = DriverList::FindByPath("/dev/tty");

    pclUART->Write("START\n", 6);
    ProfileInit();
    for (uint32_t i = 0; i < 100; i++) {
        //---[ API Profiling ]-----------------------------
        ProfileOverhead();
        pclUART->Write(".", 1);
        Semaphore_Profiling();
        pclUART->Write(".", 1);
        Mutex_Profiling();
        pclUART->Write(".", 1);
        Thread_Profiling();
        pclUART->Write(".", 1);
        Scheduler_Profiling();
        pclUART->Write(".", 1);
        pclUART->Write("\r\n", 2);
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

    clMainThread.Init(awMainStack, sizeof(awMainStack), 1, AppMain, nullptr);

    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, nullptr);

    clMainThread.Start();
    clIdleThread.Start();

    UnitTestSupport::OnInit();

    Kernel::Start();
    return 0;
}
