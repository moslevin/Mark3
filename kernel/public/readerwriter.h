/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   readerwriter.h

    \brief  Reader-Writer lock implementation

*/

#pragma once

#include "mark3cfg.h"
#include "blocking.h"
#include "mutex.h"

#if KERNEL_USE_READERWRITER
namespace Mark3 {

/*!
 * \brief The ReaderWriterLock class
 */
class ReaderWriterLock {
public:
    void* operator new(size_t sz, void* pv) { return (ReaderWriterLock*)pv; }
    ~ReaderWriterLock();

    /*!
     * \brief Init
     */
    void Init();

    /*!
     * \brief AcquireReader
     */
    void AcquireReader();

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief AcquireReader
     * \param u32TimeoutMs_
     * \return
     */
    bool AcquireReader(uint32_t u32TimeoutMs_);
#endif
    
    /*!
     * \brief ReleaseReader
     */
    void ReleaseReader();

    /*!
     * \brief AcquireWriter
     */
    void AcquireWriter();

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief AcquireWriter
     * \param u32TimeoutMs_
     * \return
     */
    bool AcquireWriter(uint32_t u32TimeoutMs_);
#endif

    /*!
     * \brief ReleaseWriter
     */
    void ReleaseWriter();
private:

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief AcquireReader_i
     * \param u32TimeoutMs_
     * \return
     */
    bool AcquireReader_i(uint32_t u32TimeoutMs_);
#else
    /*!
     * \brief AcquireReader_i
     */
    void AcquireReader_i();
#endif

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief AcquireWriter_i
     * \param u32TimeoutMs_
     * \return
     */
    bool AcquireWriter_i(uint32_t u32TimeoutMs_);
#endif

private:
    Mutex m_clGlobalMutex;
    Mutex m_clReaderMutex;
    uint8_t m_u8ReadCount;
};

} // namespace Mark3
#endif
