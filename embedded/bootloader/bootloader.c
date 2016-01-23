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
=========================================================================== */
/*!
    \file bootloader.c
    \brief atmega328p Bootloader
    
    \page BOOTLOAD1 The Mark3 Bootloader

    The Mark3 bootloader module implements a custom FunkenSlip-encoded 
    bootloader, which fits in a 1KB bootloader block.  
    
    FunkenSlip encoding is used by Mark2 to provide a robust, common, and
    standards-based communications protocol for host-to-target interaction.
    
    Why use FunkenSlip encoding?
    
    -It's packet-driven.  As a result, the host utility and the target device
     can work together, producing fast, bullet-proof loads.
    
    -FunkenSlip encoding is similar to many popu32ar programming formats, and 
     as a result it's easy to use as a generic abstraction for intel hex,
     S-records, etc.
    
    -Each packet is terminated with a special end-of-packet character, which
     makes it especially easy to parse and decode.
     
    -Each packet contains framing information (channel ID, message size, and
     CRC16) which makes data easy to validate.
     
    -Only devices supporting the Flash-write channel will respond to requests
     to write the flash.
    
    -It's lightweight - we can fit our whole bootloader in under 1KB of code.
    
    Bootloader implementation:
    
    On bootup, we disable interrupts and configure the serial port in busy-
    read/write mode.  We then wait a few seconds to see whether or not a 
    client utility is going to try and flash the device.  If we receive the 
    proper framing messages, we will enter bootloader mode; at which point 
    we will wait for packets until we recieve an EOF message from the host
    flashing utility.  If no attempt to communicate with the bootloader is
    made in the first few seconds, the bootloader will exit, attempting to 
    jump to the main application.  In the event that there is no main
    application, the device will jump right back to the bootloader.
    
    Once in bootloader mode, we wait on FunkenSlip encoded packets.  These
    can be used to either SEEK to a new address in flash, WRITE a packet of
    data to the current flash location, or commit remaining data and reboot
    to the newly-flashed application on EOF.
    
    In the event of an error, an error is returned to the host application
    via the UART, and corrective action can be attempted.  However, in most
    cases, it's probably easiest to reboot the device and attempt to 
    re-flash.  
    
    In this implementation, the device always enters the bootloader - there
    is no special bit that must be held before the bootloader will be invoked.
    While this adds delay, the convenience of always being able to access
    the bootloader is certainly a benefit during development.
    
 */

/*------------------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <avr/wdt.h>

/*------------------------------------------------------------------------- */
/*!
 *  generic types/defines used throughout the bootloader
 */
/*------------------------------------------------------------------------- */
#define uint8_t         unsigned char
#define uint16_t        unsigned short
#define uint32_t         unsigned long
#define bool          unsigned char
#define K_TRUE          1
#define K_FALSE         0

/*------------------------------------------------------------------------- */
/*!
 *  Serial port register configuration.  This is used to abstract-out the 
 *  registers from device-to-device.
 */
/*------------------------------------------------------------------------- */
#define UDR             UDR0
#define u8SRA           UCSR0A
#define u8SRB           UCSR0B
#define u8SRC           UCSR0C
#define RXC             RXC0
#define RXEN            RXEN0
#define TXEN            TXEN0
#define UDRE            UDRE0
#define BAUD_H          UBRR0H
#define BAUD_L          UBRR0L

/*------------------------------------------------------------------------- */
/*!
 *  Define the baud rate that the bootloader will operate at
 */
/*------------------------------------------------------------------------- */
#define BAUD_RATE       ((uint32_t)57600)

/*------------------------------------------------------------------------- */
/*!
 *  Define the frequency that the system is running at.  This MUST match the
 *  actual running frequency, otherwise the serial port won't run at the 
 *  correct baud rate.
 */
/*------------------------------------------------------------------------- */
#define SYSTEM_FREQ     ((uint32_t)16000000)

