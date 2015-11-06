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
    \file slip_mux.cpp
    \brief FunkenSlip Channel Multiplexer
    
    Demultiplexes FunkenSlip packets transmitted over a single serial channel,
    and provides an abstraction to attach handlers for each event type. 
*/

#include "kerneltypes.h"
#include "driver.h"
#include "drvUART.h"
#include "slip.h"
#include "slip_mux.h"
#include "message.h"

//---------------------------------------------------------------------------
MessageQueue *SlipMux::m_pclMessageQueue;
uint8_t SlipMux::m_aucData[SLIP_BUFFER_SIZE];
Driver *SlipMux::m_pclDriver;
Slip_Channel SlipMux::m_apfChannelHandlers[SLIP_CHANNEL_COUNT] = {0};
Semaphore SlipMux::m_clSlipSem;
Slip SlipMux::m_clSlip;

//---------------------------------------------------------------------------
/*!
    \fn static void SlipMux_CallBack(Driver *pclDriver_)
    \param pclDriver_ Pointer to the driver data for the port triggering
                    the callback
*/
static void SlipMux_CallBack( Driver *pclDriver_)
{
    Message *pclMsg = GlobalMessagePool::Pop();
    if (pclMsg)
    {        
        pclDriver_->Control(CMD_SET_RX_DISABLE, 0, 0, 0, 0);
        
        // Send a message to the queue, letting it know that there's a 
        // pending slip message that needs to be processed
        pclMsg->SetCode(SLIP_RX_MESSAGE_ID);
        pclMsg->SetData(NULL);
        SlipMux::GetQueue()->Send(pclMsg);
    }
}

//---------------------------------------------------------------------------
void SlipMux::Init(const char *pcDriverPath_, uint16_t u16RxSize_, uint8_t *aucRx_, uint16_t u16TxSize_, uint8_t *aucTx_)
{
    m_pclDriver = DriverList::FindByPath(pcDriverPath_);
    m_pclMessageQueue = NULL;

    m_clSlip.SetDriver(m_pclDriver);
    m_clSlipSem.Init(0, 1);
    
    m_pclDriver->Control(CMD_SET_BUFFERS, (void*)aucRx_, u16RxSize_, (void*)aucTx_, u16TxSize_);    
    m_pclDriver->Control(CMD_SET_RX_CALLBACK, (void*)SlipMux_CallBack, 0, 0, 0);
    {
        uint8_t u8Escape = 192;
        m_pclDriver->Control(CMD_SET_RX_ESCAPE, (void*)&u8Escape, 1, 0, NULL);
    }
}

//---------------------------------------------------------------------------
void SlipMux::InstallHandler( uint8_t u8Channel_, Slip_Channel pfHandler_ )
{
    if (pfHandler_)
    {
        m_apfChannelHandlers[u8Channel_] = pfHandler_;
    }
}

//---------------------------------------------------------------------------
void SlipMux::MessageReceive(void)
{    
    uint16_t u16Len;
    uint8_t u8Channel;

    u16Len = m_clSlip.ReadData( &u8Channel, (char*)m_aucData, SLIP_BUFFER_SIZE );
    if (u16Len && (m_apfChannelHandlers[u8Channel] != NULL))
    {
        m_apfChannelHandlers[u8Channel]( m_pclDriver, u8Channel, &(m_aucData[3]), u16Len);
    }
        
    // Re-enable the driver once we're done.
    m_pclDriver->Control( CMD_SET_RX_ENABLE, 0, 0, 0, 0 );    
}

