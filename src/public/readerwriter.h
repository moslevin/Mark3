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
    @file   readerwriter.h

    @brief  Reader-Writer lock implementation

*/

#pragma once

#include "mark3cfg.h"
#include "blocking.h"
#include "mutex.h"

namespace Mark3
{
/**
 * @brief The ReaderWriterLock class
 * This class implements an object that marshalls access to a resource based on
 * the intended usage of the resource.  A reader-writer lock permits multiple
 * concurrent read access, or single-writer access to a resource.  If the object
 * holds a write lock, other writers, and all readers will block until the writer
 * is finished.  If the object holds reader locks, all writers will block until
 * all readers are finished before the first writer can take ownership of the
 * resource.  This is based upon lower-level synchronization primatives, and is
 * somewhat more heavyweight than primative synchronization types.
 */
class ReaderWriterLock
{
public:
    void* operator new(size_t sz, void* pv) { return reinterpret_cast<ReaderWriterLock*>(pv); }

    /**
     * @brief Init
     * Initialize the reader-writer lock before use.  Must be called before attempting
     * any other operations on the object.
     */
    void Init();

    /**
     * @brief AcquireReader
     * Acquire the object's reader lock.  Multiple concurrent readers are allowed.
     * If the writer lock is currently held, the calling thread will wait until the
     * writer lock is relinquished
     */
    void AcquireReader();

    /**
     * @brief AcquireReader
     * Acquire the object's reader lock.  Multiple concurrent readers are allowed.
     * If the writer lock is currently held, the calling thread will wait until the
     * writer lock is relinquished
     * @param u32TimeoutMs_ Maximum time to wait (in ms) before the operation is aborted
     * @return true on success, false on timeout
     */
    bool AcquireReader(uint32_t u32TimeoutMs_);

    /**
     * @brief ReleaseReader
     * Release a previously-held reader lock.
     */
    void ReleaseReader();

    /**
     * @brief AcquireWriter
     * Acquire the writer lock.  Only a single writer is allowed to access the object
     * at a time.  This will block the currently-runnign thread until all other readers/writers
     * have released their locks.
     */
    void AcquireWriter();

    /**
     * @brief AcquireWriter
     * Acquire the writer lock.  Only a single writer is allowed to access the object
     * at a time.  This will block the currently-runnign thread until all other readers/writers
     * have released their locks.
     * @param u32TimeoutMs_ Maximum time to wait (in ms) before the operation is aborted
     * @return true on success, false on timeout
     */
    bool AcquireWriter(uint32_t u32TimeoutMs_);

    /**
     * @brief ReleaseWriter
     * Release the currently held writer, allowing other readers/writers to access the object
     */
    void ReleaseWriter();

private:
    /**
     * @brief AcquireReader_i
     * Internal helper function for AcquireReaer
     * @param u32TimeoutMs_ Maximum time to wait (in ms) before the operation is aborted
     * @return true on success, false on timeout
     */
    bool AcquireReader_i(uint32_t u32TimeoutMs_);
    /**
     * @brief AcquireWriter_i
     * Internal helper function for AcquireWriter
     * @param u32TimeoutMs_ Maximum time to wait (in ms) before the operation is aborted
     * @return true on success, false on timeout
     */
    bool AcquireWriter_i(uint32_t u32TimeoutMs_);

    Mutex   m_clGlobalMutex; //!< Mutex used to lock the object against concurrent read + write
    Mutex   m_clReaderMutex; //!< Mutex used to lock object for readers
    uint8_t m_u8ReadCount;   //!< Number of concurrent readers
};

} // namespace Mark3
