/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3.h"
#include "memutil.h"

/*===========================================================================

Lab Example 9:  Dynamic Threading

Lessons covered in this example include:
- Creating, pausing, and destorying dynamically-created threads at runtime

Takeaway:

In addition to being able to specify a static set of threads during system
initialization, Mark3 gives the user the ability to create and manipu32ate
threads at runtime.  These threads can act as "temporary workers" that can
be activated when needed, without impacting the responsiveness of the rest
of the application.

===========================================================================*/

extern "C" {
void __cxa_pure_virtual(void)
{
}
void DebugPrint(const char* szString_);
}

namespace {
using namespace Mark3;

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-poi   nt
// function used by the application thread.
Thread clApp1Thread;
K_WORD awApp1Stack[PORT_KERNEL_DEFAULT_STACK_SIZE];
void App1Main(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread stack data for a thread that we'll create
// dynamically.
K_WORD awApp2Stack[PORT_KERNEL_DEFAULT_STACK_SIZE];

//---------------------------------------------------------------------------
// idle thread -- do nothing
Thread clIdleThread;
K_WORD awIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
void IdleMain(void* /*unused_*/) {while (1) {} }

#define MAX_THREADS (10)
Thread*  apclActiveThreads[10];
uint32_t au16ActiveTime[10];

void PrintThreadSlack(void)
{
    Kernel::DebugPrint("Stack Slack");
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] != 0) {
            char szStr[10];

            auto u16Slack = apclActiveThreads[i]->GetStackSlack();
            MemUtil::DecimalToHex((K_ADDR)apclActiveThreads[i], szStr);
            Kernel::DebugPrint(szStr);
            Kernel::DebugPrint(" ");
            MemUtil::DecimalToString(u16Slack, szStr);
            Kernel::DebugPrint(szStr);
            Kernel::DebugPrint("\n");
        }
    }
}

void PrintCPUUsage(void)
{
    Kernel::DebugPrint("Cpu usage\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] != 0) {
            //KernelAware::Trace(0, __LINE__, (K_ADDR)apclActiveThreads[i], au16ActiveTime[i]);
        }
    }
}

void ThreadCreate(Thread* pclThread_)
{
    Kernel::DebugPrint("TC\n");
    CS_ENTER();
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] == 0) {
            apclActiveThreads[i] = pclThread_;
            break;
        }
    }
    CS_EXIT();

    PrintThreadSlack();
    PrintCPUUsage();
}

void ThreadExit(Thread* pclThread_)
{
    Kernel::DebugPrint("TX\n");
    CS_ENTER();
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] == pclThread_) {
            apclActiveThreads[i] = 0;
            au16ActiveTime[i]    = 0;
            break;
        }
    }
    CS_EXIT();

    PrintThreadSlack();
    PrintCPUUsage();
}

void ThreadContextSwitch(Thread* pclThread_)
{
    Kernel::DebugPrint("CS\n");
    static uint16_t u16LastTick = 0;
    auto u16Ticks    = KernelTimer::Read();

    CS_ENTER();
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] == pclThread_) {
            au16ActiveTime[i] += u16Ticks - u16LastTick;
            break;
        }
    }
    CS_EXIT();

    u16LastTick = u16Ticks;
}

//---------------------------------------------------------------------------
void WorkerMain1(void* arg_)
{
    auto* pclSem   = static_cast<Semaphore*>(arg_);
    uint32_t   u32Count = 0;

    // Do some work.  Post a semaphore to notify the other thread that the
    // work has been completed.
    while (u32Count < 1000000) {
        u32Count++;
    }

    Kernel::DebugPrint("Worker1 -- Done Work\n");
    pclSem->Post();

    // Work is completed, just spin now.  Let another thread destory u16.
    while (1) {
    }
}
//---------------------------------------------------------------------------
void WorkerMain2(void* arg_)
{
    uint32_t u32Count = 0;
    while (u32Count < 1000000) {
        u32Count++;
    }

    Kernel::DebugPrint("Worker2 -- Done Work\n");

    // A dynamic thread can self-terminate as well:
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void App1Main(void* unused_)
{
    Thread    clMyThread;
    Semaphore clMySem;

    clMySem.Init(0, 1);
    while (1) {
        // Example 1 - create a worker thread at our current priority in order to
        // parallelize some work.
        clMyThread.Init(awApp2Stack, sizeof(awApp2Stack), 1, WorkerMain1, (void*)&clMySem);
        clMyThread.Start();

        // Do some work of our own in parallel, while the other thread works on its project.
        uint32_t u32Count = 0;
        while (u32Count < 100000) {
            u32Count++;
        }

        Kernel::DebugPrint("Thread -- Done Work\n");

        PrintThreadSlack();

        // Wait for the other thread to finish its job.
        clMySem.Pend();

        // Once the thread has signalled u16, we can safely call "Exit" on the thread to
        // remove it from scheduling and recycle it later.
        clMyThread.Exit();

        // Spin the thread up again to do something else in parallel.  This time, the thread
        // will run completely asynchronously to this thread.
        clMyThread.Init(awApp2Stack, sizeof(awApp2Stack), 1, WorkerMain2, 0);
        clMyThread.Start();

        u32Count = 0;
        while (u32Count < 1000000) {
            u32Count++;
        }

        Kernel::DebugPrint("Thread -- Done Work\n");

        // Check that we're sure the worker thread has terminated before we try running the
        // test loop again.
        while (clMyThread.GetState() != ThreadState::Exit) {
        }

        Kernel::DebugPrint("  Test Done\n");
        Thread::Sleep(1000);
        PrintThreadSlack();
    }
}
} // anonymous namespace

using namespace Mark3;
//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();
    Kernel::SetDebugPrintFunction(DebugPrint);

    Kernel::SetThreadCreateCallout(ThreadCreate);
    Kernel::SetThreadExitCallout(ThreadExit);
    Kernel::SetThreadContextSwitchCallout(ThreadContextSwitch);

    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, 0);
    clIdleThread.Start();

    clApp1Thread.Init(awApp1Stack, sizeof(awApp1Stack), 1, App1Main, 0);
    clApp1Thread.Start();
    Kernel::Start();

    return 0;
}
