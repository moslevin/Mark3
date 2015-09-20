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
    \file nlfs_file.h
    \brief NLFS file access class
*/

#ifndef __NLFS_FILE_H
#define __NLFS_FILE_H

#include "kerneltypes.h"
#include "nlfs.h"
#include "nlfs_config.h"

//----------------------------------------------------------------------------
typedef enum
{
    NLFS_FILE_CREATE = 0x01,      //!< Create the file if it does not exist
    NLFS_FILE_APPEND = 0x02,      //!< Open to end of file
    NLFS_FILE_TRUNCATE = 0x04,    //!< Truncate file size to 0-bytes
    NLFS_FILE_READ = 0x08,        //!< Open file for read
    NLFS_FILE_WRITE = 0x10        //!< Open file for write
} NLFS_File_Mode;
typedef uint8_t NLFS_File_Mode_t;

//----------------------------------------------------------------------------
/*!
 * \brief The NLFS_File class
 *
 * This class contains an implementation of file-level access built on-top of
 * the NLFS filesystem architecture.  An instance of this class represents an
 * active/open file from inside the NLFSfilesystem.
 */
class NLFS_File
{

public:
    /*!
     * \brief Open  Opens a file from a given filesystem
     * \param pclFS_ - Pointer to the NLFS filesystem containing the file
     * \param szPath_ - Path to the file within the NLFS filesystem
     * \param eMode_ - File open mode
     * \return 0 on success, -1 on failure
     */
    int     Open(NLFS *pclFS_, const char *szPath_, NLFS_File_Mode_t eMode_);

    /*!
     * \brief Read Read bytes from a file into a specified data buffer
     * \param [in] u32Len_ - Length (in bytes) of data to read
     * \param [out] pvBuf_ - Pointer to the buffer to read into
     * \return Number of bytes read from the file
     */
    int     Read(void *pvBuf_, uint32_t u32Len_);

    /*!
     * \brief Write Write a specified blob of data to the file
     * \param [in] u32Len_ - Length (in bytes) of the source buffer
     * \param [in] pvBuf_ - Pointer to the data buffer containing the data to
     *                      be written
     * \return Number of bytes written to the file
     */
    int     Write(void *pvBuf_, uint32_t u32Len_);

    /*!
     * \brief Seek Seek to the specified byte offset within the file
     * \param [in] u32Offset_ Offset in bytes from the beginning of the file
     * \return 0 on success, -1 on failure
     */
    int     Seek(uint32_t u32Offset_);

    /*!
     * \brief Close Is used to close an open file buffer
     * \return 0 on success, -1 on failure.
     */
    int     Close(void);

private:
    NLFS                *m_pclFileSystem;       //!< Pointer to the host filesystem
    uint32_t             m_u32Offset;             //!< Current byte offset within the file
    uint32_t             m_u32CurrentBlock;       //!< Index of the current filesystem block
    uint16_t            m_u16File;               //!< File index of the current file
    NLFS_File_Mode_t    m_u8Flags;              //!< File mode flags
    NLFS_Node_t m_stNode;               //!< Local copy of the file node
};

#endif // __NLFS_FILE_H
