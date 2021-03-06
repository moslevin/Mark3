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
/**
    @file mark3.h

    @brief Single include file given to users of the Mark3 Kernel API

*/
#pragma once

#include "mark3cfg.h"
#include "threadport.h"
#include "criticalsection.h"
#include "criticalguard.h"

#include "kerneltypes.h"
#include "kerneldebug.h"

#include "ithreadport.h"
#include "kernelswi.h"
#include "kerneltimer.h"

#include "kernel.h"
#include "thread.h"
#include "timerlist.h"

#include "ksemaphore.h"
#include "mutex.h"
#include "lockguard.h"
#include "eventflag.h"
#include "message.h"
#include "notify.h"
#include "mailbox.h"
#include "readerwriter.h"
#include "condvar.h"

#include "atomic.h"

#include "profile.h"
#include "autoalloc.h"
#include "priomap.h"

#include "threadlist.h"
#include "threadlistlist.h"

#include "schedulerguard.h"

#include "coroutine.h"
#include "colist.h"
#include "cosched.h"

