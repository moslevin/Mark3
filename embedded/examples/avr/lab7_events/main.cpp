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

Lab Example 7: using Event Flags

Lessons covered in this example include:
-Using the EventFlag Class to synchronize thread execution
-Explore the behavior of the EVENT_FLAG_ANY and EVENT_FLAG_ALL, and the
 event-mask bitfield.

Takeaway:

Like Semaphores and Mutexes, EventFlag objects can be used to synchronize
the execution of threads in a system.  The EventFlag class allows for many
threads to share the same object, blocking on different event combinations.
This provides an efficient, robust way for threads to process asynchronous
system events that occur with a unified interface.

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE      (320/sizeof(K_WORD))
static Thread  clApp1Thread;
static K_WORD  awApp1Stack[APP1_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE      (320/sizeof(K_WORD))
static Thread  clApp2Thread;
static K_WORD  awApp2Stack[APP2_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
//
static EventFlag clFlags;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    clApp1Thread.Init(  awApp1Stack,  APP1_STACK_SIZE,  1, App1Main,  0);
    clApp2Thread.Init(  awApp2Stack,  APP2_STACK_SIZE,  1, App2Main,  0);

    clApp1Thread.Start();
    clApp2Thread.Start();

    clFlags.Init();

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    while(1)
    {
        uint16_t u16Flags;

        // Block this thread until any of the event flags have been set by
        // some outside force (here, we use Thread 2).  As an exercise to the
        // user, try playing around with the event mask to see the effect it
        // has on which events get processed.  Different threads can block on
        // different bitmasks - this allows events with different real-time
        // priorities to be handled in different threads, while still using
        // the same event-flag object.

        // Also note that EVENT_FLAG_ANY indicates that the thread will be
        // unblocked whenever any of the flags in the mask are selected.  If
        // you wanted to trigger an action that only takes place once multiple
        // bits are set, you could block the thread waiting for a specific
        // event bitmask with EVENT_FLAG_ALL specified.
        u16Flags = clFlags.Wait(0xFFFF, EVENT_FLAG_ANY);

        // Print a message indicaating which bit was set this time.
        switch (u16Flags)
        {
        case 0x0001:
            KernelAware::Print("Event1\n");
            break;
        case 0x0002:
            KernelAware::Print("Event2\n");
            break;
        case 0x0004:
            KernelAware::Print("Event3\n");
            break;
        case 0x0008:
            KernelAware::Print("Event4\n");
            break;
        case 0x0010:
            KernelAware::Print("Event5\n");
            break;
        case 0x0020:
            KernelAware::Print("Event6\n");
            break;
        case 0x0040:
            KernelAware::Print("Event7\n");
            break;
        case 0x0080:
            KernelAware::Print("Event8\n");
            break;
        case 0x0100:
            KernelAware::Print("Event9\n");
            break;
        case 0x0200:
            KernelAware::Print("Event10\n");
            break;
        case 0x0400:
            KernelAware::Print("Event11\n");
            break;
        case 0x0800:
            KernelAware::Print("Event12\n");
            break;
        case 0x1000:
            KernelAware::Print("Event13\n");
            break;
        case 0x2000:
            KernelAware::Print("Event14\n");
            break;
        case 0x4000:
            KernelAware::Print("Event15\n");
            break;
        case 0x8000:
            KernelAware::Print("Event16\n");
            break;
        default:
            break;
        }

        // Clear the event-flag that we just printed a message about.  This
        // will allow u16 to acknowledge further events in that bit in the future.
        clFlags.Clear(u16Flags);

    }
}

//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    uint16_t u16Flag = 1;
    while(1)
    {
        Thread::Sleep(100);

        // Event flags essentially map events to bits in a bitmap.  Here we
        // set one bit each 100ms.  In this loop, we cycle through bits 0-15
        // repeatedly.  Note that this will wake the other thread, which is
        // blocked, waiting for *any* of the flags in the bitmap to be set.
        clFlags.Set(u16Flag);

        // Bitshift the flag value to the left.  This will be the flag we set
        // the next time this thread runs through its loop.
        if (u16Flag != 0x8000)
        {
            u16Flag <<= 1;
        }
        else
        {
            u16Flag = 1;
        }
    }
}
