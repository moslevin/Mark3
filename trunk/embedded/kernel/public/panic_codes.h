/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file panic_codes.h

    \brief Define and enumerate the possible causes of a kernel panic.
*/

#ifndef __PANIC_CODES_H
#define __PANIC_CODES_H

#define PANIC_ASSERT_FAILED         (1)
#define PANIC_LIST_UNLINK_FAILED    (2)
#define PANIC_STACK_SLACK_VIOLATED  (3)
#define PANIC_PEND_LOCK_VIOLATION   (4)
#define PANIC_EVENT_LOCK_VIOLATION  (5)
#define PANIC_MUTEX_LOCK_VIOLATION  (6)

#endif // __PANIC_CODES_H

