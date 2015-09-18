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

    \file   sw_uart.cpp

    \brief  Software UART Implementation
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "sw_uart.h"

//---------------------------------------------------------------------------
#define SW_UART_BIT     (0)
#define SW_UART_DIR     DDRB
#define SW_UART_OUT     PORTB
#define SYSTEM_FREQ		(16000000)

//---------------------------------------------------------------------------
#define FRAME_BITS		(10)	// 8 data bits, 1 start, 1 stop bit.

//---------------------------------------------------------------------------
#define UART_SET_OUTPUT(x)   ( ((x) & 1) ? ( SW_UART_OUT |= (1 << SW_UART_BIT) ) : ( SW_UART_OUT &= ~(1 <<SW_UART_BIT) ) )
//---------------------------------------------------------------------------
typedef enum
{
    TX_STATE_IDLE = 0,		// No data currently being sent
    TX_STATE_TX,			// Data transmit in progress
} TXState_t;

//---------------------------------------------------------------------------
static volatile uint8_t     u8TX;		// Current transmit byte register
static volatile uint8_t     u8Count;	// Number of bits left to shift out
static volatile TXState_t   eState;		// Current transmit state-machine state.

//---------------------------------------------------------------------------
void SoftwareUART::Init( uint32_t u32Baud_ )
{
    // Baudrate = bits / second
    // Bit Time = 1 / baudrate = second / bit
    // CLK Period = Bit Time (seconds / bit) * peripheral clock speed (cycles / second)

	SW_UART_DIR |= (1 << SW_UART_BIT);
	SW_UART_OUT |= (1 << SW_UART_BIT);
	
    // Start checking
    m_u16Prescale = 1024;

    // Iteratively determine our ideal prescaler value for the supplied baud rate
    bool bDone = false;
    uint32_t u32BitPeriod;
    while (!bDone)
    {
        u32BitPeriod = (SYSTEM_FREQ/(uint32_t)m_u16Prescale)/u32Baud_;
        if (u32BitPeriod >= 256)
        {
			// Bail, this is as low as we go...                
            if (m_u16Prescale == 1)
            {
                m_u16Prescale = 8;				
            }
            if (m_u16Prescale == 8)
            {
                m_u16Prescale = 64;
            }
            else if (m_u16Prescale == 64)
            {
                m_u16Prescale = 256;
            }
            else if (m_u16Prescale == 256)
            {
                m_u16Prescale = 1024;
            }
            bDone = true;
        }
        else if (u32BitPeriod < 32)
        {
            if (m_u16Prescale == 1)
            {
                bDone = true;
            }
            else if (m_u16Prescale == 8)
            {
                m_u16Prescale = 1;
            }
            else if (m_u16Prescale == 64)
            {
                m_u16Prescale = 8;
            }
            else if (m_u16Prescale == 256)
            {
                m_u16Prescale = 64;
            }
            else if (m_u16Prescale == 1024)
            {
                m_u16Prescale = 256;
            }
        }
        else
        {
            bDone = true;
        }
    }

    u32BitPeriod = ((SYSTEM_FREQ/(uint32_t)m_u16Prescale)/u32Baud_) - 1;
    if (u32BitPeriod > 255)
    {
        u32BitPeriod = 255;
    }
	
    m_u8TimerPeriod = (uint8_t)u32BitPeriod;
	TimerInit();
}

//---------------------------------------------------------------------------
void SoftwareUART::SendByte( uint8_t u8Byte_ )
{
	// Wait until the byte is off the line
    while (eState != TX_STATE_IDLE) { /* DO NOTHING */ }
		
	// Latch the data-byte 
    u8TX = u8Byte_;
	
	// Set the remaining number of bits to count from the interrupt (-1 for
	// the start bit, which is set in TimerStart()
	u8Count = FRAME_BITS - 1;
    eState = TX_STATE_TX;	
	
	// Start the timer and set the start bit.
    TimerStart();
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerStart(void)
{
	// Convert mapping of prescale factor to the appropriate register 
	// combinations.
    if (m_u16Prescale == 1)
    {
        TCCR0B |= (1 << CS00);
    }
    else if (m_u16Prescale == 8)
    {
        TCCR0B |= (1 << CS01);
    }
    else if (m_u16Prescale == 64)
    {
        TCCR0B |= (1 << CS01) | (1 << CS00);
    }
    else if (m_u16Prescale == 256)
    {
        TCCR0B |= (1 << CS02);
    }
    else if (m_u16Prescale == 1024)
    {
        TCCR0B |= (1 << CS02) || (1 << CS00);
    }
	    		
	TCNT0 = 0;					// Reset counter
	TIFR0 &= ~(1 <<OCF0A);		// Clear interrupt flag if set
    TIMSK0 |= (1 << OCIE0A);	// Enable interrupt
    UART_SET_OUTPUT(0);			// Set Start Bit
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerStop(void)
{
    TCCR0B = 0;					// Stop counting
    TIMSK0 &= ~(1 << OCIE0A);	// Disable interrupt
    TIFR0 &= ~(1 << OCF0A);		// Clear interupt flag
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerInit(void)
{
	// Stop counting
	TCCR0B = 0;
	
	// Clear & disable the timer interrupts
	TIMSK0 = 0;
	TIFR0 = 0;
	
	// Set the timer mode to count-up, and reset when we reach OCR0A.
    TCCR0A = 0;
    TCCR0A |= (1 << WGM01);

	// Set the timer-count corresponding to 1 bit-width.
    OCR0A = m_u8TimerPeriod;
}

//---------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
	if (u8Count == 0)
	{
		// When count reaches zero, signal that we're idle and stop the timer
		SoftwareUART::TimerStop();		
		eState = TX_STATE_IDLE;
	}
	else if (u8Count == 1)
	{		
		// When count reaches one, we've sent all data bits, so send stop bit
		UART_SET_OUTPUT(1);		
		u8Count--;
	}	
	else
	{
		// Send  one bit of data on the line
		UART_SET_OUTPUT(u8TX);
		u8TX >>= 1;
		u8Count--;		
	}
}
