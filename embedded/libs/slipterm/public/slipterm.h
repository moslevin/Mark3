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
    \file slipterm.h
    
    \brief Serial debug interface using SLIP serial, and Funkenslip serial
           port multiplexing
 */

#include "kerneltypes.h"
#include "driver.h"
#include "slip.h"

//---------------------------------------------------------------------------
#define SEVERITY_DEBUG                4
#define SEVERITY_INFO                3
#define SEVERITY_WARN                2
#define SEVERITY_CRITICAL            1
#define SEVERITY_CATASTROPHIC        0

//---------------------------------------------------------------------------
#ifndef __SLIPTERM_H__
#define __SLIPTERM_H__

/*!
    Class implementing a simple debug terminal interface.  This is
    useful for printf style debugging.
 */
class SlipTerm
{
public:
    /*!
     *  \brief Init
     *
     *  Initialize the terminal by opening a handle to the serial interface
     *  attached at /dev/tty.  Must be called prior to using the print
     *  functionality.
     */
    void Init();
    
    /*!
     *  \brief PrintLn
     *
     *  Print a string of text to the SLIP interface, multiplexed using the
     *  FunkenSlip terminal channel.
     *
     *  \param szLine_ String to print
     */
    void PrintLn( const char *szLine_ );

    /*!
     *  \brief PrintLn
     *
     *  Print a string of text to the SLIP interface, but only if the 
     *  current logging verbosity level is greater than or equal to the 
     *  specified message severity
     *  
     *  \param u8Severity_ Message severity level, 0 = highest severity
     *  \param szLine_ String to print
     */
    void PrintLn( uint8_t u8Severity_, const char *szLine_ );
    
    /*!        
     *  \brief SetVerbosity
     *
     *  Set the logging verbosity level - the minimum severity level that
     *  will be printed to the terminal.  The higher the number, the 
     *  more chatty the output.
     */
    void SetVerbosity( uint8_t u8Level_ ) { m_u8Verbosity = u8Level_; }
private:
    /*!
     *  \brief StrLen
     *
     *  Quick 'n' dirty StrLen functionality used for printing the string.
     *  
     *  \return Length of the string (in bytes)
     */
    uint16_t StrLen( const char *szString_ );
    
    uint8_t m_u8Verbosity;     //!< Verbosity level.  Messages with a severity 
                            //!< level greater than this Are not displayed.
    
    Slip m_clSlip;        //!< Slip object that this module interfaces with
};

#endif
