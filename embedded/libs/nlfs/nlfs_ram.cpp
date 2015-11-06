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
    \file   nlfs_ram.cpp
    \brief  RAM-based Nice Little Filesystem (NLFS) driver
*/

#include "nlfs.h"
#include "nlfs_ram.h"
#include "memutil.h"
#include "nlfs_config.h"

//---------------------------------------------------------------------------
void NLFS_RAM::Read_Node( uint16_t u16Node_, NLFS_Node_t *pstFileNode_)
{
    NLFS_Node_t *pstFileNode =  (NLFS_Node_t*)(m_puHost->kaData
                                                    + (u16Node_ * sizeof(NLFS_Node_t)));

    MemUtil::CopyMemory(pstFileNode_, pstFileNode, sizeof(NLFS_Node_t));
}

//---------------------------------------------------------------------------
void NLFS_RAM::Write_Node(uint16_t u16Node_, NLFS_Node_t *pstFileNode_)
{
    NLFS_Node_t *pstFileNode =  (NLFS_Node_t*)(m_puHost->kaData
                                                    + (u16Node_ * sizeof(NLFS_Node_t)));

    MemUtil::CopyMemory(pstFileNode, pstFileNode_, sizeof(NLFS_Node_t));
}

//---------------------------------------------------------------------------
void NLFS_RAM::Read_Block_Header(uint32_t u32Block_, NLFS_Block_t *pstFileBlock_)
{
    NLFS_Block_t *pstFileBlock =  (NLFS_Block_t*)(m_puHost->kaData
                                                    + m_stLocalRoot.u32BlockOffset
                                                    + (u32Block_ * sizeof(NLFS_Block_t)));

    MemUtil::CopyMemory(pstFileBlock_, pstFileBlock, sizeof(NLFS_Block_t));
}

//---------------------------------------------------------------------------
void NLFS_RAM::Write_Block_Header(uint32_t u32Block_, NLFS_Block_t *pstFileBlock_)
{
    NLFS_Block_t *pstFileBlock =  (NLFS_Block_t*)(m_puHost->kaData
                                                    + m_stLocalRoot.u32BlockOffset
                                                    + (u32Block_ * sizeof(NLFS_Block_t)));

    MemUtil::CopyMemory(pstFileBlock, pstFileBlock_, sizeof(NLFS_Block_t));
}

//---------------------------------------------------------------------------
void NLFS_RAM::Read_Block(uint32_t u32Block_, uint32_t u32Offset_, void *pvData_, uint32_t u32Len_)
{
    void *pvSrc_ = (void*)( m_puHost->kaData
                            + m_stLocalRoot.u32DataOffset
                            + u32Offset_
                            + (u32Block_ * m_stLocalRoot.u32BlockSize) );
    MemUtil::CopyMemory(pvData_, pvSrc_, (uint16_t)u32Len_);    
}

//---------------------------------------------------------------------------
void NLFS_RAM::Write_Block(uint32_t u32Block_, uint32_t u32Offset_, void *pvData_, uint32_t u32Len_)
{
    void *pvDst_ = (void*)( m_puHost->kaData
                            + m_stLocalRoot.u32DataOffset
                            + u32Offset_
                            + (u32Block_ * m_stLocalRoot.u32BlockSize) );
    MemUtil::CopyMemory(pvDst_, pvData_, (uint16_t)u32Len_);    
}
