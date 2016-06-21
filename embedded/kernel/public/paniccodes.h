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
/*!
    \file paniccodes.h

    \brief Defines the reason codes thrown when a kernel panic occurs
*/

#ifndef __PANIC_CODES_H
#define __PANIC_CODES_H

#define PANIC_ASSERT_FAILED             (1)
#define PANIC_LIST_UNLINK_FAILED        (2)
#define PANIC_STACK_SLACK_VIOLATED      (3)
#define PANIC_AUTO_HEAP_EXHAUSTED       (4)
#define PANIC_POWERMAN_EXHAUSTED        (5)
#define PANIC_NO_READY_THREADS          (6)
#define PANIC_RUNNING_THREAD_DESCOPED   (7)
#define PANIC_ACTIVE_SEMAPHORE_DESCOPED (8)
#define PANIC_ACTIVE_MUTEX_DESCOPED     (9)
#define PANIC_ACTIVE_EVENTFLAG_DESCOPED (10)
#define PANIC_ACTIVE_NOTIFY_DESCOPED    (11)
#define PANIC_ACTIVE_MAILBOX_DESCOPED   (12)
#define PANIC_ACTIVE_TIMER_DESCOPED     (13)

#endif // __PANIC_CODES_H

