/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3c.h"

/*===========================================================================

Lab Example 8:  Using messages for IPC.

In this example, we present a typical asynchronous producer/consumer pattern
using Mark3's message-driven IPC.


Lessons covered in this example include:
- Use of Message and MessageQueue objects to send data between threads
- Use of GlobalMessagePool to allocate and free message objects

Takeaway:

Unlike cases presented in previous examples that relied on semaphores or
event flags, messages carry substantial context, specified in its "code" and
"data" members.  This mechanism can be used to pass data between threads
extremely efficiently, with a simple and flexible API.  Any number of threads
can write to/block on a single message queue, which give this method of
IPC even more flexibility.

===========================================================================*/

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE      (320/sizeof(K_WORD))
DECLARE_THREAD(hApp1Thread);
static K_WORD  awApp1Stack[APP1_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE      (320/sizeof(K_WORD))
DECLARE_THREAD(hApp2Thread);
static K_WORD  awApp2Stack[APP2_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
DECLARE_MESSAGEQUEUE(hMsgQ);

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel_Init();

    Thread_Init( hApp1Thread, awApp1Stack,  APP1_STACK_SIZE,  1, App1Main,  0);
    Thread_Init( hApp2Thread, awApp2Stack,  APP2_STACK_SIZE,  1, App2Main,  0);

    Thread_Start( hApp1Thread );
    Thread_Start( hApp2Thread );

    MessageQueue_Init( hMsgQ );

    Kernel_Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    uint16_t usData = 0;
    while(1)
    {
        // This thread grabs a message from the global message pool, sets a
        // code-value and the message data pointer, then sends the message to
        // a message queue object.  Another thread (Thread2) is blocked, waiting
        // for a message to arrive in the queue.

        // Get the message object
        Message_t hMsg = GlobalMessagePool_Pop();

        // Set the message object's data (contrived in this example)
        Message_SetCode( hMsg, 0x1337);
        usData++;
        Message_SetData( hMsg, &usData);

        // Send the message to the shared message queue
        MessageQueue_Send( hMsgQ, hMsg);

        // Wait before sending another message.
        Thread_Sleep(20);
    }
}

//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    while(1)
    {
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
        Message_t hMsg = MessageQueue_Receive( hMsgQ );

        // We received a message, now print out its information
        KernelAware_Print("Received Message\n");
        KernelAware_Trace2(0, __LINE__, Message_GetCode( hMsg ), *((uint16_t*)Message_GetData( hMsg )));

        // Done with the message, return it back to the global message queue.
        GlobalMessagePool_Push(hMsg);
    }
}
