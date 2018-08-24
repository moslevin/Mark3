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
/**

    @file   atomic.cpp

    @brief  Basic Atomic Operations
*/

#include "mark3.h"

namespace Mark3
{
//---------------------------------------------------------------------------
uint8_t Atomic::Set(uint8_t* pu8Source_, uint8_t u8Val_)
{
    KERNEL_ASSERT(pu8Source_ != nullptr);

    uint8_t u8Ret;
    CS_ENTER();
    u8Ret       = *pu8Source_;
    *pu8Source_ = u8Val_;
    CS_EXIT();
    return u8Ret;
}
//---------------------------------------------------------------------------
uint16_t Atomic::Set(uint16_t* pu16Source_, uint16_t u16Val_)
{
    KERNEL_ASSERT(pu16Source_ != nullptr);

    uint16_t u16Ret;
    CS_ENTER();
    u16Ret       = *pu16Source_;
    *pu16Source_ = u16Val_;
    CS_EXIT();
    return u16Ret;
}
//---------------------------------------------------------------------------
uint32_t Atomic::Set(uint32_t* pu32Source_, uint32_t u32Val_)
{
    KERNEL_ASSERT(pu32Source_ != nullptr);

    uint32_t u32Ret;
    CS_ENTER();
    u32Ret       = *pu32Source_;
    *pu32Source_ = u32Val_;
    CS_EXIT();
    return u32Ret;
}

//---------------------------------------------------------------------------
uint8_t Atomic::Add(uint8_t* pu8Source_, uint8_t u8Val_)
{
    KERNEL_ASSERT(pu8Source_ != nullptr);

    uint8_t u8Ret;
    CS_ENTER();
    u8Ret = *pu8Source_;
    *pu8Source_ += u8Val_;
    CS_EXIT();
    return u8Ret;
}

//---------------------------------------------------------------------------
uint16_t Atomic::Add(uint16_t* pu16Source_, uint16_t u16Val_)
{
    KERNEL_ASSERT(pu16Source_ != nullptr);
    uint16_t u16Ret;
    CS_ENTER();
    u16Ret = *pu16Source_;
    *pu16Source_ += u16Val_;
    CS_EXIT();
    return u16Ret;
}

//---------------------------------------------------------------------------
uint32_t Atomic::Add(uint32_t* pu32Source_, uint32_t u32Val_)
{
    KERNEL_ASSERT(pu32Source_ != nullptr);

    uint32_t u32Ret;
    CS_ENTER();
    u32Ret = *pu32Source_;
    *pu32Source_ += u32Val_;
    CS_EXIT();
    return u32Ret;
}

//---------------------------------------------------------------------------
uint8_t Atomic::Sub(uint8_t* pu8Source_, uint8_t u8Val_)
{
    KERNEL_ASSERT(pu8Source_ != nullptr);

    uint8_t u8Ret;
    CS_ENTER();
    u8Ret = *pu8Source_;
    *pu8Source_ -= u8Val_;
    CS_EXIT();
    return u8Ret;
}

//---------------------------------------------------------------------------
uint16_t Atomic::Sub(uint16_t* pu16Source_, uint16_t u16Val_)
{
    KERNEL_ASSERT(pu16Source_ != nullptr);

    uint16_t u16Ret;
    CS_ENTER();
    u16Ret = *pu16Source_;
    *pu16Source_ -= u16Val_;
    CS_EXIT();
    return u16Ret;
}

//---------------------------------------------------------------------------
uint32_t Atomic::Sub(uint32_t* pu32Source_, uint32_t u32Val_)
{
    KERNEL_ASSERT(pu32Source_ != nullptr);

    uint32_t u32Ret;
    CS_ENTER();
    u32Ret = *pu32Source_;
    *pu32Source_ -= u32Val_;
    CS_EXIT();
    return u32Ret;
}

//---------------------------------------------------------------------------
bool Atomic::TestAndSet(bool* pbLock_)
{
    KERNEL_ASSERT(pbLock_ != nullptr);

    uint8_t u8Ret;
    CS_ENTER();
    u8Ret = *pbLock_;
    if (!u8Ret) {
        *pbLock_ = 1;
    }
    CS_EXIT();
    return u8Ret;
}
} // namespace Mark3