/*------------------------------------------------------------------------- */
/*!
 *  Buffer definitions - we have two buffers in our bootloader.
 *  
 *  -The serial buffer 
 *       This is where packets transmitted to the device are staged until the
 *       contents can be verified.  Verification in this case involves correct
 *       SLIP decoding, channel matching (Funkenslip framing), sub-msg matching
 *       and 16-bit payload checkusm matching.
 *       
 *  -The page buffer
 *       Once the individual packets have been validated as being correct - the
 *       data is copied into the page buffer.  Once the page-buffer is full
 *       (the size varies between parts), the page buffer is committed to 
 *       FLASH.
 */
#define PAGE_SIZE       (128)				/*!< 64 words on mega328p. */
#define RX_BUF_SIZE     (64)                /*!< Maximum single funkenslip packet size  */

/*------------------------------------------------------------------------- */
/*!
 *  Protocol configuration
 *  
 *  The FunkenSlip protocol defines 256 channels on which messages can be 
 *  addressed.  Only devices which implement certain channels will respond
 *  to messages on those channels without a NACK.  
 *  
 *  This section defines the channel ID used by our bootloader, as well as 
 *  the sub-commands (data-byte 0) used to implement the specific operations
 *  required by the bootloader.
 *  
 *  Speaking of which, we can get away with three simple commands:
 *  
 *  -SEEK-  Set the write pointer to a specific address, generally set on 
 *          startup.  If a partially-written page exists, assume the seek 
 *          will take u16 out of the page currently being written.  Write the 
 *          contents of the partial page to flash, and then set the write 
 *          pointer to the given address
 *  
 *  -WRITE- Write a packet of data to the current write address, committing to 
 *          flash whenever a page boundary is crossed.
 *          
 *  -EOF-   No more data to be transmitted.  Commit partial-pages to flash, and
 *          boot into the newly-flashed application.
 */
#define PROGRAM_CHANNEL			(127)		/*!< FunkenSlip channel to program on */
#define PROGRAM_CMD_SEEK		(0)			/*!< Seek to address */
#define PROGRAM_CMD_WRITE		(1)			/*!< Write current buffer */
#define PROGRAM_CMD_EOF			(2)			/*!< End of file - stop programming  */

/*------------------------------------------------------------------------- */
static uint32_t u32PageAddr;                  /*!< Page address pointer */
static uint8_t u8PageIdx;                   /*!< Index in the data page */
static uint8_t aucPageBuf[PAGE_SIZE];       /*!< Data buffer written to FLASH */
static uint8_t aucSerialBuf[RX_BUF_SIZE];   /*!< Buffer filled with FunkenSlip packets */

/*------------------------------------------------------------------------- */
typedef void (*main_func_t)(void);          /*!< Function pointer type used to jump to app */

/*------------------------------------------------------------------------- */
/*! 
 *  Forward declarations
 */

/*------------------------------------------------------------------------- */
static void     BL_Exit(void); 
static void     Serial_Init(void);
static uint8_t  Serial_Read(void);
static void     Serial_Write(uint8_t u8Byte_);
static bool   Slip_FillPacket(void);
static uint8_t  Slip_DecodePacket(void);
static bool   Serial_ValidatePacket(uint16_t *pu16Len_);
static void		Flash_WritePage(void);

/*------------------------------------------------------------------------- */
/*!
 *  Exit out of the bootloader, jumping to the main application.  Writes
 *  a message to the UART "Booting App", before performing the reboot.
 *  
 *  \fn static void BL_Exit(void)
 */
static void BL_Exit(void)
{
    /* Set a function pointer to the start of the user-app.  */
    main_func_t app_start = (void*)0;
	
    /* Write our farewell message to the UART  */
	Serial_Write('B');
	Serial_Write('o');
	Serial_Write('o');
	Serial_Write('t');
	Serial_Write('i');
	Serial_Write('n');
	Serial_Write('g');
	Serial_Write(' ');
	Serial_Write('A');
	Serial_Write('p');
	Serial_Write('p');
	Serial_Write('\n');

    /* Reboot!  */
    app_start();
}

