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

    @file   notify.cpp

    @brief  Lightweight thread notification - blocking object

*/
#include "mark3.h"
namespace Mark3
{
namespace
{
    //---------------------------------------------------------------------------
    void TimedNotify_Callback(Thread* pclOwner_, void* pvData_)
    {
        KERNEL_ASSERT(nullptr != pclOwner_);
        KERNEL_ASSERT(nullptr != pvData_);

        auto* pclNotify = static_cast<Notify*>(pvData_);

        // Indicate that the semaphore has expired on the thread
        pclOwner_->SetExpired(true);

        // Wake up the thread that was blocked on this semaphore.
        pclNotify->WakeMe(pclOwner_);

        if (pclOwner_->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority()) {
            Thread::Yield();
        }
    }
} // anonymous namespace

//---------------------------------------------------------------------------
Notify::~Notify()
{
    // If there are any threads waiting on this object when it goes out
    // of scope, set a kernel panic.
    if (nullptr != m_clBlockList.GetHead()) {
        Kernel::Panic(PANIC_ACTIVE_NOTIFY_DESCOPED);
    }
}

//---------------------------------------------------------------------------
void Notify::Init(void)
{
    KERNEL_ASSERT(!m_clBlockList.GetHead());
    SetInitialized();

    m_bPending = false;
}

//---------------------------------------------------------------------------
void Notify::Signal(void)
{
    KERNEL_ASSERT(IsInitialized());

    auto bReschedule = false;

    { // Begin critical section
        const auto cs = CriticalGuard{};
        auto* pclCurrent = m_clBlockList.GetHead();
        if (nullptr == pclCurrent) {
            m_bPending = true;
        } else {
            while (nullptr != pclCurrent) {
                UnBlock(pclCurrent);
                if (!bReschedule && (pclCurrent->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority())) {
                    bReschedule = true;
                }
                pclCurrent = m_clBlockList.GetHead();
            }
            m_bPending = false;
        }
    } // end critical section

    if (bReschedule) {
        Thread::Yield();
    }
}

//---------------------------------------------------------------------------
void Notify::Wait(bool* pbFlag_)
{
    KERNEL_ASSERT(nullptr != pbFlag_);
    KERNEL_ASSERT(IsInitialized());

    auto bEarlyExit = false;
    { // Begin critical section
        const auto cs = CriticalGuard{};
        if (!m_bPending) {
            Block(g_pclCurrent);
            if (nullptr != pbFlag_) {
                *pbFlag_ = false;
            }
        } else {
            m_bPending = false;
            bEarlyExit = true;
        }
    } // End critical section

    if (bEarlyExit) {
        return;
    }

    Thread::Yield();
    if (nullptr != pbFlag_) {
        *pbFlag_ = true;
    }
}

//---------------------------------------------------------------------------
bool Notify::Wait(uint32_t u32WaitTimeMS_, bool* pbFlag_)
{
    KERNEL_ASSERT(nullptr != pbFlag_);
    KERNEL_ASSERT(IsInitialized());

    auto bUseTimer     = false;
    auto bEarlyExit    = false;
    auto clNotifyTimer = Timer {};

    { // Begin critical section
        const auto cs = CriticalGuard{};
        if (!m_bPending) {
            if (0u != u32WaitTimeMS_) {
                bUseTimer = true;
                g_pclCurrent->SetExpired(false);

                clNotifyTimer.Init();
                clNotifyTimer.Start(false, u32WaitTimeMS_, TimedNotify_Callback, this);
            }

            Block(g_pclCurrent);

            if (nullptr != pbFlag_) {
                *pbFlag_ = false;
            }
        } else {
            m_bPending = false;
            bEarlyExit = true;
        }
    } // end critical section

    if (bEarlyExit) {
        return true;
    }

    Thread::Yield();

    if (bUseTimer) {
        clNotifyTimer.Stop();
        return (g_pclCurrent->GetExpired() == false);
    }

    if (nullptr != pbFlag_) {
        *pbFlag_ = true;
    }

    return true;
}

//---------------------------------------------------------------------------
void Notify::WakeMe(Thread* pclChosenOne_)
{
    KERNEL_ASSERT(nullptr != pclChosenOne_);
    KERNEL_ASSERT(IsInitialized());

    UnBlock(pclChosenOne_);
}
} // namespace Mark3
