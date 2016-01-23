/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file graphics_slip.h
    \brief SLIP Serial graphics driver
*/

#include "kerneltypes.h"
#include "driver.h"
#include "graphics.h"
#include "graphics_slip.h"
#include "slip.h"
#include "slip_mux.h"
#include "font.h"

//---------------------------------------------------------------------------
bool GraphicsSlip::FindStamp(uint8_t *pu8Stamp_)
{
    uint8_t i;
    // iterate through to see if the stamp already exists
    for (i=0; i < STAMP_CACHE_SIZE; i++)
    {
        if (m_astStampCache[i].pu8Data == pu8Stamp_)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
void GraphicsSlip::AddStamp(uint8_t *pu8Stamp_)
{
    // Update the stamp cache with the new stuff
    m_astStampCache[m_u8StampCacheIndex].pu8Data = pu8Stamp_;
        
    // 32 element cache.
    m_u8StampCacheIndex = (m_u8StampCacheIndex + 1);
    if (m_u8StampCacheIndex >= STAMP_CACHE_SIZE)
    {
        m_u8StampCacheIndex = 0;
    }
}

//---------------------------------------------------------------------------
void GraphicsSlip::AddIfNecessary(uint8_t *pu8Stamp_, uint8_t u8Size_)
{
    if (!FindStamp(pu8Stamp_))
    {
        // Send a copy of the stamp to the host
        m_pclSlip->WriteData(SLIP_CHANNEL_GRAPHICS, (char*)pu8Stamp_, u8Size_);

        // Update the cache
        AddStamp(pu8Stamp_);
    }
}

//---------------------------------------------------------------------------
void GraphicsSlip::Init(void)
{
    m_u16Res16X = 240;
    m_u16Res16Y = 192;
    m_u8BPP = 16;
    m_u16Left = 0;
    m_u16Right = 239;
    m_u16Top = 0;
    m_u16Bottom = 191;
    
    m_pclSlip->SetDriver( DriverList::FindByPath("/dev/tty" ) );                            
}

//---------------------------------------------------------------------------
void GraphicsSlip::DrawPixel(DrawPoint_t *pstPoint_)
{
    SlipDataVector astSlipData[2];
    uint8_t u8EventType = DISPLAY_EVENT_SET_PIXEL;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstPoint_;
    astSlipData[1].u8Size = sizeof(DrawPoint_t);
    
    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 2);
}

//---------------------------------------------------------------------------
void GraphicsSlip::ClearScreen(void)
{
    uint8_t u8Data;
    u8Data = DISPLAY_EVENT_CLEAR;
    
    m_pclSlip->WriteData(SLIP_CHANNEL_GRAPHICS, (char*)&u8Data, 1);
}

//---------------------------------------------------------------------------
void GraphicsSlip::Line(DrawLine_t *pstLine_)
{
    SlipDataVector astSlipData[2];
    uint8_t u8EventType = DISPLAY_EVENT_LINE;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstLine_;
    astSlipData[1].u8Size = sizeof(DrawLine_t);
    
    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 2);
}

//---------------------------------------------------------------------------
void GraphicsSlip::Rectangle(DrawRectangle_t *pstRectangle_)
{
    SlipDataVector astSlipData[2];
    uint8_t u8EventType = DISPLAY_EVENT_RECTANGLE;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstRectangle_;
    astSlipData[1].u8Size = sizeof(DrawRectangle_t);
    
    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 2);
}

//---------------------------------------------------------------------------
void GraphicsSlip::Circle(DrawCircle_t *pstCircle_)
{
    SlipDataVector astSlipData[2];
    uint8_t u8EventType = DISPLAY_EVENT_CIRCLE;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstCircle_;
    astSlipData[1].u8Size = sizeof(DrawCircle_t);
    
    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 2);
}

//---------------------------------------------------------------------------
void GraphicsSlip::Ellipse(DrawEllipse_t *pstEllipse_)
{
    SlipDataVector astSlipData[2];
    uint8_t u8EventType = DISPLAY_EVENT_ELLIPSE;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstEllipse_;
    astSlipData[1].u8Size = sizeof(DrawEllipse_t);
    
    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 2);
}

//---------------------------------------------------------------------------
void GraphicsSlip::Stamp(DrawStamp_t *pstStamp_)
{
    SlipDataVector astSlipData[3];
    
    uint8_t u8EventType = DISPLAY_EVENT_STAMP;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstStamp_;
    astSlipData[1].u8Size = sizeof(DrawStamp_t) - 2;
    astSlipData[2].pu8Data = pstStamp_->pu8Data;
    astSlipData[2].u8Size = (uint8_t)(((pstStamp_->u16Width + 7) / 8)
                                            * pstStamp_->u16Height);

    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 3);
}

//---------------------------------------------------------------------------
uint8_t GraphicsSlip::Strlen(char *szData_)
{
    uint8_t u8Retval = 0;
    while(*szData_++)
    {
        u8Retval++;
    }
    return u8Retval + 1;
}

#if 0
//---------------------------------------------------------------------------
void GraphicsSlip::Text(DrawText_t *pstText_)
{
    SlipDataVector astSlipData[5];
    
    uint8_t u8EventType = DISPLAY_EVENT_TEXT;
    
    astSlipData[0].pu8Data = &u8EventType;
    astSlipData[0].u8Size = 1;
    astSlipData[1].pu8Data = (uint8_t*)pstText_;
    astSlipData[1].u8Size = sizeof(DrawText_t) - 4;
    astSlipData[2].pu8Data = &(pstText_->pstFont->u8Size);
    astSlipData[2].u8Size = 1;
    astSlipData[3].pu8Data = (uint8_t*) pstText_->pstFont->szName;
    astSlipData[3].u8Size = Strlen((char*)pstText_->pstFont->szName);
    astSlipData[4].pu8Data = (uint8_t*)pstText_->pcString;
    astSlipData[4].u8Size = Strlen((char*)pstText_->pcString);

    m_pclSlip->WriteVector(SLIP_CHANNEL_GRAPHICS, astSlipData, 5);
}

#endif
