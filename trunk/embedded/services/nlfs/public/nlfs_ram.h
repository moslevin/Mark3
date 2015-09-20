/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   nlfs_ram.h
    \brief  RAM-based Nice Little Filesystem (NLFS) driver
*/

#ifndef __NLFS_RAM_H
#define __NLFS_RAM_H

#include "nlfs.h"

/*!
 * \brief The NLFS_RAM class
 *
 * This class implements an NLFS filesystem in a RAM buffer.  In this case, the
 * host pointer passed into the "format" call is a pointer to the locally-
 * allocated buffer in which the filesystem lives.
 */
class NLFS_RAM : public NLFS
{
private:

    /*!
     * \brief Read_Node is an implementation-specific method used to read a
     *        file node from physical storage into a local data struture.
     * \param [in] u16Node_ - File node index
     * \param [out] pstNode_ - Pointer to the file node object to read into
     */
    virtual void Read_Node(uint16_t u16Node_, NLFS_Node_t *pstNode_);

    /*!
     * \brief Write_Node is an implementation-specific method used to write a
     *        file node from a local structure back to the physical storage.
     * \param [in] u16Node_ - File node index
     * \param [in] pstNode_ - Pointer to the file node object to write from
     */
    virtual void Write_Node(uint16_t u16Node_, NLFS_Node_t *pstNode_);

    /*!
     * \brief Read_Block_Header is an implementation-specific method used to read
     *        a file block header from physical storage into a local struct.
     * \param [in] u32Block_ - data block index
     * \param [out] pstBlock_ - block header structure to read into
     */
    virtual void Read_Block_Header(uint32_t u32Block_, NLFS_Block_t *pstBlock_);

    /*!
     * \brief Write_Block_Header is an implementation-specific method used to write
     *        a file block header back to physical storage from a local struct
     * \param [in] u32Block_ - data block index
     * \param [in] pstFileBlock_ - pointer to the local data structure to write from
     */
    virtual void Write_Block_Header(uint32_t u32Block_, NLFS_Block_t *pstFileBlock_);

    /*!
     * \brief Read_Block is an implementation-specific method used to read raw file
     *        data from physical storage into a local buffer.
     *
     * \param [in] u32Block_ - filesystem block ID corresponding to the file
     * \param [in] u32Offset_ - offset (in bytes) from the beginning of the block
     * \param [out] pvData_ - output buffer to read into
     * \param [in] u32Len_ - length of data to read (in bytes)
     */
    virtual void Read_Block(uint32_t u32Block_, uint32_t u32Offset_, void *pvData_, uint32_t u32Len_);

    /*!
     * \brief Write_Block is an implementation-specific method used to write a
     *        piece of file data to its data block in the underlying physical
     *        storage.
     *
     * \param [in] u32Block_ - filesystem block ID corresponding to the file
     * \param [in] u32Offset_ - offset (in bytes) from the beginning of the block
     * \param [in] pvData_ - data buffer to write to disk
     * \param [in] u32Len_ - length of data to write (in bytes)
     */
    void Write_Block(uint32_t u32Block_, uint32_t u32Offset_, void *pvData_, uint32_t u32Len_);

};

#endif // NLFS_RAM_H
