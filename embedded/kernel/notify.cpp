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

    \file   notify.cpp

    \brief  Lightweight thread notification - blocking object

*/


#include "mark3cfg.h"
#include "notify.h"

#if KERNEL_USE_NOTIFY
//---------------------------------------------------------------------------
void TimedNotify_Callback(Thread *pclOwner_, void *pvData_)
{
    Notify *pclNotify = static_cast<Notify*>(pvData_);

    // Indicate that the semaphore has expired on the thread
    pclOwner_->SetExpired(true);

    // Wake up the thread that was blocked on this semaphore.
    pclNotify->WakeMe(pclOwner_);

    if (pclOwner_->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority())
    {
        Thread::Yield();
    }
}

//---------------------------------------------------------------------------
void Notify::Init(void)
{
    m_clBlockList.Init();
}

//---------------------------------------------------------------------------
void Notify::Signal(void)
{
    bool bReschedule = false;

    CS_ENTER();
    Thread *pclCurrent = (Thread*)m_clBlockList.GetHead();
    while (pclCurrent != NULL)
    {
        UnBlock(pclCurrent);
        if ( !bReschedule &&
           ( pclCurrent->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority() ) )
        {
            bReschedule = true;
        }
        pclCurrent = (Thread*)m_clBlockList.GetHead();
    }
    CS_EXIT();

    if (bReschedule)
    {
        Thread::Yield();
    }
}

//---------------------------------------------------------------------------
void Notify::Wait( bool *pbFlag_ )
{
    CS_ENTER();
    Block(g_pclCurrent);
    if (pbFlag_)
    {
        *pbFlag_ = false;
    }
    CS_EXIT();

    Thread::Yield();
    if (pbFlag_)
    {
        *pbFlag_ = true;
    }
}

//---------------------------------------------------------------------------
#if KERNEL_USE_TIMEOUTS
bool Notify::Wait( uint32_t u32WaitTimeMS_, bool *pbFlag_ )
{
    bool bUseTimer = false;
    Timer clNotifyTimer;

    CS_ENTER();
    if (u32WaitTimeMS_)
    {
        bUseTimer = true;
        g_pclCurrent->SetExpired(false);

        clNotifyTimer.Init();
        clNotifyTimer.Start(0, u32WaitTimeMS_, TimedNotify_Callback, (void*)this);
    }

    Block(g_pclCurrent);

    if (pbFlag_)
    {
        *pbFlag_ = false;
    }
    CS_EXIT();

    Thread::Yield();

    if (bUseTimer)
    {
        clNotifyTimer.Stop();
        return (g_pclCurrent->GetExpired() == 0);
    }

    if (pbFlag_)
    {
        *pbFlag_ = true;
    }

    return true;
}
#endif
//---------------------------------------------------------------------------
void Notify::WakeMe(Thread *pclChosenOne_)
{
    UnBlock(pclChosenOne_);
}

#endif
