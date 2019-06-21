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

    @file   memutil.cpp

    @brief  Implementation of memory, string, and conversion routines
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "memutil.h"
#include "kerneldebug.h"

namespace Mark3
{

//---------------------------------------------------------------------------
void MemUtil::DecimalToString(uint8_t u8Data_, char* szText_)
{
    uint8_t u8Tmp = u8Data_;
    uint8_t u8Max;

    KERNEL_ASSERT(szText_);

    // Find max index to print...
    if (u8Data_ >= 100) {
        u8Max = 3;
    } else if (u8Data_ >= 10) {
        u8Max = 2;
    } else {
        u8Max = 1;
    }

    szText_[u8Max] = 0;
    while ((u8Max--) != 0u) {
        szText_[u8Max] = '0' + (u8Tmp % 10);
        u8Tmp /= 10;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToString(uint16_t u16Data_, char* szText_)
{
    uint16_t u16Tmp     = u16Data_;
    uint16_t u16Max     = 1;
    uint16_t u16Compare = 10;

    KERNEL_ASSERT(szText_);

    while (u16Data_ >= u16Compare && u16Max < 5) {
        u16Compare *= 10;
        u16Max++;
    }

    szText_[u16Max] = 0;
    while ((u16Max--) != 0u) {
        szText_[u16Max] = '0' + (u16Tmp % 10);
        u16Tmp /= 10;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToString(uint32_t u32Data_, char* szText_)
{
    uint32_t u32Tmp     = u32Data_;
    uint32_t u32Max     = 1;
    uint32_t u32Compare = 10;

    KERNEL_ASSERT(szText_);

    while (u32Data_ >= u32Compare && u32Max < 12) {
        u32Compare *= 10;
        u32Max++;
    }

    szText_[u32Max] = 0;
    while ((u32Max--) != 0u) {
        szText_[u32Max] = '0' + (u32Tmp % 10);
        u32Tmp /= 10;
    }
}

//---------------------------------------------------------------------------
void MemUtil::DecimalToString(uint64_t u64Data_, char* szText_)
{
    uint64_t u64Tmp     = u64Data_;
    uint64_t u64Max     = 1;
    uint64_t u64Compare = 10;

    KERNEL_ASSERT(szText_);

    while (u64Data_ >= u64Compare && u64Max < 21) {
        u64Compare *= 10;
        u64Max++;
    }

    szText_[u64Max] = 0;
    while ((u64Max--) != 0u) {
        szText_[u64Max] = '0' + (u64Tmp % 10);
        u64Tmp /= 10;
    }
}

//---------------------------------------------------------------------------
// Basic checksum routines
uint8_t MemUtil::Checksum8(const void* pvSrc_, uint16_t u16Len_)
{
    uint8_t  u8Ret  = 0;
    uint8_t* pcData = (uint8_t*)pvSrc_;

    KERNEL_ASSERT(pvSrc_);

    // 8-bit CRC, computed byte at a time
    while ((u16Len_--) != 0u) { u8Ret += *pcData++; }
    return u8Ret;
}

//---------------------------------------------------------------------------
uint16_t MemUtil::Checksum16(const void* pvSrc_, uint16_t u16Len_)
{
    uint16_t u16Ret = 0;
    uint8_t* pcData = (uint8_t*)pvSrc_;

    KERNEL_ASSERT(pvSrc_);

    // 16-bit CRC, computed byte at a time
    while ((u16Len_--) != 0u) { u16Ret += *pcData++; }
    return u16Ret;
}

//---------------------------------------------------------------------------
// Basic string routines
uint16_t MemUtil::StringLength(const char* szStr_)
{
    uint8_t* pcData = (uint8_t*)szStr_;
    uint16_t u16Len = 0;

    KERNEL_ASSERT(szStr_);

    while (*pcData++ != 0u) { u16Len++; }
    return u16Len;
}

//---------------------------------------------------------------------------
bool MemUtil::CompareStrings(const char* szStr1_, const char* szStr2_)
{
    char* szTmp1 = (char*)szStr1_;
    char* szTmp2 = (char*)szStr2_;

    KERNEL_ASSERT(szStr1_);
    KERNEL_ASSERT(szStr2_);

    while ((*szTmp1 != 0) && (*szTmp2 != 0)) {
        if (*szTmp1++ != *szTmp2++) {
            return false;
        }
    }

    // Both terminate at the same length
    return ((*szTmp1) == 0) && ((*szTmp2) == 0);
}

//---------------------------------------------------------------------------
bool MemUtil::CompareStrings(const char* szStr1_, const char* szStr2_, uint16_t u16Length_)
{
    char* szTmp1 = (char*)szStr1_;
    char* szTmp2 = (char*)szStr2_;

    while (((*szTmp1 != 0) && (*szTmp2 != 0)) && (u16Length_ != 0u)) {
        if (*szTmp1++ != *szTmp2++) {
            return false;
        }
        u16Length_--;
    }

    // Both terminate at the same length
    return (((*szTmp1) == 0) && ((*szTmp2) == 0)) || (u16Length_ == 0u);
}

//---------------------------------------------------------------------------
void MemUtil::CopyMemory(void* pvDst_, const void* pvSrc_, uint16_t u16Len_)
{
    char* szDst = (char*)pvDst_;
    char* szSrc = (char*)pvSrc_;

    KERNEL_ASSERT(pvDst_);
    KERNEL_ASSERT(pvSrc_);

    // Run through the strings verifying that each character matches
    // and the lengths are the same.
    while ((u16Len_--) != 0u) { *szDst++ = *szSrc++; }
}

//---------------------------------------------------------------------------
void MemUtil::CopyString(char* szDst_, const char* szSrc_)
{
    char* szDst = (char*)szDst_;
    char* szSrc = (char*)szSrc_;

    KERNEL_ASSERT(szDst_);
    KERNEL_ASSERT(szSrc_);

    // Run through the strings verifying that each character matches
    // and the lengths are the same.
    while (*szSrc != 0) { *szDst++ = *szSrc++; }
}

//---------------------------------------------------------------------------
int16_t MemUtil::StringSearch(const char* szBuffer_, const char* szPattern_)
{
    auto*   szTmpPat = szPattern_;
    int16_t i16Idx   = 0;
    int16_t i16Start;
    KERNEL_ASSERT(szBuffer_);
    KERNEL_ASSERT(szPattern_);

    // Run through the big buffer looking for a match of the pattern
    while (szBuffer_[i16Idx] != 0) {
        // Reload the pattern
        i16Start = i16Idx;
        szTmpPat = (char*)szPattern_;
        while ((*szTmpPat != 0) && (szBuffer_[i16Idx] != 0)) {
            if (*szTmpPat != szBuffer_[i16Idx]) {
                break;
            }
            szTmpPat++;
            i16Idx++;
        }
        // Made it to the end of the pattern, it's a match.
        if (*szTmpPat == '\0') {
            return i16Start;
        }
        i16Idx++;
    }

    return -1;
}

//---------------------------------------------------------------------------
bool MemUtil::CompareMemory(const void* pvMem1_, const void* pvMem2_, uint16_t u16Len_)
{
    auto* szTmp1 = static_cast<const char*>(pvMem1_);
    auto* szTmp2 = static_cast<const char*>(pvMem2_);

    KERNEL_ASSERT(pvMem1_);
    KERNEL_ASSERT(pvMem2_);

    // Run through the strings verifying that each character matches
    // and the lengths are the same.
    while ((u16Len_--) != 0u) {
        if (*szTmp1++ != *szTmp2++) {
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
void MemUtil::SetMemory(void* pvDst_, uint8_t u8Val_, uint16_t u16Len_)
{
    auto* szDst = static_cast<char*>(pvDst_);

    KERNEL_ASSERT(pvDst_);

    while ((u16Len_--) != 0u) { *szDst++ = u8Val_; }
}

//---------------------------------------------------------------------------
uint8_t MemUtil::Tokenize(const char* szBuffer_, Token_t* pastTokens_, uint8_t u8MaxTokens_)
{
    auto u8CurrArg = uint8_t{0};
    auto u8LastArg = uint8_t{0};
    auto i         = uint8_t{0};

    auto bEscape = false;

    KERNEL_ASSERT(szBuffer_);
    KERNEL_ASSERT(pastTokens_);

    while (szBuffer_[i] != 0) {
        //-- Handle unescaped quotes
        if (szBuffer_[i] == '\"') {
            bEscape = !bEscape;
            i++;
            continue;
        }

        //-- Handle all escaped chars - by ignoring them
        if (szBuffer_[i] == '\\') {
            i++;
            if (szBuffer_[i] != 0) {
                i++;
            }
            continue;
        }

        //-- Process chars based on current escape characters
        if (bEscape) {
            // Everything within the quote is treated as literal, but escaped chars are still treated the same
            i++;
            continue;
        }

        //-- Non-escaped case
        if (szBuffer_[i] != ' ') {
            i++;
            continue;
        }

        pastTokens_[u8CurrArg].pcToken = &(szBuffer_[u8LastArg]);
        pastTokens_[u8CurrArg].u8Len   = i - u8LastArg;
        u8CurrArg++;
        if (u8CurrArg >= u8MaxTokens_) {
            return u8MaxTokens_;
        }

        i++;
        while (szBuffer_[i] == ' ') { i++; }

        u8LastArg = i;
    }
    if ((i != 0u) && (szBuffer_[i] == 0) && ((i - u8LastArg) != 0)) {
        pastTokens_[u8CurrArg].pcToken = &(szBuffer_[u8LastArg]);
        pastTokens_[u8CurrArg].u8Len   = i - u8LastArg;
        u8CurrArg++;
    }
    return u8CurrArg;
}
} // namespace Mark3
