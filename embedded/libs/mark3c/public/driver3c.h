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
    struct _Driver  *next;      //!< Linked-list node -- must go first

    DriverVTable_t  *pstVTable; //!< Pointer to the VTable object for this driver-type
    const char      *szName;    //!< Driver's name/path variable
} Driver_t;

//---------------------------------------------------------------------------
/*!
    Base device-driver object used in hardware abstraction.  All other device
    drivers inherit from this object
*/

//---------------------------------------------------------------------------
/*!
 * \brief Driver_Init Initialize a driver, must be called prior to use
 * \param pstDriver_  Pointer to the driver object to initialize.
 */
void Driver_Init( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
 * \brief Driver_Open Open a device driver, after which it can be used
 * \param pstDriver_ Pointer to the driver object to open
 * \return Driver-specific return code, 0 = OK, non-0 = error
 */
uint8_t Driver_Open( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
 * \brief Driver_Close Close a previously opened driver
 * \param pstDriver_ Pointer to the driver object to close
 * \return Driver-specific return code, 0 = OK, non-0 = error
 */
uint8_t Driver_Close( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
 * \brief Driver_Read
 *
 * Read a specified number of bytes from the device into a specific buffer.
 * Depending on the driver-specific implementation, this may be a number
 * less than the requested number of bytes read, indicating that there
 * there was less input than desired, or that as a result of buffering,
 * the data may not be available.
 *
 * \param pstDriver_ Pointer to the driver to read from
 * \param usSize_    Number of bytes to read (<= size of the buffer)
 * \param pucData_   Pointer to a data buffer receiving the read data
 * \return Number of bytes read from the device.
 */
uint16_t Driver_Read( Driver_t *pstDriver_, uint16_t usSize_, uint8_t *pucData_ );

//---------------------------------------------------------------------------
/*!
 * \brief Driver_Write
 *
 *  Write a payload of data of a given length to the device.
 *  Depending on the implementation of the driver, the amount of data
 *  written to the device may be less than the requested number of
 *  bytes.  A result less than the requested size may indicate that
 *  the device buffer is full, indicating that the user must retry
 *  the write at a later point with the remaining data.
 *
 * \param pstDriver_ Pointer to the driver object to write to
 * \param usSize_   Number of bytes to write (<= size of the buffer)
 * \param pucData_  Pointer to a data buffer containing the data to write
 * \return
 */
uint16_t Driver_Write( Driver_t *pstDriver_, uint16_t usSize_, uint8_t *pucData_ );

//---------------------------------------------------------------------------
/*!
 * \brief Driver_Control
 *
 * This is the main entry-point for device-specific io and control
 * operations.  This is used for implementing all "side-channel"
 * communications with a device, and any device-specific IO
 * operations that do not conform to the typical POSIX
 * read/write paradigm.  Use of this funciton is analagous to
 * the non-POSIX (yet still common) devctl() or ioctl().
 *
 * \param pstDriver_ Pointer to the driver object to manipulate
 * \param usEvent_   Code defining the io event (driver-specific)
 * \param usInSize_  Size of the input data (in bytes)
 * \param pucIn_     Pointer to the intput data
 * \param usOutSize_ Size of the output data (in bytes)
 * \param pucOut_    Pointer to the output data
 * \return
 */
uint16_t Driver_Control( Driver_t *pstDriver_, uint16_t usEvent_, uint16_t usInSize_, uint8_t *pucIn_, uint16_t usOutSize_, uint8_t *pucOut_);

//---------------------------------------------------------------------------
/*!
 * \brief Driver_SetName
 *
 * Set the path for the driver.  Name must be set prior to
 * access (since driver access is name-based).
 *
 * \param pstDriver_  Pointer to a driver to set the name of
 * \param pcName_     String constant containing the device path
 */
void Driver_SetName( Driver_t *pstDriver_, const char *pcName_ );

//---------------------------------------------------------------------------
/*!
 * \brief Driver_GetPath
 *
 * Returns a string containing the device path.
 *
 * \param pstDriver_ Pointer of the driver to read the name of
 * \return Return the string constant representing the device path
 */
const char *Driver_GetPath( Driver_t *pstDriver_ );

//---------------------------------------------------------------------------
/*!
 * \brief DriverList_Init
 *
 *  List of Driver objects used to keep track of all device drivers in the
 *  system.  By default, the list contains a single entity, "/dev/null".
 */
void DriverList_Init( void );

//---------------------------------------------------------------------------
/*!
 * \brief DriverList_FindByPath
 *
 * Find a driver by its path/name, and return a pointer to the object.
 * This returns the first match, in the even that multiple paths are found.
 *
 * \param pcPath   Name of the driver/path to the driver
 * \return Pointer to a driver found in the search, or NULL on no match found.
 */
Driver_t *DriverList_FindByPath( const char *pcPath );

//---------------------------------------------------------------------------
/*!
 * \brief DriverList_Add
 *
 * Add a driver to the global path space.  Registration of drivers with this
 * API is voluntary, but only drivers added to the path space can be searched
 * using DriverList_FindByPath().
 *
 * \param pstDriver_ Pointer to the driver to add to the global path space.
 */
void DriverList_Add( Driver_t *pstDriver_ );

#ifdef __cplusplus
    }
#endif

#endif //KERNEL_USE_DRIVER

#endif

