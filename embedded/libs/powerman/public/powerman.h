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
    \file powerman.h
    \brief Power management helper classes (power-state voting mechanisms)
*/
#ifndef __POWERMAN_H__
#define __POWERMAN_H__

#include <stdint.h>
#include <stdbool.h>

#include "mark3.h"

#define MAX_BALLOTS         (48)

//---------------------------------------------------------------------------
typedef enum {
    POWER_VOTE_NONE,    //!< Abstain from voting
    POWER_VOTE_SLEEP,   //!< Vote to sleep
    POWER_VOTE_WAKE     //!< Vote to stay awake
} power_vote_t;

//---------------------------------------------------------------------------
class PowerMan;
class PowerBallot
{
public:
    /*!
     * \brief Register
     *
     * Assign a ballot with a PowerMan manager, which will be used to keep
     * track of power-state votes associated with the system or part of it.
     */
    void Register(PowerMan* pclManager_);

    /*!
     * \brief Cast
     *
     * Cast a vote or change the vote currently on the ballot.  Note that
     * the system won't re-tally the votes until an appropriate time (i.e.
     * the idle thread).
     *
     * \param eVote_ New vote to hold in the ballot.
     */
    void Cast(power_vote_t eVote_);

private:
    uint8_t   m_u8ID;
    PowerMan* m_pclManager;
};

//---------------------------------------------------------------------------
class PowerMan
{
public:
    /*!
     * \brief CountBallots
     *
     * Tabulate all the ballots, and determine whether or not a system (or
     * part of the system) can enter the sleep state.  The system can only
     * enter sleep if there exist vote(s) in favor of sleeping, but none in
     * favor of staying awake.
     *
     * \return true - system is able to sleep, false - system must stay awake
     */
    bool CountBallots(void);

protected:

    friend class PowerBallot;

    /*!
     * \brief ChangeVote
     *
     * Change the vote held on a specific ballot, indicated by its serial
     * number/index.  The old vote is erased, and the new vote registered.
     *
     * \param u8Index_  Index/serial number of the ballot to modify
     * \param eVote_    New vote status
     */
    void ChangeVote(uint8_t u8Index_, power_vote_t eVote_);

    /*!
     * \brief GetNextID
     *
     * Returns a unique ID representing the index/serial number of the ballot.
     * Note that this will cause a kernel panic if the ballot indexes are
     * exhausted
     *
     * \return The serial number / index of the next ballot
     */
    uint8_t GetNextID();

private:
    uint8_t m_au8SleepBallots[MAX_BALLOTS / 8];   //!< Bitmap indicating "sleep" votes
    uint8_t m_au8WakeBallots[MAX_BALLOTS / 8];    //!< Bitmap indication "wake" votes
    uint8_t m_u8ID;                               //!< Auto-incrementing ID, used to identify ballots
};

#endif //__POWERMAN_H__