/*------------------------------------------------------------------------- */
/*!
 *  Initialize the serial port to the default baud rate specified in the 
 *  port configuration.  The bootloader uses a polling/busy-waiting RX and 
 *  TX mechanism (no ISRs), so only basic RX and TX operations are supported.
 *  
 *  \fn static void Serial_Init(void) 
 */
static void Serial_Init(void) 
{
    uint16_t u16BaudTemp = 0;
    
    /* Clear port config */
    u8SRA = 0;
    u8SRB = 0;
    
    /* Set baud rate */
    u16BaudTemp = (uint16_t)(((SYSTEM_FREQ/16)/BAUD_RATE) - 1);
    BAUD_H = (uint8_t)(u16BaudTemp >> 8);
    BAUD_L = (uint8_t)(u16BaudTemp & 0x00FF);
    
    /* Set 8N1 format on the port  */
    u8SRC = 0x06;
    
    /* Enable RX & TX, but no interrupts.  */
    u8SRB = (1 << RXEN) | (1 << TXEN);
}

/*------------------------------------------------------------------------- */
/*!
 *  Read a byte of data from the serial port, returning it to the user.
 *  \fn static uint8_t Serial_Read(void) 
 *  \return A single byte of data read from the serial port
 */
static uint8_t Serial_Read(void) 
{
    while (!(u8SRA & (1 << RXC0))){};
    return UDR;
}

/*------------------------------------------------------------------------- */
/*!
 *  Poll until we recieve the SLIP end-of-packet character (192).  If a 
 *  specified internal (ad-hoc) timeout occurs before receiving a valid
 *  framing character, bail.
 *  \fn static bool Serial_RxPoll(void) 
 *  \return K_TRUE if a SLIP end-of-packet character recieved within the 
 *          predefined tiemout interval, K_FALSE if a timeout occurred.
 */
static bool Serial_RxPoll(void) 
{
    volatile uint32_t u32Timeout = 2000000UL;
    
    while (u32Timeout--) 
    {
        if (u8SRA & (1 << RXC0))
        {
            if (UDR == 192)
            {
                return K_TRUE;
            }
        }
    }
        
    return K_FALSE;
}

/*------------------------------------------------------------------------- */
/*!
 *  Push a byte of data out of the serial port.  Waits until the port is free
 *  before attempting to write the character.
 *  
 *  \fn static void Serial_Write(uint8_t u8Byte_)
 */
static void Serial_Write(uint8_t u8Byte_) 
{
    while (!(u8SRA & (1 << UDRE))){};
    UDR = u8Byte_;
}

/*------------------------------------------------------------------------- */
/*!
 *  Busy-wait until a full packet of data is received.
 *  \fn static bool Slip_FillPacket(void)     
 *  \return K_TRUE if a packet was successfully read, K_FALSE if the packet
 *          failed as a result of overflowing the buffer/garbage data on the
 *          port.
 */
static bool Slip_FillPacket(void) 
{
    uint8_t i = 0;
    uint8_t u8Data;
    while (i < RX_BUF_SIZE)
    {
        u8Data = Serial_Read();
        aucSerialBuf[i] = u8Data;
        
        if (u8Data == 192)
        {
            return K_TRUE;
        }
        i++;
    }
    return K_FALSE;
}
/*------------------------------------------------------------------------- */
/*!
 *  Perform in-place decoding on SLIP data in the serial buffer.
 *  \fn static uint8_t Slip_DecodePacket(void) 
 *  \return Number of characters read
 */
static uint8_t Slip_DecodePacket(void) 
{
    uint8_t i = 0;
    uint8_t *pu8Src = aucSerialBuf;
    uint8_t *pu8Dst = aucSerialBuf;
    
    /* Perform slip decoding in-place in the serial buffer packet */
    while ((*pu8Src != 192) && i < RX_BUF_SIZE)
    {
        if (*pu8Src == 219)
        {
            if (*(pu8Src+1) == 220)
            {
                *pu8Dst = 192;				
            }
            else if (*(pu8Src+1) == 221)
            {
                *pu8Dst = 219;				
            }
            pu8Src++;
        }
        else
        {
            *pu8Dst = *pu8Src;	
        }
        i++;
        pu8Src++;
        pu8Dst++;
    }
    
    *pu8Dst = 192;
		
    return i;
}

