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
#define SW_UART_TX_BIT     (0)
#define SW_UART_TX_DIR     DDRB
#define SW_UART_TX_OUT     PORTB

//---------------------------------------------------------------------------
#define SW_UART_RX_BIT	   (3)
#define SW_UART_RX_DIR	   DDRD
#define SW_UART_RX_IN	   PIND
 
//---------------------------------------------------------------------------
#define SYSTEM_FREQ		   (16000000)

//---------------------------------------------------------------------------
#define FRAME_BITS		(10)	// 8 data bits, 1 start, 1 stop bit.

//---------------------------------------------------------------------------
#define UART_SET_OUTPUT(x)   ( ((x) & 1) ? ( SW_UART_TX_OUT |= (1 << SW_UART_TX_BIT) ) : ( SW_UART_TX_OUT &= ~(1 <<SW_UART_TX_BIT) ) )
//---------------------------------------------------------------------------
typedef enum
{
    UART_STATE_IDLE = 0,	// No data currently being sent
    UART_STATE_TX,			// Data transmit in progress
	UART_STATE_RX
} UARTState_t;

//---------------------------------------------------------------------------
static volatile uint8_t		   u8TX;		// Current transmit byte register
static volatile uint8_t		   u8RX;		// Current received-byte register
static volatile uint8_t		   u8Count;		// Number of bits left to shift out
static volatile bool		   bPendingRX;	// Whether or not there is a pending RX byte
static volatile UARTState_t    eState;		// Current transmit state-machine state.

//---------------------------------------------------------------------------
void SoftwareUART::Init( uint32_t u32Baud_ )
{
    // Baudrate = bits / second
    // Bit Time = 1 / baudrate = second / bit
    // CLK Period = Bit Time (seconds / bit) * peripheral clock speed (cycles / second)

	// Setup the TX pin...
	SW_UART_TX_DIR |= (1 << SW_UART_TX_BIT);
	SW_UART_TX_OUT |= (1 << SW_UART_TX_BIT);
	
	// Setup the RX pin...
	SW_UART_RX_DIR &= ~(1 << SW_UART_RX_BIT);
	PORTD |= 0x08; // Pullup enabled...
	
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
	bPendingRX = false;
	
	TimerInit();
	RxInit();
}

//---------------------------------------------------------------------------
void SoftwareUART::SendByte( uint8_t u8Byte_ )
{
	// Wait until the byte is off the line
    while (eState != UART_STATE_IDLE) { /* DO NOTHING */ }
		
	// Latch the data-byte 
    u8TX = u8Byte_;
	
	// Set the remaining number of bits to count from the interrupt (-1 for
	// the start bit, which is set in TimerStart()
	u8Count = FRAME_BITS - 1;
    eState = UART_STATE_TX;	
	
	// Disable the RX interrupt path while we're sending data
	EIFR |= (1 << INTF1);
	EIMSK &= ~(1 << INT1);
	
	// Start the timer 
    TimerStart();
	
	// Set the start bit.
	UART_SET_OUTPUT(0);
	
	while (eState != UART_STATE_IDLE) { };	
}

//---------------------------------------------------------------------------
uint8_t SoftwareUART::ReceiveByte(void)
{	
	// There's a byte already pending - take it.
	if (UART_STATE_IDLE && bPendingRX)	
	{
		bPendingRX = false;
		return u8RX;	
	}
	
	// Block until there's a byte pending.
	while (UART_STATE_RX != eState){};
	while (UART_STATE_RX == eState){};
		
	return u8RX;
}
//---------------------------------------------------------------------------
void SoftwareUART::TimerStart(void)
{	
	TCNT0 = 0;					// Reset counter
	TIFR0 |= (1 << OCF0A);		// Clear interrupt flag
    TIMSK0 |= (1 << OCIE0A);	// Enable interrupt
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerStop(void)
{
    TIMSK0 &= ~(1 << OCIE0A);	// Disable interrupt
    TIFR0 |= (1 << OCF0A);		// Clear interrupt flag
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerInit(void)
{
	// Stop counting
	TCCR0B = 0;
	
	// Clear & disable the timer interrupts
	TIMSK0 = 0;
	TIFR0 |= (1 << OCF0A);		// Clear interrupt flag
	
	// Set the timer mode to count-up, and reset when we reach OCR0A.
    TCCR0A = 0;
    TCCR0A |= (1 << WGM01);

	// Set the timer-count corresponding to 1 bit-width.
    OCR0A = m_u8TimerPeriod;
	
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
}

//---------------------------------------------------------------------------
void SoftwareUART::RxInit(void)
{
	// Falling edge on INT1 causes interrupt
	// This is us sensing the START bit.
	EICRA = (1 << ISC11);
		
	// Clear any previously-pending interrupts on INTF1
	EIFR &= ~(1 <<INTF1);
		
	// Enable the RX interrupt
	EIMSK |= (1 << INT1);
}

//---------------------------------------------------------------------------
ISR(INT1_vect)
{
	// Beginning of a stop-bit detected.  Set up a timer to 
	// sample the bits, beginning at 1.5x the timer period.		
	
	// Short-cycle the first timer interval to ensure we get 1.5x bit
	// periods
	TCNT0 = OCR0A >> 1;
	
	// Number of interrupts expected for this bit.
	u8Count = 10; // Start bit + 0.5, bits 1-8, Stop bit.
	u8RX = 0;
	
	// Set the RX state, so that nobody can transmit while we're attempting
	// to receive data.
	eState = UART_STATE_RX;
	
	// Disable the interrupt -- we'll use the timer for sampling.
	EIMSK &= ~(1 << INT1);

	TIFR0 |= (1 << OCF0A);		// Clear interrupt flag if set
	TIMSK0 |= (1 << OCIE0A);	// Enable interrupt
}

//---------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
	if (UART_STATE_TX == eState)
	{
		if (u8Count == 0)
		{
			// When count reaches zero, signal that we're idle and stop the timer
			SoftwareUART::TimerStop();
			eState = UART_STATE_IDLE;
			
			EIFR |= (1 << INTF1);
			EIMSK |= (1 << INT1);
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
	else if (UART_STATE_RX == eState)
	{				
		if (u8Count == 1)
		{
			SoftwareUART::TimerStop();
			bPendingRX = true;			
			eState = UART_STATE_IDLE;	
			
			EIFR |= (1 << INTF1);
			EIMSK |= (1 << INT1);		
		}
		else if (u8Count < 10) // Ignore stop bit (u8Count == 10)
		{	
			// Shift existing bits in the RX register DOWN.		
			u8RX >>= 1;
			
			if (PIND & 0x08)
			{
				// Shift bit into the TOP of the register.
				u8RX |= 0x80;
			}								
		}
		u8Count--;		
	}
}
