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
#include "mark3.h"

/*===========================================================================

Lab Example 8:  using messages for IPC.

In this example, we present a typical asynchronous producer/consumer pattern
using Mark3's message-driven IPC.


Lessons covered in this example include:
- use of Message and MessageQueue objects to send data between threads
- use of GlobalMessagePool to allocate and free message objects

Takeaway:

Unlike cases presented in previous examples that relied on semaphores or
event flags, messages carry substantial context, specified in its "code" and
"data" members.  This mechanism can be used to pass data between threads
extremely efficiently, with a simple and flexible API.  Any number of threads
can write to/block on a single message queue, which give this method of
IPC even more flexibility.

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) {}
void DebugPrint(const char* szString_);
}

namespace
{
using namespace Mark3;
//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE (PORT_KERNEL_DEFAULT_STACK_SIZE)
Thread clApp1Thread;
K_WORD awApp1Stack[APP1_STACK_SIZE];
void   App1Main(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE (PORT_KERNEL_DEFAULT_STACK_SIZE)
Thread clApp2Thread;
K_WORD awApp2Stack[APP2_STACK_SIZE];
void   App2Main(void* unused_);

//---------------------------------------------------------------------------
// idle thread -- do nothing
Thread clIdleThread;
K_WORD awIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
void   IdleMain(void* /*unused_*/)
{
    while (1) {}
}

//---------------------------------------------------------------------------
MessageQueue clMsgQ;

#define MESSAGE_POOL_SIZE (3)
MessagePool s_clMessagePool;
Message     s_clMessages[MESSAGE_POOL_SIZE];

//---------------------------------------------------------------------------
void App1Main(void* unused_)
{
    auto u16Data = 0;
    while (1) {
        // This thread grabs a message from the global message pool, sets a
        // code-value and the message data pointer, then sends the message to
        // a message queue object.  Another thread (Thread2) is blocked, waiting
        // for a message to arrive in the queue.

        // Get the message object
        auto* pclMsg = s_clMessagePool.Pop();

        // Set the message object's data (contrived in this example)
        pclMsg->SetCode(0x1337);
        u16Data++;
        pclMsg->SetData(&u16Data);

        // Send the message to the shared message queue
        clMsgQ.Send(pclMsg);

        // Wait before sending another message.
        Thread::Sleep(200);
    }
}

//---------------------------------------------------------------------------
void App2Main(void* unused_)
{
    while (1) {
        // This thread waits until it receives a message on the shared global
        // message queue.  When it gets the message, it prints out information
        // about the message's code and data, before returning the messaage object
        // back to the global message pool.  In a more practical application,
        // the user would typically use the code to tell the receiving thread
        // what kind of message was sent, and what type of data to expect in the
        // data field.

        // Wait for a message to arrive on the specified queue.  Note that once
        // this thread receives the message, it is "owned" by the thread, and
        // must be returned back to its source message pool when it is no longer
        // needed.
        auto* pclMsg = clMsgQ.Receive();

        // We received a message, now print out its information
        Kernel::DebugPrint("Received Message\n");
        // KernelAware::Trace(0, __LINE__, pclMsg->GetCode(), *((uint16_t*)pclMsg->GetData()));

        // Done with the message, return it back to the global message queue.
        s_clMessagePool.Push(pclMsg);
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

    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, 0);
    clIdleThread.Start();

    clApp1Thread.Init(awApp1Stack, sizeof(awApp1Stack), 1, App1Main, 0);
    clApp2Thread.Init(awApp2Stack, sizeof(awApp2Stack), 1, App2Main, 0);

    clApp1Thread.Start();
    clApp2Thread.Start();

    clMsgQ.Init();

    s_clMessagePool.Init();
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++) {
        s_clMessages[i].Init();
        s_clMessagePool.Push(&s_clMessages[i]);
    }

    Kernel::Start();

    return 0;
}
