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
/*!
    \file mark3.h

    \brief Single include file given to users of the Mark3 Kernel API

*/

#ifndef __MARK3_H__
#define __MARK3_H__

#include "mark3cfg.h"
#include "kerneltypes.h"

#include "threadport.h"
#include "kernelswi.h"
#include "kerneltimer.h"
#include "kernelprofile.h"

#include "kernel.h"
#include "thread.h"
#include "timerlist.h"

#include "ksemaphore.h"
#include "mutex.h"
#include "eventflag.h"
#include "message.h"
#include "notify.h"
#include "mailbox.h"

#include "atomic.h"
#include "driver.h"

#include "kernelaware.h"

#include "profile.h"
#include "autoalloc.h"

#endif
