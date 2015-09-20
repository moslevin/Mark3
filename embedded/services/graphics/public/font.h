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
    \file font.h
    \brief Font structure definitions
*/

#ifndef __FONT_H__
#define __FONT_H__

#include "kerneltypes.h"
#include "fontport.h"

//---------------------------------------------------------------------------
typedef struct
{
    uint8_t u8Width;        //!< Width of this font glyph in pixels
    uint8_t u8Height;        //!< Height of this font glyph in pixels
    uint8_t u8VOffset;        //!< Vertical offset of this glyph
    uint8_t aucData[1];        //!< Glyph data array
} Glyph_t;

//---------------------------------------------------------------------------
/*!
    The size of the glyph is the width*height (in bytes), plus the overhead 
    of the struct parameters.
*/
#define GLYPH_SIZE(x) \
    (((uint16_t)((x->u8Width + 7) >> 3) * (uint16_t)(x->u8Height)) + sizeof(Glyph_t) - 1)

//---------------------------------------------------------------------------
typedef struct
{
    uint8_t u8Size;
    uint8_t u8Flags;
    uint8_t u8StartChar;
    uint8_t u8MaxChar;
    const char *szName;
    const FONT_STORAGE_TYPE *pu8FontData;    
} Font_t;

#endif