/*------------------------------------------------------------------------- */
/*!
 *  Attempt to validate a packet of serial data which has already been 
 *  SLIP decoded.  This function validates the contents of the payload as 
 *  being FunkenSlip encoded data - We search for a correct channel, correct
 *  CRC's, and correct message length before allowing a packet to be valid.
 *  \fn static bool Serial_ValidatePacket(uint16_t *pu16Len_)
 *  \param uint16_t *pu16Len_ Length of packet data returned to the user
 *  \return K_TRUE if packet is valid, K_FALSE if any of the checks fail
 */
static bool Serial_ValidatePacket(uint16_t *pu16Len_) 
{    
    uint8_t u8Channel;
    uint16_t u16CRC_Calc = 0;
    uint16_t u16CRC_Read = 0;
    uint16_t u16Len;
    uint8_t *pu8Data;
    
    u8Channel = aucSerialBuf[0];
    
    /* Ensure the channel is correct  */
	if (u8Channel != PROGRAM_CHANNEL)
    {
	    return K_FALSE;
    }
	
    u16CRC_Calc += aucSerialBuf[0];
    
    /* Read the length out  */
    u16Len = ((uint16_t)aucSerialBuf[1]) << 8;
    u16CRC_Calc += aucSerialBuf[1];
    
    u16Len += (uint16_t)aucSerialBuf[2];
    u16CRC_Calc += aucSerialBuf[2];
    
    /* Length returned to the user is -1 because of the sub-command byte, */
    /* which is part of the actual FunkenSlip data payload. */
    *pu16Len_ = u16Len - 1; 
    
    /* Continue reading through the packet to compute the CRC  */
	pu8Data = &aucSerialBuf[3];
    while (u16Len--)
    {
        u16CRC_Calc += *pu8Data;
        pu8Data++;
    }
    
    /* The CRC is stored at the end of the packet  */
    u16CRC_Read = ((uint16_t)*pu8Data) << 8;
    pu8Data++;
    
    u16CRC_Read += ((uint16_t)*pu8Data);
    
    /* Make sure the read CRC matches the generated CRC */
    if (u16CRC_Read != u16CRC_Calc)
    {
        *pu16Len_ = 0;		
	    return K_FALSE;
    }
    
    return K_TRUE;
}

/*------------------------------------------------------------------------- */
/*!
 *  Shamelessly lifted from the AVR libc docs.  This uses the functions and 
 *  macros defined in boot.h in order to safely commit our page buffer to 
 *  flash.
 *  \fn static void Flash_WritePage(void)
 */
static void Flash_WritePage(void)
{
    uint16_t i;
    uint8_t *pu8Data = aucPageBuf;
	
    eeprom_busy_wait();

    boot_page_erase(u32PageAddr);
    boot_spm_busy_wait();      /* Wait until the memory is erased.  */

    for (i=0; i<PAGE_SIZE; i+=2)
    {
        /* Set up little-endian word.  */
        uint16_t w = *pu8Data++;
        w += ((uint16_t)(*pu8Data++)) << 8;

        boot_page_fill (u32PageAddr + i, w);
    }

    boot_page_write (u32PageAddr);     /* Store buffer in flash page.  */
    boot_spm_busy_wait();       /* Wait until the memory is written.  */

    /* Reenable RWW-section again. We need this if we want to jump back  */
    /* to the application after bootloading.  */
    boot_rww_enable ();
}

/*------------------------------------------------------------------------- */
/*!
 *  Copy data in from the serial buffer to the page buffer.  Whenever a page
 *  buffer is full, commit the page to flash, and start the next.
 *  \fn static bool Flash_WriteBuffer(uint16_t u16Len_)
 *  \return K_TRUE on success, K_FALSE on failure
 */
