/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   readerwriter.cpp

    \brief  Reader-writer lock implementation

*/
#include "mark3.h"
#include "kerneldebug.h"

#define _CAN_HAS_DEBUG

#if KERNEL_USE_READERWRITER

namespace Mark3 {
void ReaderWriterLock::Init()
{
    m_u8ReadCount = 0;
    m_clGlobalMutex.Init();
    m_clReaderMutex.Init();
}

void ReaderWriterLock::AcquireReader()
{
#if KERNEL_USE_TIMEOUTS
    AcquireReader_i(0);
#else
    AcquireReader_i();
#endif
}

#if KERNEL_USE_TIMEOUTS
bool ReaderWriterLock::AcquireReader(uint32_t u32TimeoutMs_)
{
    return AcquireReader_i(u32TimeoutMs_);
}
#endif

void ReaderWriterLock::ReleaseReader()
{
    m_clReaderMutex.Claim();
    m_u8ReadCount--;
    if (m_u8ReadCount == 0) {
        m_clGlobalMutex.Release();
    }
    m_clReaderMutex.Release();
}

void ReaderWriterLock::AcquireWriter()
{
#if KERNEL_USE_TIMEOUTS
    AcquireWriter_i(0);
#else
    AcquireWriter_i();
#endif
}

#if KERNEL_USE_TIMEOUTS
bool ReaderWriterLock::AcquireWriter(uint32_t u32TimeoutMs_)
{
    return AcquireWriter_i(u32TimeoutMs_);
}
#endif

void ReaderWriterLock::ReleaseWriter()
{
    m_clGlobalMutex.Release();
}

#if KERNEL_USE_TIMEOUTS
bool ReaderWriterLock::AcquireReader_i(uint32_t u32TimeoutMs_)
{
    auto rc = false;
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
#else
void ReaderWriterLock::AcquireReader_i()
{
    m_clReaderMutex.Claim();

    m_u8ReadCount++;
    if (m_u8ReadCount == 1) {
        m_clGlobalMutex.Claim();
    }

    m_clReaderMutex.Release();
}
#endif

#if KERNEL_USE_TIMEOUTS
bool ReaderWriterLock::AcquireWriter_i(uint32_t u32TimeoutMs_)
{
    return m_clGlobalMutex.Claim(u32TimeoutMs_);
}
#else
void ReaderWriterLock::AcquireWriter_i()
{
    m_clGlobalmutex.Claim();
}
#endif
} // namespace Mark3

#endif // KERNEL_USE_READERWRITER

