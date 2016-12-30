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
    \file streamer.cpp

    \brief Thread/Interrupt-safe byte-based data streaming.
 */

#include "kerneltypes.h"
#include "mark3.h"
#include "streamer.h"

//---------------------------------------------------------------------------
void Streamer::Init(uint8_t* pau8Buffer_, uint16_t u16Size_)
{
    m_u16Head     = 0;
    m_u16Tail     = 0;
    m_u16Size     = u16Size_;
    m_u16Avail    = m_u16Size;
    m_pau8Buffer  = pau8Buffer_;
    m_pu8LockAddr = 0;
}

//---------------------------------------------------------------------------
bool Streamer::Read(uint8_t* pu8Data_)
{
    bool rc = true;

    CS_ENTER();
    if (m_u16Avail == m_u16Size) {
        rc = false;
    } else {
        uint8_t* pu8Dest = &m_pau8Buffer[m_u16Tail];
        if (pu8Dest == m_pu8LockAddr) {
            rc = false;
        } else {
            *pu8Data_ = *pu8Dest;
            m_u16Tail++;
            if (m_u16Tail >= m_u16Size) {
                m_u16Tail = 0;
            }
            m_u16Avail++;
        }
    }
    CS_EXIT();

    return rc;
}

//---------------------------------------------------------------------------
uint16_t Streamer::Read(uint8_t* pu8Data_, uint16_t u16Len_)
{
    uint16_t u16ToRead;

    if (m_pu8LockAddr) {
        return 0;
    }

    uint16_t u16Allocated;
    uint16_t u16PreWrap;
    uint8_t* pu8Src;
    uint8_t* pu8Dst;

    CS_ENTER();

    u16Allocated = m_u16Size - m_u16Avail;

    if (u16Allocated > u16Len_) {
        u16ToRead = u16Len_;
    } else {
        u16ToRead = u16Allocated;
    }

    u16PreWrap = m_u16Size - m_u16Tail;

    pu8Src = &m_pau8Buffer[m_u16Tail];
    pu8Dst = pu8Data_;

    Lock(pu8Src);

    CS_EXIT();

    if (u16Allocated) {

        if (u16PreWrap >= u16ToRead) {
            for (uint16_t i = 0; i < u16ToRead; i++) {
                *pu8Dst++ = *pu8Src++;
            }
        } else {
            for (uint16_t i = 0; i < u16PreWrap; i++) {
                *pu8Dst++ = *pu8Src++;
            }
            pu8Src = m_pau8Buffer;
            for (uint16_t i = u16PreWrap; i < u16ToRead; i++) {
                *pu8Dst++ = *pu8Src++;
            }
        }
    }

    CS_ENTER();

    m_u16Avail += u16ToRead;
    if (u16PreWrap >= u16ToRead) {
        m_u16Tail += u16ToRead;
    } else {
        m_u16Tail += u16ToRead - m_u16Size;
    }

    Unlock();

    CS_EXIT();
    return u16ToRead;
}

//---------------------------------------------------------------------------
bool Streamer::Write(uint8_t u8Data_)
{
    bool rc = true;

    CS_ENTER();
    if (!m_u16Avail) {
        rc = false;
    } else {
        if (m_pu8LockAddr == &m_pau8Buffer[m_u16Head]) {
            rc = false;
        } else {
            m_pau8Buffer[m_u16Head] = u8Data_;
            m_u16Head++;
            if (m_u16Head >= m_u16Size) {
                m_u16Head = 0;
            }
            m_u16Avail--;
        }
    }
    CS_EXIT();

    return rc;
}

//---------------------------------------------------------------------------
uint16_t Streamer::Write(uint8_t* pu8Data_, uint16_t u16Len_)
{
    uint16_t u16ToWrite;

    // Bail if the buffer is currently locked.
    if (m_pu8LockAddr) {
        return 0;
    }

    // Update the buffer metadata in a critical section, and lock it so that
    // we can safely write to it with interrupts enabled.

    uint16_t u16PreWrap;
    uint8_t* pu8Src;
    uint8_t* pu8Dst;

    CS_ENTER();
    if (m_u16Avail > u16Len_) {
        u16ToWrite = u16Len_;
    } else {
        u16ToWrite = m_u16Avail;
    }

    u16PreWrap = m_u16Size - m_u16Head;

    pu8Src = pu8Data_;
    pu8Dst = &m_pau8Buffer[m_u16Head];

    m_u16Avail -= u16ToWrite;

    if (u16PreWrap >= u16ToWrite) {
        m_u16Head += u16ToWrite;
    } else {
        m_u16Head += u16ToWrite - m_u16Size;
    }

    Lock(pu8Dst);
    CS_EXIT();

    // Perform the buffer writes with interrupts enabled, buffers locked.
    if (u16ToWrite) {
        if (u16PreWrap >= u16ToWrite) {
            for (uint16_t i = 0; i < u16ToWrite; i++) {
                *pu8Dst++ = *pu8Src++;
            }
        } else {
            for (uint16_t i = 0; i < u16PreWrap; i++) {
                *pu8Dst++ = *pu8Src++;
            }
            pu8Dst = m_pau8Buffer;
            for (uint16_t i = u16PreWrap; i < u16ToWrite; i++) {
                *pu8Dst++ = *pu8Src++;
            }
        }
    }

    Unlock();
    return u16ToWrite;
}

//---------------------------------------------------------------------------
bool Streamer::CanRead(void)
{
    bool bRc = true;
    CS_ENTER();
    if (m_u16Avail == m_u16Size)
    {
        bRc = false;
    }
    CS_EXIT();
    return bRc;
}

//---------------------------------------------------------------------------
bool Streamer::CanWrite(void)
{
    bool bRc = false;

    CS_ENTER();
    if (m_u16Avail)
    {
        bRc = true;
    }
    CS_EXIT();

    return bRc;
}

//---------------------------------------------------------------------------
bool Streamer::Claim(uint8_t** pu8Addr_)
{
    bool rc = true;

    CS_ENTER();

    if (!m_u16Avail) {
        rc = false;
    } else {
        if (m_pu8LockAddr == &m_pau8Buffer[m_u16Head]) {
            rc = false;
        } else {
            *pu8Addr_ = &m_pau8Buffer[m_u16Head];
            if (!m_pu8LockAddr) {
                m_pu8LockAddr = &m_pau8Buffer[m_u16Head];
            }
            m_u16Head++;
            if (m_u16Head >= m_u16Size) {
                m_u16Head = 0;
            }
            m_u16Avail--;
        }
    }

    CS_EXIT();
    return rc;
}

//---------------------------------------------------------------------------
void Streamer::Lock(uint8_t* pu8LockAddr_)
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

//---------------------------------------------------------------------------
bool Streamer::IsEmpty(void)
{
    bool rc;
    CS_ENTER();
    if (m_u16Avail == m_u16Size) {
        rc = true;
    } else {
        rc = false;
    }
    CS_EXIT();
    return rc;
}
