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
// The TX-bit can be moved to *any* GPIO.
//---------------------------------------------------------------------------
#define SW_UART_TX_BIT       (0)
#define SW_UART_TX_DIR       DDRB
#define SW_UART_TX_OUT       PORTB

//---------------------------------------------------------------------------
// The RX-bit is tied to the same GPIO that triggers the RX interrupt.
// This can also be any GPIO that supports either EXT interrupts, or PCINT
// interrupts.  Although, using PCINT interrupts, will require remapping of
// the INT flags in the following section
//---------------------------------------------------------------------------
#define SW_UART_RX_BIT       (3)
#define SW_UART_RX_DIR       DDRD
#define SW_UART_RX_OUT       PORTD
#define SW_UART_RX_IN        PIND
 
//---------------------------------------------------------------------------
// Interrupt used to detect the beginning of a byte (start bit).  Must 
// correspond to the same GPIO used above.
//---------------------------------------------------------------------------
#define SW_UART_RX_INT             INT1
#define SW_UART_RX_MASK            EIMSK
#define SW_UART_RX_FLAG            EIFR
#define SW_UART_RX_MASK_BIT        INT1
#define SW_UART_RX_FLAG_BIT        INTF1
#define SW_UART_RX_EDGE            EICRA
#define SW_UART_RX_EDGE_BIT        (1 << ISC11)

//---------------------------------------------------------------------------
#define SW_UART_RX_INT_DISABLE()    \
{ \
    SW_UART_RX_FLAG |= (1 << SW_UART_RX_FLAG_BIT); \
    SW_UART_RX_MASK &= ~(1 << SW_UART_RX_MASK_BIT); \
}

//---------------------------------------------------------------------------
#define SW_UART_RX_INT_ENABLE()    \
{ \
    SW_UART_RX_FLAG |= (1 << SW_UART_RX_FLAG_BIT); \
    SW_UART_RX_MASK |= (1 << SW_UART_RX_MASK_BIT); \
}

//---------------------------------------------------------------------------
// These are the Timer registers used to control the timing of both the RX
// *and* the TX channels.
//---------------------------------------------------------------------------
#define SW_UART_TIMER_COUNT      TCNT0
#define SW_UART_TIMER_MATCH      OCR0A
#define SW_UART_TIMER_INT        TIFR0
#define SW_UART_TIMER_MASK       TIMSK0
#define SW_UART_TIMER_MODE       TCCR0A
#define SW_UART_TIMER_MODE_BIT   (1 << WGM01)
#define SW_UART_TIMER_INT_BIT    OCF0A
#define SW_UART_TIMER_MASK_BIT   OCIE0A
#define SW_UART_TIMER_PS         TCCR0B
#define SW_UART_TIMER_PS1        (1 << CS00)
#define SW_UART_TIMER_PS8        (1 << CS01)
#define SW_UART_TIMER_PS64       ((1 << CS01) | (1 << CS00))
#define SW_UART_TIMER_PS256      ((1 << CS02))
#define SW_UART_TIMER_PS1024     ((1 << CS02) || (1 << CS00))

//---------------------------------------------------------------------------
#define SW_UART_TIMER_INT_DISABLE()    \
{ \
    SW_UART_TIMER_INT |= (1 << SW_UART_TIMER_INT_BIT); \
    SW_UART_TIMER_MASK &= ~(1 << SW_UART_TIMER_MASK_BIT); \
}

//---------------------------------------------------------------------------
#define SW_UART_TIMER_INT_ENABLE()    \
{ \
    SW_UART_TIMER_INT |= (1 << SW_UART_TIMER_INT_BIT); \
    SW_UART_TIMER_MASK |= (1 << SW_UART_TIMER_MASK_BIT); \
}

//---------------------------------------------------------------------------
#define SYSTEM_FREQ       (16000000)

//---------------------------------------------------------------------------
#define FRAME_BITS        (10)    // 8 data bits, 1 start, 1 stop bit.

//---------------------------------------------------------------------------
#define UART_SET_OUTPUT(x)   ( ((x) & 1) ? ( SW_UART_TX_OUT |= (1 << SW_UART_TX_BIT) ) : ( SW_UART_TX_OUT &= ~(1 <<SW_UART_TX_BIT) ) )
//---------------------------------------------------------------------------
typedef enum
{
    UART_STATE_IDLE = 0,      // No data currently being sent
    UART_STATE_TX,            // Data transmit in progress
    UART_STATE_RX             // Data receive in progress
} UARTState_t;

