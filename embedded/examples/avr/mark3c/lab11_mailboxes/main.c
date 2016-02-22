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
#include "mark3c.h"

/*===========================================================================

Lab Example 11:  Mailboxes

Lessons covered in this example include:
- Initialize a mailbox for use as an IPC mechanism.
- Create and use mailboxes to pass data between threads.

Takeaway:
- Mailboxes are a powerful IPC mechanism used to pass messages of a fixed-size
  between threads.

===========================================================================*/
#if !KERNEL_USE_IDLE_FUNC
# error "This demo requires KERNEL_USE_IDLE_FUNC"
#endif

//---------------------------------------------------------------------------
#define APP_STACK_SIZE      (256/sizeof(K_WORD))
DECLARE_THREAD(hApp1Thread);
static K_WORD  awApp1Stack[APP_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
DECLARE_THREAD(hApp2Thread);
static K_WORD  awApp2Stack[APP_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
DECLARE_MAILBOX(hMailbox);
static uint8_t au8MBData[100];

typedef struct
{
    uint8_t au8Buffer[10];
} MBType_t;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel_Init();

    // Initialize the threads used in this example
    Thread_Init(hApp1Thread, awApp1Stack,  sizeof(awApp1Stack),  1, App1Main,  0);
    Thread_Start(hApp1Thread);

    Thread_Init(hApp2Thread, awApp2Stack,  sizeof(awApp2Stack),  2, App2Main,  0);
    Thread_Start(hApp2Thread);

    // Initialize the mailbox used in this example
    Mailbox_Init(hMailbox, au8MBData, 100, sizeof(MBType_t));

    Kernel_Start();

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
        Mailbox_Receive(hMailbox, &stMsg);
        KernelAware_Trace2(0, __LINE__, stMsg.au8Buffer[0], stMsg.au8Buffer[9]);
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

        KernelAware_Print("Messages Begin\n");

        uint8_t i, j;
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < 10; j++)
            {
                stMsg.au8Buffer[j] = (i * 10) + j;
            }
            Mailbox_Send(hMailbox, &stMsg);
        }

        KernelAware_Print("Messages End\n");
        Thread_Sleep(2000);
    }
}
