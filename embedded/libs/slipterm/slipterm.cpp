/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file slipterm.cpp
    
    \brief Serial debug interface using SLIP protocol, and FunkenSlip 
           multiplexing.
*/

#include "kerneltypes.h"
#include "slip.h"
#include "slipterm.h"

//---------------------------------------------------------------------------
void SlipTerm::Init()
{
    m_clSlip.SetDriver( DriverList::FindByPath("/dev/tty" ) );            
    m_u8Verbosity = SEVERITY_DEBUG;
}

//---------------------------------------------------------------------------
uint16_t SlipTerm::StrLen( const char *szLine_ )
{
    uint16_t i=0;
    while (szLine_[i] != 0 ) 
    {
        i++;
    }
    return i;
}

//---------------------------------------------------------------------------
void SlipTerm::PrintLn( const char *szLine_ )
{
    SlipDataVector astData[2];
    astData[0].pu8Data = (uint8_t*)szLine_;
    astData[0].u8Size = StrLen(szLine_);
    astData[1].pu8Data = (uint8_t*)"\r\n";
    astData[1].u8Size = 2;
    
    m_clSlip.WriteVector(SLIP_CHANNEL_TERMINAL, astData, 2);
}

//---------------------------------------------------------------------------
void SlipTerm::PrintLn( uint8_t u8Severity_, const char *szLine_ )
{
    if (u8Severity_ <= m_u8Verbosity)
    {
        PrintLn( szLine_ );
    }
}