//---------------------------------------------------------------------------
static volatile uint8_t        u8TX;        // Current transmit byte register
static volatile uint8_t        u8RX;        // Current received-byte register
static volatile uint8_t        u8Count;     // Number of bits left to shift out
static volatile bool           bPendingRX;  // Whether or not there is a pending RX byte
static volatile UARTState_t    eState;      // Current transmit state-machine state.

//---------------------------------------------------------------------------
void SoftwareUART::Init( uint32_t u32Baud_ )
{
    // Baudrate = bits / second
    // Bit Time = 1 / baudrate = second / bit
    // CLK Period = Bit Time (seconds / bit) * peripheral clock speed (cycles / second)

    // Setup the TX pin...
    SW_UART_TX_DIR |= (1 << SW_UART_TX_BIT);
    SW_UART_TX_OUT |= (1 << SW_UART_TX_BIT);

    // Setup the RX pin as an input with pullups enabled.
    SW_UART_RX_DIR &= ~(1 << SW_UART_RX_BIT);
    SW_UART_RX_OUT |= (1 << SW_UART_RX_BIT);

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

    // Disable the RX interrupt path while we're sending data (UART is half-duplex)
    SW_UART_RX_INT_DISABLE();

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

    bPendingRX = false;
    return u8RX;
}
//---------------------------------------------------------------------------
void SoftwareUART::TimerStart(void)
{
    SW_UART_TIMER_COUNT = 0;
    SW_UART_TIMER_INT_ENABLE();
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerStop(void)
{
    SW_UART_TIMER_INT_DISABLE();
}

//---------------------------------------------------------------------------
void SoftwareUART::TimerInit(void)
{
    // Clear & disable the timer interrupts
    SW_UART_TIMER_INT_DISABLE();

    // Set the timer mode to count-up, and reset when we reach OCR0A.
    SW_UART_TIMER_MODE = 0;
    SW_UART_TIMER_MODE |= SW_UART_TIMER_MODE_BIT;

    // Set the timer-count corresponding to 1 bit-width.
    SW_UART_TIMER_MATCH = m_u8TimerPeriod;

    // Convert mapping of prescale factor to the appropriate register
    // combinations.
    if (m_u16Prescale == 1)
    {
        SW_UART_TIMER_PS |= SW_UART_TIMER_PS1;
    }
    else if (m_u16Prescale == 8)
    {
        SW_UART_TIMER_PS |= SW_UART_TIMER_PS8;
    }
    else if (m_u16Prescale == 64)
    {
        SW_UART_TIMER_PS |= SW_UART_TIMER_PS64;
    }
    else if (m_u16Prescale == 256)
    {
        SW_UART_TIMER_PS |= SW_UART_TIMER_PS256;
    }
    else if (m_u16Prescale == 1024)
    {
        SW_UART_TIMER_PS |= SW_UART_TIMER_PS1024;
    }
}

//---------------------------------------------------------------------------
void SoftwareUART::RxInit(void)
{
    // Falling edge on INT1 causes interrupt
    // This is us sensing the START bit.
    SW_UART_RX_EDGE |= SW_UART_RX_EDGE_BIT;
    SW_UART_RX_INT_ENABLE();
}

//---------------------------------------------------------------------------
ISR(INT1_vect)
{
    // Beginning of a stop-bit detected.  Set up a timer to
    // sample the bits, beginning at 1.5x the timer period.
    // Short-cycle the first timer interval to ensure we get 1.5x bit
    // periods
    SW_UART_TIMER_COUNT = (SW_UART_TIMER_MATCH + 1) >> 1;

    // Set the RX state, so that nobody can transmit while we're attempting
    // to receive data.
    eState = UART_STATE_RX;

    // Number of interrupts expected for this bit.
    // Start bit + 0.5, bits 1-8, Stop bit.
    u8Count = 10;
    u8RX = 0;
    bPendingRX = false;

    // Disable the edge-triggered interrupt -- the bits are read by the timer
    // at fixed intervals.
    SW_UART_RX_INT_DISABLE();

    // Re-enable the timer interrupts.
    SW_UART_TIMER_INT_ENABLE();
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

            SW_UART_RX_INT_ENABLE();
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

            SW_UART_RX_INT_ENABLE();
        }
        else if (u8Count < 10) // Ignore stop bit (u8Count == 10)
        {
            // Shift existing bits in the RX register DOWN.
            u8RX >>= 1;

            if (SW_UART_RX_IN & (1 << SW_UART_RX_BIT))
            {
                // Shift bit into the TOP of the register.
                u8RX |= 0x80;
            }
        }
        u8Count--;
    }
}
