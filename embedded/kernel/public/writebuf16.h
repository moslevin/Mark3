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
	\file writebuf16.h

	\brief Thread-safe circular buffer implementation with 16-bit elements
*/

#ifndef __WRITEBUF16_H__
#define __WRITEBUF16_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#if KERNEL_USE_DEBUG && !KERNEL_AWARE_SIMULATION

/*!
	Function pointer type used to define a callback handler for when
	the circular buffer reaches 50% capacity, 
*/
typedef void (*WriteBufferCallback)( uint16_t *pu16Data_, uint16_t u16Size_ );

/*!
	This class is used to provide a general-purpose, fully thread-safe circular
	buffer implementation which can be used for creating tracebuffers, data 
	logging queues, transaction queues, etc.  We use it for implementing
	a debug print journal.
*/
class WriteBuffer16
{
public:
	/*!
		\fn void SetBuffers( uint16_t *pu16Data_, uint16_t u16Size_ )
		
		Assign the data to be used as storage for this circular buffer
		
        \param pu16Data_ Pointer to the array of data to be managed
				as a circular buffer by this object.

		\param u16Size_ Size of the buffer in 16-bit elements
	*/
	void SetBuffers( uint16_t *pu16Data_, uint16_t u16Size_ )
	{ 
        m_pu16Data = pu16Data_;
		m_u16Size = u16Size_; 
		m_u16Head = 0;
		m_u16Tail = 0;
	}
	
	/*!
		\fn void SetCallback( WriteBufferCallback pfCallback_ ) 
		
		Set the callback function to be called when the buffer hits 
		50% of its capacity, and again when the buffer rolls over 
		completely.
		
		\param pfCallback_ Function pointer to call whenever the 
				buffer has reached 50% capacity, or has rolled over
				completely.
	*/
	void SetCallback( WriteBufferCallback pfCallback_ ) 
		{ m_pfCallback = pfCallback_; }
	
	/*!
		\fn void WriteData( uint16_t *pu16Buf_, uint16_t u16Len_ )
		
		Write an array of values to the circular buffer
		
        \param pu16Buf_ Source data array to write to the circular buffer
		\param u16Len_ Length of the source data array in 16-bit elements
	*/
	void WriteData( uint16_t *pu16Buf_, uint16_t u16Len_ );
	
	/*!
        \fn void WriteVector( uint16_t **ppu16Buf_, uint16_t *pu16Len_, uint8_t u8Count_)
		
		Write a multi-part vector to the circular buffer
		
        \param ppu16Buf_ Pointer to the array of source data pointers
        \param pu16Len_ Array of buffer lengths
		\param u8Count_ Number of source-data arrays to write to the buffer
	*/
    void WriteVector( uint16_t **ppu16Buf_, uint16_t *pu16Len_, uint8_t u8Count_);

private:
    uint16_t *m_pu16Data;				//!< Pointer to the circular buffer data
	
	volatile uint16_t m_u16Size;			//!< Size of the buffer
	volatile uint16_t m_u16Head;			//!< Current head element (where data is written)
	volatile uint16_t m_u16Tail;			//!< Current tail element (where data is read)
	
	WriteBufferCallback m_pfCallback;	//!< Buffer callback function
};
#endif

#endif
