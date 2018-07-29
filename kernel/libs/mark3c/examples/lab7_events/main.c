/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3c.h"

extern void DebugPrint(const char* szString_);

/*===========================================================================

Lab Example 7: Using Event Flags

Lessons covered in this example include:
-Using the EventFlag Class to synchronize thread execution
-Explore the behavior of the EventFlagOperation::Any_Set and EventFlagOperation::All_Set, and the
 event-mask bitfield.

Takeaway:

Like Semaphores and Mutexes, EventFlag objects can be used to synchronize
the execution of threads in a system.  The EventFlag object allows for many
threads to share the same object, blocking on different event combinations.
This provides an efficient, robust way for threads to process asynchronous
system events that occur with a unified interface.

===========================================================================*/

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE (PORT_KERNEL_DEFAULT_STACK_SIZE)
DECLARE_THREAD(hApp1Thread);
static K_WORD awApp1Stack[APP1_STACK_SIZE];
static void App1Main(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE (PORT_KERNEL_DEFAULT_STACK_SIZE)
DECLARE_THREAD(hApp2Thread);
static K_WORD awApp2Stack[APP2_STACK_SIZE];
static void App2Main(void* unused_);

//---------------------------------------------------------------------------
//
DECLARE_EVENTFLAG(hFlags);

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel_Init();
    Kernel_SetDebugPrintFunction(DebugPrint);

    Thread_Init(hApp1Thread, awApp1Stack, APP1_STACK_SIZE, 1, App1Main, 0);
    Thread_Init(hApp2Thread, awApp2Stack, APP2_STACK_SIZE, 1, App2Main, 0);

    Thread_Start(hApp1Thread);
    Thread_Start(hApp2Thread);

    EventFlag_Init(hFlags);

    Kernel_Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void* unused_)
{
    while (1) {
        uint16_t usFlags;

        // Block this thread until any of the event flags have been set by
        // some outside force (here, we use Thread 2).  As an exercise to the
        // user, try playing around with the event mask to see the effect it
        // has on which events get processed.  Different threads can block on
        // different bitmasks - this allows events with different real-time
        // priorities to be handled in different threads, while still using
        // the same event-flag object.

        // Also note that EventFlagOperation::Any_Set indicates that the thread will be
        // unblocked whenever any of the flags in the mask are selected.  If
        // you wanted to trigger an action that only takes place once multiple
        // bits are set, you could block the thread waiting for a specific
        // event bitmask with EventFlagOperation::All_Set specified.
        usFlags = EventFlag_Wait(hFlags, 0xFFFF, EVENT_FLAG_ALL_SET);

        // Print a message indicaating which bit was set this time.
        switch (usFlags) {
            case 0x0001: Kernel_DebugPrint("Event1\n"); break;
            case 0x0002: Kernel_DebugPrint("Event2\n"); break;
            case 0x0004: Kernel_DebugPrint("Event3\n"); break;
            case 0x0008: Kernel_DebugPrint("Event4\n"); break;
            case 0x0010: Kernel_DebugPrint("Event5\n"); break;
            case 0x0020: Kernel_DebugPrint("Event6\n"); break;
            case 0x0040: Kernel_DebugPrint("Event7\n"); break;
            case 0x0080: Kernel_DebugPrint("Event8\n"); break;
            case 0x0100: Kernel_DebugPrint("Event9\n"); break;
            case 0x0200: Kernel_DebugPrint("Event10\n"); break;
            case 0x0400: Kernel_DebugPrint("Event11\n"); break;
            case 0x0800: Kernel_DebugPrint("Event12\n"); break;
            case 0x1000: Kernel_DebugPrint("Event13\n"); break;
            case 0x2000: Kernel_DebugPrint("Event14\n"); break;
            case 0x4000: Kernel_DebugPrint("Event15\n"); break;
            case 0x8000: Kernel_DebugPrint("Event16\n"); break;
            default: break;
        }

        // Clear the event-flag that we just printed a message about.  This
        // will allow us to acknowledge further events in that bit in the future.
        EventFlag_Clear(hFlags, usFlags);
    }
}

//---------------------------------------------------------------------------
void App2Main(void* unused_)
{
    uint16_t usFlag = 1;
    while (1) {
        Thread_Sleep(100);

        // Event flags essentially map events to bits in a bitmap.  Here we
        // set one bit each 100ms.  In this loop, we cycle through bits 0-15
        // repeatedly.  Note that this will wake the other thread, which is
        // blocked, waiting for *any* of the flags in the bitmap to be set.
        EventFlag_Set(hFlags, usFlag);

        // Bitshift the flag value to the left.  This will be the flag we set
        // the next time this thread runs through its loop.
        if (usFlag != 0x8000) {
            usFlag <<= 1;
        } else {
            usFlag = 1;
        }
    }
}
