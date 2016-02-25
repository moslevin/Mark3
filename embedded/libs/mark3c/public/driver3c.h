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

    \file   driver.h    

    \brief  Driver abstraction framework for Mark3C

*/


#include "mark3cfg.h"
#include <stdint.h>

#ifndef __DRIVER3C_H__
#define __DRIVER3C_H__

#if KERNEL_USE_DRIVER

#ifdef __cplusplus
    extern "C" {
#endif

//---------------------------------------------------------------------------
typedef uint8_t (*OpenFunc_t)(void *pvCtx_);
typedef uint8_t (*CloseFunc_t)(void *pvCtx_);
typedef uint16_t (*ReadFunc_t)(void *pvCtx_, uint16_t usSize_, uint8_t *pucData_ );
typedef uint16_t (*WriteFunc_t)(void *pvCtx_, uint16_t usSize_, uint8_t *pucData_ );
typedef uint16_t (*ControlFunc_t)(void *pvCtx, uint16_t usEvent_, uint16_t usInSize_, uint8_t *pucIn_, uint16_t usOutSize_, uint8_t *pucOut_);

//---------------------------------------------------------------------------
typedef struct
{
    OpenFunc_t        pfOpen;
    CloseFunc_t       pfClose;
    ReadFunc_t        pfRead;
    WriteFunc_t       pfWrite;
    ControlFunc_t     pfControl;
} DriverVTable_t;

//---------------------------------------------------------------------------
typedef struct _Driver
{
    // Inherit from LinkListNode -- must go first
    struct _Driver  *next;

    DriverVTable_t  *pstVTable;
    const char      *szName;
} Driver_t;

//---------------------------------------------------------------------------
/*!
    Base device-driver object used in hardware abstraction.  All other device
    drivers inherit from this object
*/

/*!
    \fn void Init()

    Initialize a driver, must be called prior to use
*/
void Driver_Init( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
    \fn uint8_t Open()

    Open a device driver prior to use.

    \return Driver-specific return code, 0 = OK, non-0 = error
*/
uint8_t Driver_Open( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
    \fn uint8_t Close()

    Close a previously-opened device driver.

    \return Driver-specific return code, 0 = OK, non-0 = error
*/
uint8_t Driver_Close( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
    \fn uint16_t Read( uint16_t usBytes_,
                             uint8_t *pucData_)

    Read a specified number of bytes from the device into a specific buffer.
    Depending on the driver-specific implementation, this may be a number
    less than the requested number of bytes read, indicating that there
    there was less input than desired, or that as a result of buffering,
    the data may not be available.

    \param usBytes_ Number of bytes to read (<= size of the buffer)
    \param pucData_ Pointer to a data buffer receiving the read data

    \return Number of bytes actually read
*/
uint16_t Driver_Read( Driver_t *pstDriver_, uint16_t usSize_, uint8_t *pucData_ );

//---------------------------------------------------------------------------
/*!
    \fn uint16_t Write( uint16_t usBytes_,
                              uint8_t *pucData_)

    Write a payload of data of a given length to the device.
    Depending on the implementation of the driver, the amount of data
    written to the device may be less than the requested number of
    bytes.  A result less than the requested size may indicate that
    the device buffer is full, indicating that the user must retry
    the write at a later point with the remaining data.

    \param usBytes_ Number of bytes to write (<= size of the buffer)
    \param pucData_ Pointer to a data buffer containing the data to write

    \return Number of bytes actually written
*/
uint16_t Driver_Write( Driver_t *pstDriver_, uint16_t usSize_, uint8_t *pucData_ );

//---------------------------------------------------------------------------
/*!
    \fn uint16_t Control( uint16_t usEvent_,
                                void *pvDataIn_,
                                uint16_t usSizeIn_,
                                void *pvDataOut_,
                                uint16_t usSizeOut_ )

    This is the main entry-point for device-specific io and control
    operations.  This is used for implementing all "side-channel"
    communications with a device, and any device-specific IO
    operations that do not conform to the typical POSIX
    read/write paradigm.  Use of this funciton is analagous to
    the non-POSIX (yet still common) devctl() or ioctl().

    \param usEvent_ Code defining the io event (driver-specific)
    \param pvDataIn_ Pointer to the intput data
    \param usSizeIn_ Size of the input data (in bytes)
    \param pvDataOut_ Pointer to the output data
    \param usSizeOut_ Size of the output data (in bytes)

    \return Driver-specific return code, 0 = OK, non-0 = error
*/
uint16_t Driver_Control( Driver_t *pstDriver_, uint16_t usEvent_, uint16_t usInSize_, uint8_t *pucIn_, uint16_t usOutSize_, uint8_t *pucOut_);

//---------------------------------------------------------------------------
/*!
    \fn void SetName( const char *pcName_ )

    Set the path for the driver.  Name must be set prior to
    access (since driver access is name-based).

    \param pcName_ String constant containing the device path
*/
void Driver_SetName( Driver_t *pstDriver_, const char *pcName_ );

//---------------------------------------------------------------------------
/*!
    \fn const char *GetPath()

    Returns a string containing the device path.

    \return pcName_ Return the string constant representing the device path
*/
const char *Driver_GetPath( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
    List of Driver objects used to keep track of all device drivers in the 
    system.  By default, the list contains a single entity, "/dev/null".
*/
void DriverList_Init( void );

//---------------------------------------------------------------------------
Driver_t *DriverList_FindByPath( const char *pcPath );

//---------------------------------------------------------------------------
void DriverList_Add( Driver_t *pstDriver_ );

#ifdef __cplusplus
    }
#endif

#endif //KERNEL_USE_DRIVER

#endif