static bool Flash_WriteBuffer(uint16_t u16Len_) 
{
    uint8_t u8Idx = 4; /* Size of the header...  */
    
    /* Write from the serial buffer to the flash staging buffer  */
    while (u16Len_--)
    {
        aucPageBuf[u8PageIdx] = aucSerialBuf[u8Idx++];
        
        u8PageIdx++;
        if (u8PageIdx == PAGE_SIZE)
        {
            /* Write the page of data to flash...         */
            Flash_WritePage();
			
            /* Update indexes/pages.  */
            u8PageIdx = 0;
            u32PageAddr += PAGE_SIZE;			
        }
    }
    
    return K_TRUE;
}

/*------------------------------------------------------------------------- */
/*!
 *  Commit a partial-page of data to flash.  This completes the page buffer 
 *  with 0xFF bytes, before running Flash_WritePage() to seal the deal.
 *  \fn static void Flash_WritePartialPage(void)
 */
static void Flash_WritePartialPage(void)
{
	if (u8PageIdx != 0)
	{
		while (u8PageIdx < PAGE_SIZE)
		{
			aucPageBuf[u8PageIdx] = 0xFF;
			u8PageIdx++;
		}
		Flash_WritePage();	
	}
}

/*------------------------------------------------------------------------- */
int main(void) 
{
    /* Ensure interrupts are disabled when running the bootloader...  */
    cli();

    /* Clear the watchdog timer...  */
	{
		volatile uint8_t u8Temp = MCUSR;
		MCUSR = 0;
		WDTCSR |= (1 << WDCE) | (1 << WDE);
		WDTCSR = 0;
        u8Temp = u8Temp;
	}

    /* Start off by initializing the serial port  */
    Serial_Init();

    /* Send a banner message, indicating we're in the Mark3 Boot Loader  */
	Serial_Write('M');
	Serial_Write('a');
	Serial_Write('r');
	Serial_Write('k');
    Serial_Write('3');
	Serial_Write('B');
	Serial_Write('L');
	Serial_Write('\n');
		
#if 1
    /* Check to see if we're getting our start character to begin using the BL  */
    if (!Serial_RxPoll())
    {
        /* Timed out - exit the bootloader  */
        BL_Exit();
    }
    
    /* Acknowledge the request to start programming.  */
	Serial_Write(69);	
#endif

    /* Main programming loop.  Program until we can't program no more!  */
	while (1)
	{
		uint16_t u16Len;
		
        /* Wait until we receive a packet of data.  */
		Slip_FillPacket();		
		
        /* Decode the serial buffer  */
		if (!Slip_DecodePacket())
		{
			Serial_Write('D');
			continue;
		}
		
        /* Make sure the packet is VALID before trying to operate on it.  */
		if (!Serial_ValidatePacket(&u16Len))
		{
			Serial_Write('V');
			continue;
		}
		
        /* Figure out what action to take based on the command field...  */
		if (aucSerialBuf[3] == PROGRAM_CMD_SEEK)
		{
            /* Seek to new address...  */
			uint16_t u16NewAddr = ((uint16_t)aucSerialBuf[4]) << 8;
			u16NewAddr += aucSerialBuf[5];
			Flash_WritePartialPage();
			u32PageAddr = (uint32_t)u16NewAddr;
			u8PageIdx = 0;			
		}	
		else if (aucSerialBuf[3] == PROGRAM_CMD_WRITE)
		{
            /* Write contents of buffer to staging buffer, then flash. */
			Flash_WriteBuffer(u16Len);	
		}
		else if (aucSerialBuf[3] == PROGRAM_CMD_EOF)
		{
            /* End of file.  Commit current page to flash (if non-empty) and boot to  */
            /* application, if possible.  */
			Flash_WritePartialPage();
			BL_Exit();
		}			
		else
		{
            /* error, invalid command   */
			Serial_Write('I');
			continue;
		}
		
        /* If we get here, the packet was valid.  */
		Serial_Write(69);
	}		
    
    /* Return to app on exit.  */
    BL_Exit();
}
