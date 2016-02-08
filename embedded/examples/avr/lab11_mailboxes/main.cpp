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

/*===========================================================================

Lab Example 11:  Mailboxes

Lessons covered in this example include:
- Initialize a mailbox for use as an IPC mechanism.
- Create and use mailboxes to pass data between threads.

Takeaway:
- Mailboxes are a powerful IPC mechanism used to pass messages of a fixed-size
  between threads.

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
#define APP_STACK_SIZE      (256/sizeof(K_WORD))
static Thread  clApp1Thread;
static K_WORD  awApp1Stack[APP_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
static Thread  clApp2Thread;
static K_WORD  awApp2Stack[APP_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
static Mailbox clMailbox;
static uint8_t au8MBData[100];

typedef struct
{
    uint8_t au8Buffer[10];
} MBType_t;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    // Initialize the threads used in this example
    clApp1Thread.Init(  awApp1Stack,  sizeof(awApp1Stack),  1, App1Main,  0);
    clApp1Thread.Start();

    clApp2Thread.Init(  awApp2Stack,  sizeof(awApp2Stack),  2, App2Main,  0);
    clApp2Thread.Start();

    // Initialize the mailbox used in this example
    clMailbox.Init(au8MBData, 100, sizeof(MBType_t));

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    while (1)
    {
        MBType_t stMsg;

        // Wait until there is an envelope available in the shared mailbox, and
        // then log a trace message.
        clMailbox.Receive(&stMsg);
        KernelAware::Trace(0, __LINE__, stMsg.au8Buffer[0], stMsg.au8Buffer[9]);
    }
}

//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    while (1)
    {
        MBType_t stMsg;

        // Place a bunch of envelopes in the mailbox, and then wait for a
        // while.  Note that this thread has a higher priority than the other
        // thread, so it will keep pushing envelopes to the other thread until
        // it gets to the sleep, at which point the other thread will be allowed
        // to execute.

        KernelAware::Print("Messages Begin\n");

        for (uint8_t i = 0; i < 10; i++)
        {
            for (uint8_t j = 0; j < 10; j++)
            {
                stMsg.au8Buffer[j] = (i * 10) + j;
            }
            clMailbox.Send(&stMsg);
        }

        KernelAware::Print("Messages End\n");
        Thread::Sleep(2000);
    }
}
