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
=========================================================================== */
/*!
    \file tracebuffer.h
    \brief Kernel trace buffer class declaration
    
    Global kernel trace-buffer.  used to instrument the kernel with lightweight
    encoded print statements.  If something goes wrong, the tracebuffer can be
    examined for debugging purposes.  Also, subsets of kernel trace information
    can be extracted and analyzed to provide information about runtime 
    performance, thread-scheduling, and other nifty things in real-time.  
 */
#ifndef __TRACEBUFFER_H__
#define __TRACEBUFFER_H__

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "writebuf16.h"

#if KERNEL_USE_DEBUG && !KERNEL_AWARE_SIMULATION

#define TRACE_BUFFER_SIZE           (16)

/*!
 *  Global tracebuffer class declaration
 */
class TraceBuffer
{
public:
    /*!
     *  \fn void Init();
     *  
     *  Initialize the tracebuffer before use.      
     */
    static void Init();
    
    /*!
     *  \fn static uint16_t Increment();
     *  
     *  Increment the tracebuffer's atomically-incrementing index.
     *  
     *  \return a 16-bit index
     */
    static uint16_t Increment();
    
    /*!
     *  \fn static void Write( uint16_t *pu16Data_, uint16_t u16Size_ )
     *  
     *  Write a packet of data to the global tracebuffer.
     *  
     *  \param pu16Data_ Pointer to the source data buffer to copy to the trace buffer
     *  \param u16Size_ Size of the source data buffer in 16-bit words.
     */
    static void Write( uint16_t *pu16Data_, uint16_t u16Size_ );
    
    /*! 
     *  \fn void SetCallback( WriteBufferCallback pfCallback_ )
     *  
     *  Set a callback function to be triggered whenever the tracebuffer
     *  hits the 50% point, or rolls over.
     *  
     *  \param pfCallback_ Callback to assign
     */ 
    void SetCallback( WriteBufferCallback pfCallback_ )
        { m_clBuffer.SetCallback( pfCallback_ ); }
private:

    static WriteBuffer16 m_clBuffer;        //!< Object used to implement the tracebuffer
    static volatile uint16_t m_u16Index;        //!< Current print index    
    static uint16_t m_au16Buffer[ (TRACE_BUFFER_SIZE / sizeof( uint16_t )) ];   //!< Data buffer
};

#endif //KERNEL_USE_DEBUG

#endif 
