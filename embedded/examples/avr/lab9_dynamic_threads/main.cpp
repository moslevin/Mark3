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
#if !KERNEL_USE_IDLE_FUNC
#error "This demo requires KERNEL_USE_IDLE_FUNC"
#endif

extern "C" {
void __cxa_pure_virtual(void)
{
}
}

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-poi   nt
// function used by the application thread.
#define APP1_STACK_SIZE (400 / sizeof(K_WORD))
static Thread clApp1Thread;
static K_WORD awApp1Stack[APP1_STACK_SIZE];
static void App1Main(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread stack data for a thread that we'll create
// dynamically.
#define APP2_STACK_SIZE (400 / sizeof(K_WORD))
static K_WORD awApp2Stack[APP2_STACK_SIZE];

#if KERNEL_USE_THREAD_CALLOUTS
#define MAX_THREADS (10)
static Thread* apclActiveThreads[10];
static uint32_t au16ActiveTime[10];

static void PrintThreadSlack(void)
{
    KernelAware::Print("Stack Slack");
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] != 0) {
            char szStr[10];

            uint16_t u16Slack = apclActiveThreads[i]->GetStackSlack();
            MemUtil::DecimalToHex((K_ADDR)apclActiveThreads[i], szStr);
            KernelAware::Print(szStr);
            KernelAware::Print(" ");
            MemUtil::DecimalToString(u16Slack, szStr);
            KernelAware::Print(szStr);
            KernelAware::Print("\n");
        }
    }
}

static void PrintCPUUsage(void)
{
    KernelAware::Print("Cpu usage\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] != 0) {
            KernelAware::Trace(0, __LINE__, (K_ADDR)apclActiveThreads[i], au16ActiveTime[i]);
        }
    }
}

static void ThreadCreateCallout(Thread* pclThread_)
{
    KernelAware::Print("TC\n");
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

static void ThreadExitCallout(Thread* pclThread_)
{
    KernelAware::Print("TX\n");
    CS_ENTER();
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (apclActiveThreads[i] == pclThread_) {
            apclActiveThreads[i] = 0;
            au16ActiveTime[i] = 0;
            break;
        }
    }
    CS_EXIT();

    PrintThreadSlack();
    PrintCPUUsage();
}

static void ThreadContextSwitchCallback(Thread* pclThread_)
{
    KernelAware::Print("CS\n");
    static uint16_t u16LastTick = 0;
    uint16_t u16Ticks = KernelTimer::Read();

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

#endif

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    Kernel::SetThreadCreateCallout(ThreadCreateCallout);
    Kernel::SetThreadExitCallout(ThreadExitCallout);
    Kernel::SetThreadContextSwitchCallout(ThreadContextSwitchCallback);

    clApp1Thread.Init(awApp1Stack, sizeof(awApp1Stack), 1, App1Main, 0);
    clApp1Thread.Start();
    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
static void WorkerMain1(void* arg_)
{
    Semaphore* pclSem = (Semaphore*)arg_;
    uint32_t u32Count = 0;

    // Do some work.  Post a semaphore to notify the other thread that the
    // work has been completed.
    while (u32Count < 1000000) {
        u32Count++;
    }

    KernelAware::Print("Worker1 -- Done Work\n");
    pclSem->Post();

    // Work is completed, just spin now.  Let another thread destory u16.
    while (1) {
    }
}
//---------------------------------------------------------------------------
static void WorkerMain2(void* arg_)
{
    uint32_t u32Count = 0;
    while (u32Count < 1000000) {
        u32Count++;
    }

    KernelAware::Print("Worker2 -- Done Work\n");

    // A dynamic thread can self-terminate as well:
    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void App1Main(void* unused_)
{
    Thread clMyThread;
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

        KernelAware::Print("Thread -- Done Work\n");

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

        KernelAware::Print("Thread -- Done Work\n");

        // Check that we're sure the worker thread has terminated before we try running the
        // test loop again.
        while (clMyThread.GetState() != THREAD_STATE_EXIT) {
        }

        KernelAware::Print("  Test Done\n");
        Thread::Sleep(1000);
        PrintThreadSlack();
    }
}
