/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   atomic.cpp

    \brief  Basic Atomic Operations
*/

#include "atomic.h"

//---------------------------------------------------------------------------
K_UCHAR Atomic::Set( K_UCHAR *pucSource_, K_UCHAR ucVal_ )
{
    K_UCHAR ucRet;
    CS_ENTER();
    ucRet = *pucSource_;
    *pucSource_ = ucVal_;
    CS_EXIT();
    return ucRet;
}

//---------------------------------------------------------------------------
K_UCHAR Atomic::Add( K_UCHAR *pucSource_, K_UCHAR ucVal_ )
{
    K_UCHAR ucRet;
    CS_ENTER();
    ucRet = *pucSource_;
    *pucSource_ += ucVal_;
    CS_EXIT();
    return ucRet;
}

//---------------------------------------------------------------------------
K_UCHAR Atomic::Sub( K_UCHAR *pucSource_, K_UCHAR ucVal_ )
{
    K_UCHAR ucRet;
    CS_ENTER();
    ucRet = *pucSource_;
    *pucSource_ -= ucVal_;
    CS_EXIT();
    return ucRet;
}

//---------------------------------------------------------------------------
K_BOOL Atomic::TestAndSet( K_BOOL *pbLock )
{
    K_UCHAR ucRet;
    CS_ENTER();
    ucRet = *pbLock;
    if (!ucRet)
    {
        *pbLock = 1;
    }
    CS_EXIT();
    return ucRet;
}
