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

    \file   memutil.cpp

    \brief  Implementation of memory, string, and conversion routines
*/


#include "kerneltypes.h"
#include "mark3cfg.h"
#include "kerneldebug.h"
#include "memutil.h"

//---------------------------------------------------------------------------
void MemUtil::DecimalToHex( uint8_t u8Data_, char *szText_ )
{
    uint8_t u8Tmp = u8Data_;
    uint8_t u8Max;

    KERNEL_ASSERT( szText_ );

    if (u8Tmp >= 0x10) 
    {
        u8Max = 2;
    } 
    else 
    {
        u8Max = 1;
    }

    u8Tmp = u8Data_;
    szText_[u8Max] = 0;
    while (u8Max--)
    {
        if ((u8Tmp & 0x0F) <= 9) 
        {
            szText_[u8Max] = '0' + (u8Tmp & 0x0F);
        } 
        else 
        {
            szText_[u8Max] = 'A' + ((u8Tmp & 0x0F) - 10);
        }
        u8Tmp>>=4;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToHex( uint16_t u16Data_, char *szText_ )
{
    uint16_t u16Tmp = u16Data_;
    uint16_t u16Max = 1;
    uint16_t u16Compare = 0x0010;

    KERNEL_ASSERT( szText_ );

    while (u16Data_ > u16Compare && u16Max < 4)
    {
        u16Max++;
        u16Compare <<= 4;
    }

    u16Tmp = u16Data_;
    szText_[u16Max] = 0;
    while (u16Max--)
    {
        if ((u16Tmp & 0x0F) <= 9)
        {
            szText_[u16Max] = '0' + (u16Tmp & 0x0F);
        }
        else
        {
            szText_[u16Max] = 'A' + ((u16Tmp & 0x0F) - 10);
        }
        u16Tmp>>=4;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToHex( uint32_t u32Data_, char *szText_ )
{
    uint32_t u32Tmp = u32Data_;
    uint32_t u32Max = 1;
    uint32_t u32Compare = 0x0010;

    KERNEL_ASSERT( szText_ );

    while (u32Data_ > u32Compare && u32Max < 8)
    {
        u32Max++;
        u32Compare <<= 4;
    }

    u32Tmp = u32Data_;
    szText_[u32Max] = 0;
    while (u32Max--)
    {
        if ((u32Tmp & 0x0F) <= 9)
        {
            szText_[u32Max] = '0' + (u32Tmp & 0x0F);
        }
        else
        {
            szText_[u32Max] = 'A' + ((u32Tmp & 0x0F) - 10);
        }
        u32Tmp>>=4;
    }
}
//--------------------------------------------------------------------------- 
void MemUtil::DecimalToString( uint8_t u8Data_, char *szText_ )
{
    uint8_t u8Tmp = u8Data_;
    uint8_t u8Max;

    KERNEL_ASSERT(szText_);

    // Find max index to print...
    if (u8Data_ >= 100)
    {
        u8Max = 3;
    } 
    else if (u8Data_ >= 10)
    {
        u8Max = 2;
    }
    else 
    {
        u8Max = 1;
    }

    szText_[u8Max] = 0;
    while (u8Max--)
    {
        szText_[u8Max] = '0' + (u8Tmp % 10);
        u8Tmp/=10;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToString( uint16_t u16Data_, char *szText_ )
{
    uint16_t u16Tmp = u16Data_;
    uint16_t u16Max = 1;
    uint16_t u16Compare = 10;

    KERNEL_ASSERT(szText_);

    while (u16Data_ >= u16Compare && u16Max < 5)
    {
        u16Compare *= 10;
        u16Max++;
    }

    szText_[u16Max] = 0;
    while (u16Max--)
    {
        szText_[u16Max] = '0' + (u16Tmp % 10);
        u16Tmp/=10;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToString( uint32_t u32Data_, char *szText_ )
{
    uint32_t u32Tmp = u32Data_;
    uint32_t u32Max = 1;
    uint32_t u32Compare = 10;

    KERNEL_ASSERT(szText_);

    while (u32Data_ >= u32Compare && u32Max < 12)
    {
        u32Compare *= 10;
        u32Max++;
    }

    szText_[u32Max] = 0;
    while (u32Max--)
    {
        szText_[u32Max] = '0' + (u32Tmp % 10);
        u32Tmp/=10;
    }
}

//---------------------------------------------------------------------------
// Basic checksum routines
uint8_t  MemUtil::Checksum8( const void *pvSrc_, uint16_t u16Len_ )
{
    uint8_t u8Ret = 0;
    uint8_t *pcData = (uint8_t*)pvSrc_;

    KERNEL_ASSERT(pvSrc_);

    // 8-bit CRC, computed byte at a time
    while (u16Len_--)
    {
        u8Ret += *pcData++;
    }
    return u8Ret;
}

//---------------------------------------------------------------------------
uint16_t MemUtil::Checksum16( const void *pvSrc_, uint16_t u16Len_ )
{
    uint16_t u16Ret = 0;
    uint8_t *pcData = (uint8_t*)pvSrc_;

    KERNEL_ASSERT(pvSrc_);

    // 16-bit CRC, computed byte at a time
    while (u16Len_--)
    {
        u16Ret += *pcData++;
    }
    return u16Ret;
}

//---------------------------------------------------------------------------
// Basic string routines
uint16_t MemUtil::StringLength( const char *szStr_ )
{
    uint8_t *pcData = (uint8_t*)szStr_;
    uint16_t u16Len = 0;

    KERNEL_ASSERT(szStr_);

    while (*pcData++)
    {
        u16Len++;
    }
    return u16Len;
}

//---------------------------------------------------------------------------
bool   MemUtil::CompareStrings( const char *szStr1_, const char *szStr2_ )
{
    char *szTmp1 = (char*) szStr1_;
    char *szTmp2 = (char*) szStr2_;

    KERNEL_ASSERT(szStr1_);
    KERNEL_ASSERT(szStr2_);

    while (*szTmp1 && *szTmp2)
    {
        if (*szTmp1++ != *szTmp2++)
        {
            return false;
        }
    }

    // Both terminate at the same length
    if (!(*szTmp1) && !(*szTmp2))
    {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
void MemUtil::CopyMemory( void *pvDst_, const void *pvSrc_, uint16_t u16Len_ )
{
    char *szDst = (char*) pvDst_;
    char *szSrc = (char*) pvSrc_;

    KERNEL_ASSERT(pvDst_);
    KERNEL_ASSERT(pvSrc_);

    // Run through the strings verifying that each character matches
    // and the lengths are the same.
    while (u16Len_--)
    {
        *szDst++ = *szSrc++;
    }
}

//---------------------------------------------------------------------------
void MemUtil::CopyString( char *szDst_, const char *szSrc_ )
{
    char *szDst = (char*) szDst_;
    char *szSrc = (char*) szSrc_;

    KERNEL_ASSERT(szDst_);
    KERNEL_ASSERT(szSrc_);

    // Run through the strings verifying that each character matches
    // and the lengths are the same.
    while (*szSrc) 
    {
        *szDst++ = *szSrc++;
    }
}

//---------------------------------------------------------------------------
int16_t MemUtil::StringSearch( const char *szBuffer_, const char *szPattern_ )
{
    char *szTmpPat = (char*)szPattern_;
    int16_t i16Idx = 0;
    int16_t i16Start;
    KERNEL_ASSERT( szBuffer_ );
    KERNEL_ASSERT( szPattern_ );

    // Run through the big buffer looking for a match of the pattern 
    while (szBuffer_[i16Idx])
    {
        // Reload the pattern
        i16Start = i16Idx;
        szTmpPat = (char*)szPattern_; 
        while (*szTmpPat && szBuffer_[i16Idx]) 
        {
            if (*szTmpPat != szBuffer_[i16Idx])
            {
                break;
            }
            szTmpPat++;
            i16Idx++;
        }
        // Made it to the end of the pattern, it's a match.
        if (*szTmpPat == '\0')
        {
            return i16Start;
        }
        i16Idx++;
    }

    return -1;
}

//---------------------------------------------------------------------------
bool MemUtil::CompareMemory( const void *pvMem1_, const void *pvMem2_, uint16_t u16Len_ )
{
    char *szTmp1 = (char*) pvMem1_;
    char *szTmp2 = (char*) pvMem2_;

    KERNEL_ASSERT(pvMem1_);
    KERNEL_ASSERT(pvMem2_);

    // Run through the strings verifying that each character matches
    // and the lengths are the same.
    while (u16Len_--)
    {
        if (*szTmp1++ != *szTmp2++)
        {
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
void MemUtil::SetMemory( void *pvDst_, uint8_t u8Val_, uint16_t u16Len_ )
{
    char *szDst = (char*)pvDst_;

    KERNEL_ASSERT(pvDst_);
    
    while (u16Len_--)
    {
        *szDst++ = u8Val_;
    }
}

//---------------------------------------------------------------------------
uint8_t MemUtil::Tokenize( const char *szBuffer_, Token_t *pastTokens_, uint8_t u8MaxTokens_)
{
    uint8_t u8CurrArg = 0;
    uint8_t u8LastArg = 0;
    uint8_t i = 0;

    bool bEscape = false;

    KERNEL_ASSERT(szBuffer_);
    KERNEL_ASSERT(pastTokens_);

    while (szBuffer_[i])
    {
        //-- Handle unescaped quotes
        if (szBuffer_[i] == '\"')
        {
            if (bEscape)
            {
                bEscape = false;
            }
            else
            {
                bEscape = true;
            }
            i++;
            continue;
        }

        //-- Handle all escaped chars - by ignoring them
        if (szBuffer_[i] == '\\')
        {
            i++;
            if (szBuffer_[i])
            {
               i++;
            }
            continue;
        }

        //-- Process chars based on current escape characters
        if (bEscape)
        {
            // Everything within the quote is treated as literal, but escaped chars are still treated the same
            i++;
            continue;
        }

        //-- Non-escaped case
        if (szBuffer_[i] != ' ' )
        {
            i++;
            continue;
        }

        pastTokens_[u8CurrArg].pcToken = &(szBuffer_[u8LastArg]);
        pastTokens_[u8CurrArg].u8Len = i - u8LastArg;
        u8CurrArg++;
        if (u8CurrArg >= u8MaxTokens_)
        {
            return u8MaxTokens_;
        }

        i++;
        while (szBuffer_[i] && szBuffer_[i] == ' ')
        {
            i++;
        }

        u8LastArg = i;
    }
    if (i && !szBuffer_[i] && (i - u8LastArg))
    {
        pastTokens_[u8CurrArg].pcToken = &(szBuffer_[u8LastArg]);
        pastTokens_[u8CurrArg].u8Len = i - u8LastArg;
        u8CurrArg++;
    }
    return u8CurrArg;
}


