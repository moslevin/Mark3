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
    \file nlfs_file.cpp
    \brief Nice Little Filesystem - File Access Class
*/

#include "kerneltypes.h"
#include "memutil.h"
#include "nlfs_file.h"
#include "nlfs.h"
#include "nlfs_config.h"

//----------------------------------------------------------------------------
int NLFS_File::Open(NLFS *pclFS_, const char *szPath_, NLFS_File_Mode_t eMode_)
{
    uint16_t u16Node;
    u16Node = pclFS_->Find_File(szPath_);

    if (INVALID_NODE == u16Node)
    {
        DEBUG_PRINT("file does not exist in path\n");
        if (eMode_ & NLFS_FILE_CREATE)
        {
            DEBUG_PRINT("Attempt to create\n");
            u16Node = pclFS_->Create_File(szPath_);
            if (INVALID_NODE == u16Node)
            {
                DEBUG_PRINT("unable to create node in path\n");
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }

    DEBUG_PRINT("Current Node: %d\n", u16Node);

    m_pclFileSystem = pclFS_;
    m_pclFileSystem->Read_Node(u16Node, &m_stNode);

    m_u16File = u16Node;

    if (eMode_ & NLFS_FILE_APPEND)
    {
        if (!(eMode_ & NLFS_FILE_WRITE))
        {
            DEBUG_PRINT("Open file for append in read-only mode?  Why!\n");
            return -1;
        }
        if (-1 == Seek(m_stNode.stFileNode.u32FileSize))
        {
            DEBUG_PRINT("file open failed - error seeking to EOF for append\n");
            return -1;
        }

    }
    else if (eMode_ & NLFS_FILE_TRUNCATE)
    {
        if (!(eMode_ & NLFS_FILE_WRITE))
        {
            DEBUG_PRINT("Truncate file in read-only mode?  Why!\n");
            return -1;
        }

        uint32_t u32Curr = m_stNode.stFileNode.u32FirstBlock;
        uint32_t u32Prev = u32Curr;

        // Go through and clear all blocks allocated to the file
        while (INVALID_BLOCK != u32Curr)
        {
            NLFS_Block_t stBlock;
            pclFS_->Read_Block_Header(u32Curr, &stBlock);

            u32Prev = u32Curr;
            u32Curr = stBlock.u32NextBlock;

            pclFS_->Push_Free_Block(u32Prev);
        }

        m_u32Offset = 0;
        m_u32CurrentBlock = m_stNode.stFileNode.u32FirstBlock;
    }
    else
    {
        // Open file to beginning of file, regardless of mode.
        m_u32Offset = 0;
        m_u32CurrentBlock = m_stNode.stFileNode.u32FirstBlock;
    }

    m_u8Flags = eMode_;

    DEBUG_PRINT("Current Block: %d\n", m_u32CurrentBlock);
    DEBUG_PRINT("file open OK\n");
    return 0;
}

//----------------------------------------------------------------------------
int NLFS_File::Seek(uint32_t u32Offset_)
{
    NLFS_Block_t stBlock;
    m_u32CurrentBlock = m_stNode.stFileNode.u32FirstBlock;
    m_u32Offset = u32Offset_;

    if (INVALID_NODE == m_u16File)
    {
        DEBUG_PRINT("Error - invalid file");
        return -1;
    }

    if (INVALID_BLOCK == m_u32CurrentBlock)
    {
        DEBUG_PRINT("Invalid block\n");
        m_u32Offset = 0;
        return -1;
    }

    m_pclFileSystem->Read_Block_Header(m_u32CurrentBlock, &stBlock);

    while (u32Offset_ >= m_pclFileSystem->GetBlockSize())
    {
        u32Offset_ -= m_pclFileSystem->GetBlockSize();
        m_u32CurrentBlock = stBlock.u32NextBlock;
        if ((u32Offset_) && (INVALID_BLOCK == m_u32CurrentBlock))
        {
            m_u32CurrentBlock = m_stNode.stFileNode.u32FirstBlock;
            m_u32Offset = 0;
            return -1;
        }
        m_pclFileSystem->Read_Block_Header(m_u32CurrentBlock, &stBlock);
    }

    m_u32Offset = u32Offset_;
    return 0;
}

//----------------------------------------------------------------------------
int NLFS_File::Read(void *pvBuf_, uint32_t u32Len_)
{
    uint32_t u32BytesLeft;
    uint32_t u32Offset;
    uint32_t u32Read = 0;
    bool bBail = false;

    char *szCharBuf = (char*)pvBuf_;

    if (INVALID_NODE == m_u16File)
    {
        DEBUG_PRINT("Error - invalid file");
        return -1;
    }

    if (!(NLFS_FILE_READ & m_u8Flags))
    {
        DEBUG_PRINT("Error - file not open for read\n");
        return -1;
    }

    DEBUG_PRINT("Reading: %d bytes from file\n", u32Len_);
    while (u32Len_ && !bBail)
    {
        u32Offset = m_u32Offset & (m_pclFileSystem->GetBlockSize() - 1);
        u32BytesLeft = m_pclFileSystem->GetBlockSize() - u32Offset;
        if (u32BytesLeft > u32Len_)
        {
            u32BytesLeft = u32Len_;
        }
        if (m_u32Offset + u32BytesLeft >= m_stNode.stFileNode.u32FileSize)
        {
            u32BytesLeft = m_stNode.stFileNode.u32FileSize - m_u32Offset;
            bBail = true;
        }

        DEBUG_PRINT( "%d bytes left in block, %d len, %x block\n", u32BytesLeft, u32Len_, m_u32CurrentBlock);
        if (u32BytesLeft && u32Len_ && (INVALID_BLOCK != m_u32CurrentBlock))
        {
            m_pclFileSystem->Read_Block(m_u32CurrentBlock, u32Offset, (void*)szCharBuf, u32BytesLeft );

            u32Read += u32BytesLeft;
            u32Len_ -= u32BytesLeft;
            szCharBuf += u32BytesLeft;
            m_u32Offset += u32BytesLeft;
            DEBUG_PRINT( "%d bytes to go\n", u32Len_);
        }
        if (u32Len_)
        {
            DEBUG_PRINT("reading next node\n");
            NLFS_Block_t stBlock;
            m_pclFileSystem->Read_Block_Header(m_u32CurrentBlock, &stBlock);
            m_u32CurrentBlock = stBlock.u32NextBlock;
        }

        if (INVALID_BLOCK == m_u32CurrentBlock)
        {
            break;
        }

    }
    DEBUG_PRINT("Return :%d bytes read\n", u32Read);
    return u32Read;
}

//----------------------------------------------------------------------------
int NLFS_File::Write(void *pvBuf_, uint32_t u32Len_)
{
    uint32_t u32BytesLeft;
    uint32_t u32Offset;
    uint32_t u32Written = 0;
    char *szCharBuf = (char*)pvBuf_;

    if (INVALID_NODE == m_u16File)
    {
        DEBUG_PRINT("Error - invalid file");
        return -1;
    }

    if (!(NLFS_FILE_WRITE & m_u8Flags))
    {
        DEBUG_PRINT("Error - file not open for write\n");
        return -1;
    }

    DEBUG_PRINT("writing: %d bytes to file\n", u32Len_);
    while (u32Len_)
    {
        u32Offset = m_u32Offset & (m_pclFileSystem->GetBlockSize() - 1);
        u32BytesLeft = m_pclFileSystem->GetBlockSize() - u32Offset;
        if (u32BytesLeft > u32Len_)
        {
            u32BytesLeft = u32Len_;
        }
        if (u32BytesLeft && u32Len_ && (INVALID_BLOCK != m_u32CurrentBlock))
        {
            m_pclFileSystem->Write_Block(m_u32CurrentBlock, u32Offset, (void*)szCharBuf, u32BytesLeft );
            u32Written += u32BytesLeft;
            u32Len_ -= u32BytesLeft;
            szCharBuf += u32BytesLeft;
            m_stNode.stFileNode.u32FileSize += u32BytesLeft;
            m_u32Offset += u32BytesLeft;
            DEBUG_PRINT( "%d bytes to go\n", u32Len_);
        }
        if (!u32Len_)
        {
            m_pclFileSystem->Write_Node(m_u16File, &m_stNode);
        }
        else
        {
            DEBUG_PRINT("appending\n");
            m_u32CurrentBlock = m_pclFileSystem->Append_Block_To_Node(&m_stNode);
        }

        DEBUG_PRINT("writing node to file\n");
        m_pclFileSystem->Write_Node(m_u16File, &m_stNode);
    }
    return u32Written;
}

//----------------------------------------------------------------------------
int NLFS_File::Close(void)
{
    m_u16File = INVALID_NODE;
    m_u32CurrentBlock = INVALID_BLOCK;
    m_u32Offset = 0;
    m_u8Flags = 0;
    return 0;
}
