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

    \file   notify.h

    \brief  Lightweight thread notification - blocking object

*/

#ifndef __NOTIFY_H__
#define __NOTIFY_H__

#include "mark3cfg.h"
#include "blocking.h"

#if KERNEL_USE_NOTIFY

class Notify : public BlockingObject
{
public:
    void Init(void);

    void Signal(void);

    void Wait( bool *pbFlag_ );

#if KERNEL_USE_TIMEOUTS
    bool Wait( K_ULONG ulWaitTimeMS_, bool *pbFlag_ );
#endif

    void WakeMe(Thread *pclChosenOne_);
};

#endif

#endif
