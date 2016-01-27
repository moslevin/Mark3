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
=========================================================================== */
/*!
    \file streamer.h

    \brief Thread/Interrupt-safe byte-based data streaming.
 */

#include "kerneltypes.h"
#include "mark3.h"
#include "streamer.h"

//---------------------------------------------------------------------------
void Streamer::Init(uint8_t *pau8Buffer_, uint16_t u16Size_)
{
    m_u16Head     = 0;
    m_u16Tail     = 0;
    m_u16Size     = u16Size_;
    m_u16Avail    = m_u16Size;
    m_pau8Buffer  = pau8Buffer_;
    m_pu8LockAddr = 0;
}

//---------------------------------------------------------------------------
bool Streamer::Read(uint8_t *pu8Data_)
{
    bool rc = true;

    CS_ENTER();
    if (m_u16Avail == m_u16Size)
    {
        rc = false;
    }
    else
    {
        uint8_t *pu8Dest = &m_pau8Buffer[m_u16Tail];
        if (pu8Dest == m_pu8LockAddr)
        {
            rc = false;
        }
        else
        {
            *pu8Data_ = *pu8Dest;
            m_u16Tail++;
            if (m_u16Tail >= m_u16Size)
            {
                m_u16Tail = 0;
            }
            m_u16Avail++;
        }
    }
    CS_EXIT();

    return rc;
}

//---------------------------------------------------------------------------
bool Streamer::Write(uint8_t u8Data_)
{
    bool rc = true;

    CS_ENTER();
    if (!m_u16Avail)
    {
        rc = false;
    }
    else
    {
        if (m_pu8LockAddr == &m_pau8Buffer[m_u16Head])
        {
            rc = false;
        }
        else
        {
            m_pau8Buffer[m_u16Head] = u8Data_;
            m_u16Head++;
            if (m_u16Head >= m_u16Size)
            {
                m_u16Head = 0;
            }
            m_u16Avail--;
        }
    }
    CS_EXIT();

    return rc;
}

//---------------------------------------------------------------------------
bool Streamer::Claim(uint8_t **pu8Addr_)
{
    bool rc = true;

    CS_ENTER();

    if (!m_u16Avail)
    {
        rc = false;
    }
    else
    {
        if (m_pu8LockAddr == &m_pau8Buffer[m_u16Head])
        {
            rc = false;
        }
        else
        {
            *pu8Addr_ = &m_pau8Buffer[m_u16Head];
            m_u16Head++;
            if (m_u16Head >= m_u16Size)
            {
                m_u16Head = 0;
            }
            m_u16Avail--;
        }
    }

    CS_EXIT();
    return rc;
}

//---------------------------------------------------------------------------
void Streamer::Lock(uint8_t *pu8LockAddr_)
{
    CS_ENTER();
    m_pu8LockAddr = pu8LockAddr_;
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Streamer::Unlock(void)
{
    CS_ENTER();
    m_pu8LockAddr = 0;
    CS_EXIT();
}
