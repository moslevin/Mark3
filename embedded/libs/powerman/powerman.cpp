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
    \file powerman.cpp
    \brief Power management helper classes (voting mechanisms).
*/
#include <stdint.h>
#include <stdbool.h>

#include "mark3.h"
#include "powerman.h"

//---------------------------------------------------------------------------
void PowerBallot::Register(PowerMan* pclManager_)
{
    m_pclManager = pclManager_;
    m_u8ID = m_pclManager->GetNextID();
}

//---------------------------------------------------------------------------
void PowerBallot::Cast(power_vote_t eVote_)
{
    m_pclManager->ChangeVote(m_u8ID, eVote_);
}

//---------------------------------------------------------------------------
bool PowerMan::CountBallots(void)
{
    bool bCanSleep = false;
    bool bMustWake = false;

    CS_ENTER();
    for (uint16_t i = 0; i < MAX_BALLOTS / 8; i++)
    {
        if (m_au8SleepBallots[i])
        {
            bCanSleep = true;
        }
        else if (m_au8WakeBallots[i])
        {
            bMustWake = true;
            break;
        }
    }
    CS_EXIT();

    if (bMustWake)
    {
        return false;
    }
    if (bCanSleep)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void PowerMan::ChangeVote(uint8_t u8Index_, power_vote_t eVote_)
{
    uint8_t u8Byte = u8Index_ >> 3;
    uint8_t u8BitMask = 1 << (u8Index_ & 0x0007);

    CS_ENTER();
    m_au8SleepBallots[u8Byte] &= ~u8BitMask;
    m_au8WakeBallots[u8Byte]  &= ~u8BitMask;
    if (eVote_ == POWER_VOTE_SLEEP)
    {
        m_au8SleepBallots[u8Byte] &= ~u8BitMask;
    }
    else if (eVote_ == POWER_VOTE_WAKE)
    {
        m_au8WakeBallots[u8Byte] &= ~u8BitMask;
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
uint8_t PowerMan::GetNextID()
{
    if (m_u8ID >= MAX_BALLOTS)
    {
        Kernel::Panic(PANIC_POWERMAN_EXHAUSTED);
    }
    return m_u8ID++;
}
