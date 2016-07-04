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
    \file   nlfs_eeprom.cpp
    \brief  RAM-based Nice Little Filesystem (NLFS) driver
*/

#include "nlfs.h"
#include "nlfs_eeprom.h"
#include "memutil.h"
#include "nlfs_config.h"

#include <avr/io.h>
#include <avr/eeprom.h>

//---------------------------------------------------------------------------
void NLFS_EEPROM::Read_Node( uint16_t u16Node_, NLFS_Node_t *pstFileNode_)
{
    NLFS_Node_t *pstFileNode =  (NLFS_Node_t*)(m_puHost->kaData
                                                    + (u16Node_ * sizeof(NLFS_Node_t)));

    eeprom_read_block((void*)pstFileNode_, (const void*)pstFileNode, sizeof(NLFS_Node_t) );
}

//---------------------------------------------------------------------------
void NLFS_EEPROM::Write_Node(uint16_t u16Node_, NLFS_Node_t *pstFileNode_)
{
    NLFS_Node_t *pstFileNode =  (NLFS_Node_t*)(m_puHost->kaData
                                                    + (u16Node_ * sizeof(NLFS_Node_t)));

    eeprom_write_block((void*)pstFileNode_, (void*)pstFileNode, sizeof(NLFS_Node_t) );
}

//---------------------------------------------------------------------------
void NLFS_EEPROM::Read_Block_Header(uint32_t u32Block_, NLFS_Block_t *pstFileBlock_)
{
    NLFS_Block_t *pstFileBlock =  (NLFS_Block_t*)(m_puHost->kaData
                                                    + m_stLocalRoot.u32BlockOffset
                                                    + (u32Block_ * sizeof(NLFS_Block_t)));

    eeprom_read_block((void*)pstFileBlock_, (const void*)pstFileBlock, sizeof(NLFS_Block_t) );
}

//---------------------------------------------------------------------------
void NLFS_EEPROM::Write_Block_Header(uint32_t u32Block_, NLFS_Block_t *pstFileBlock_)
{
    NLFS_Block_t *pstFileBlock =  (NLFS_Block_t*)(m_puHost->kaData
                                                    + m_stLocalRoot.u32BlockOffset
                                                    + (u32Block_ * sizeof(NLFS_Block_t)));

    eeprom_write_block((void*)pstFileBlock_, (void*)pstFileBlock, sizeof(NLFS_Block_t) );
}

//---------------------------------------------------------------------------
void NLFS_EEPROM::Read_Block(uint32_t u32Block_, uint32_t u32Offset_, void *pvData_, uint32_t u32Len_)
{
    const void* pvAddr = (const void*)( m_puHost->kaData
                            + m_stLocalRoot.u32DataOffset
                            + u32Offset_
                            + (u32Block_ * m_stLocalRoot.u32BlockSize) );

    eeprom_read_block(pvData_, pvAddr, (size_t)u32Len_ );
}

//---------------------------------------------------------------------------
void NLFS_EEPROM::Write_Block(uint32_t u32Block_, uint32_t u32Offset_, void *pvData_, uint32_t u32Len_)
{
    void* pvAddr = (void*)( m_puHost->kaData
                            + m_stLocalRoot.u32DataOffset
                            + u32Offset_
                            + (u32Block_ * m_stLocalRoot.u32BlockSize) );

    eeprom_write_block(pvData_, pvAddr, (size_t)u32Len_ );
}
