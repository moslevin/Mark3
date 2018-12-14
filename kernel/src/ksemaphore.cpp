/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**

    @file   ksemaphore.cpp

    @brief  Semaphore Blocking-Object Implemenation

*/

#include "mark3.h"

namespace Mark3
{
namespace
{
    //---------------------------------------------------------------------------
    /**
     * @brief TimedSemaphore_Callback
     *
     * This function is called from the timer-expired context to trigger a timeout
     * on this semphore.  This results in the waking of the thread that generated
     * the semaphore pend call that was not completed in time.
     *
     * @param pclOwner_ Pointer to the thread to wake
     * @param pvData_   Pointer to the semaphore object that the thread is blocked on
     */
    void TimedSemaphore_Callback(Thread* pclOwner_, void* pvData_)
    {
        KERNEL_ASSERT(pclOwner_ != nullptr);
        KERNEL_ASSERT(pvData_ != nullptr);

        auto* pclSemaphore = static_cast<Semaphore*>(pvData_);

        // Indicate that the semaphore has expired on the thread
        pclOwner_->SetExpired(true);

        // Wake up the thread that was blocked on this semaphore.
        pclSemaphore->WakeMe(pclOwner_);

        if (pclOwner_->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority()) {
            Thread::Yield();
        }
    }
} // anonymous namespace

//---------------------------------------------------------------------------
Semaphore::~Semaphore()
{
    // If there are any threads waiting on this object when it goes out
    // of scope, set a kernel panic.
    if (m_clBlockList.GetHead() != nullptr) {
        Kernel::Panic(PANIC_ACTIVE_SEMAPHORE_DESCOPED);
    }
}

//---------------------------------------------------------------------------
void Semaphore::WakeMe(Thread* pclChosenOne_)
{
    KERNEL_ASSERT(pclChosenOne_);
    KERNEL_ASSERT(IsInitialized());

    // Remove from the semaphore waitlist and back to its ready list.
    UnBlock(pclChosenOne_);
}

//---------------------------------------------------------------------------
uint8_t Semaphore::WakeNext()
{
    auto* pclChosenOne = m_clBlockList.HighestWaiter();
    KERNEL_ASSERT(pclChosenOne);

    // Remove from the semaphore waitlist and back to its ready list.
    UnBlock(pclChosenOne);

    // Call a task switch if higher or equal priority thread
    if (pclChosenOne->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority()) {
        return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------
void Semaphore::Init(uint16_t u16InitVal_, uint16_t u16MaxVal_)
{
    KERNEL_ASSERT(!m_clBlockList.GetHead());

    // Copy the paramters into the object - set the maximum value for this
    // semaphore to implement either binary or counting semaphores, and set
    // the initial count.  Clear the wait list for this object.
    m_u16Value    = u16InitVal_;
    m_u16MaxValue = u16MaxVal_;

    SetInitialized();
}

//---------------------------------------------------------------------------
bool Semaphore::Post()
{
    KERNEL_ASSERT(IsInitialized());

    auto bThreadWake = false;
    auto bBail       = false;
    // Increment the semaphore count - we can mess with threads so ensure this
    // is in a critical section.  We don't just disable the scheudler since
    // we want to be able to do this from within an interrupt context as well.
    CS_ENTER();

    // If nothing is waiting for the semaphore
    if (m_clBlockList.GetHead() == nullptr) {
        // Check so see if we've reached the maximum value in the semaphore
        if (m_u16Value < m_u16MaxValue) {
            // Increment the count value
            m_u16Value++;
        } else {
            // Maximum value has been reached, bail out.
            bBail = true;
        }
    } else {
        // Otherwise, there are threads waiting for the semaphore to be
        // posted, so wake the next one (highest priority goes first).
        bThreadWake = (WakeNext() != 0u);
    }
    CS_EXIT();

    // If we weren't able to increment the semaphore count, fail out.
    if (bBail) {
        return false;
    }

    // if bThreadWake was set, it means that a higher-priority thread was
    // woken.  Trigger a context switch to ensure that this thread gets
    // to execute next.
    if (bThreadWake) {
        Thread::Yield();
    }
    return true;
}

//---------------------------------------------------------------------------
bool Semaphore::Pend_i(uint32_t u32WaitTimeMS_)
{
    KERNEL_ASSERT(IsInitialized());

    Timer clSemTimer;
    auto  bUseTimer = false;

    // Once again, messing with thread data - ensure
    // we're doing all of these operations from within a thread-safe context.
    CS_ENTER();

    // Check to see if we need to take any action based on the semaphore count
    if (m_u16Value != 0) {
        // The semaphore count is non-zero, we can just decrement the count
        // and go along our merry way.
        m_u16Value--;
    } else {
        // The semaphore count is zero - we need to block the current thread
        // and wait until the semaphore is posted from elsewhere.
        if (u32WaitTimeMS_ != 0u) {
            g_pclCurrent->SetExpired(false);
            clSemTimer.Init();
            clSemTimer.Start(false, u32WaitTimeMS_, TimedSemaphore_Callback, this);
            bUseTimer = true;
        }
        BlockPriority(g_pclCurrent);

        // Switch Threads immediately
        Thread::Yield();
    }

    CS_EXIT();

    if (bUseTimer) {
        clSemTimer.Stop();
        return (static_cast<int>(g_pclCurrent->GetExpired()) == 0);
    }
    return true;
}

//---------------------------------------------------------------------------
// Redirect the untimed pend API to the timed pend, with a null timeout.
void Semaphore::Pend()
{
    Pend_i(0);
}

//---------------------------------------------------------------------------
bool Semaphore::Pend(uint32_t u32WaitTimeMS_)
{
    return Pend_i(u32WaitTimeMS_);
}

//---------------------------------------------------------------------------
uint16_t Semaphore::GetCount()
{
    KERNEL_ASSERT(IsInitialized());

    uint16_t u16Ret;
    CS_ENTER();
    u16Ret = m_u16Value;
    CS_EXIT();
    return u16Ret;
}
} // namespace Mark3
