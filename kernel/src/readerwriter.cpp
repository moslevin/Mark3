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
/*!
    \file   readerwriter.cpp

    \brief  Reader-writer lock implementation

*/
#include "mark3.h"
#include "kerneldebug.h"

namespace Mark3 {

//---------------------------------------------------------------------------
void ReaderWriterLock::Init()
{
    m_u8ReadCount = 0;
    m_clGlobalMutex.Init();
    m_clReaderMutex.Init();
}

//---------------------------------------------------------------------------
void ReaderWriterLock::AcquireReader()
{
    AcquireReader_i(0);
}

//---------------------------------------------------------------------------
bool ReaderWriterLock::AcquireReader(uint32_t u32TimeoutMs_)
{
    return AcquireReader_i(u32TimeoutMs_);
}

//---------------------------------------------------------------------------
void ReaderWriterLock::ReleaseReader()
{
    m_clReaderMutex.Claim();
    m_u8ReadCount--;
    if (m_u8ReadCount == 0) {
        m_clGlobalMutex.Release();
    }
    m_clReaderMutex.Release();
}

//---------------------------------------------------------------------------
void ReaderWriterLock::AcquireWriter()
{
    AcquireWriter_i(0);
}

//---------------------------------------------------------------------------
bool ReaderWriterLock::AcquireWriter(uint32_t u32TimeoutMs_)
{
    return AcquireWriter_i(u32TimeoutMs_);
}

//---------------------------------------------------------------------------
void ReaderWriterLock::ReleaseWriter()
{
    m_clGlobalMutex.Release();
}

//---------------------------------------------------------------------------
bool ReaderWriterLock::AcquireReader_i(uint32_t u32TimeoutMs_)
{
    auto rc = true;
    if (!m_clReaderMutex.Claim(u32TimeoutMs_)) {
        return false;
    }

    m_u8ReadCount++;
    if (m_u8ReadCount == 1) {
        rc = m_clGlobalMutex.Claim(u32TimeoutMs_);
    }

    m_clReaderMutex.Release();
    return rc;
}

//---------------------------------------------------------------------------
bool ReaderWriterLock::AcquireWriter_i(uint32_t u32TimeoutMs_)
{
    return m_clGlobalMutex.Claim(u32TimeoutMs_);
}
} // namespace Mark3
