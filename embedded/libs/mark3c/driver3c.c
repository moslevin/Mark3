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

    \file   driver.cpp

    \brief  Device driver/hardware abstraction layer
*/

#include "mark3cfg.h"
#include "driver3c.h"

//---------------------------------------------------------------------------
#if KERNEL_USE_DRIVER

static Driver_t *pstDriverList = 0;

/*!
	This object implements the "default" driver (/dev/null)
*/

//---------------------------------------------------------------------------
static DriverVTable_t stDevNullVT =
{
    0,
    0,
    0,
    0,
    0
};

//---------------------------------------------------------------------------
static Driver_t stDevNull = { .pstVTable = &stDevNullVT, .szName = "/dev/null" };

//---------------------------------------------------------------------------
/*!
 * \brief DrvCmp
 *
 * String comparison function used to compare input driver name against a
 * known driver name in the existing driver list.
 *
 * \param szStr1_   user-specified driver name
 * \param szStr2_   name of a driver, provided from the driver table
 * \return  1 on match, 0 on no-match
 */
static uint8_t DrvCmp( const char *szStr1_, const char *szStr2_ )
{
	char *szTmp1 = (char*) szStr1_;
	char *szTmp2 = (char*) szStr2_;

	while (*szTmp1 && *szTmp2)
	{
		if (*szTmp1++ != *szTmp2++)
		{
			return 0;
		}
	}

	// Both terminate at the same length
	if (!(*szTmp1) && !(*szTmp2))
	{
		return 1;
	}

	return 0;
}

//---------------------------------------------------------------------------
void DriverList_Init( void )
{
    // Ensure we always have at least one entry - a default in case no match
    // is found (/dev/null)
    pstDriverList = 0;
    DriverList_Add(&stDevNull);
}

//---------------------------------------------------------------------------
void DriverList_Add( Driver_t *pstDriver_ )
{
    pstDriver_->next = pstDriverList;
    pstDriverList = pstDriver_;
}

//---------------------------------------------------------------------------
Driver_t *DriverList_FindByPath( const char *pcPath )
{
    Driver_t *pstTemp;

    pstTemp = pstDriverList;
	
    // Iterate through the list of drivers until we find a match, or we
    // exhaust our list of installed drivers
	while (pstTemp)
	{
        if( DrvCmp( pcPath, Driver_GetPath( pstTemp ) ) )
		{
			return pstTemp;
		}
        pstTemp = pstTemp->next;
	}
    // No matching driver found - return a pointer to our /dev/null driver
    return &stDevNull;
}

//---------------------------------------------------------------------------
void Driver_SetName( Driver_t *pstDriver_, const char *pcName_ )
{
    pstDriver_->szName = pcName_;
}

//---------------------------------------------------------------------------
const char *Driver_GetPath( Driver_t *pstDriver_ )
{
    return pstDriver_->szName;
}

//---------------------------------------------------------------------------
uint8_t Driver_Open( Driver_t *pstDriver_ )
{
    if ( pstDriver_->pstVTable->pfOpen )
    {
        return pstDriver_->pstVTable->pfOpen( pstDriver_ );
    }
    return 0;
}
//---------------------------------------------------------------------------
uint8_t Driver_Close( Driver_t *pstDriver_ )
{
    if ( pstDriver_->pstVTable->pfClose )
    {
        return pstDriver_->pstVTable->pfClose( pstDriver_ );
    }
    return 0;
}

//---------------------------------------------------------------------------
uint16_t Driver_Read( Driver_t *pstDriver_, uint16_t usSize_, uint8_t *pucData_ )
{
    if ( pstDriver_->pstVTable->pfRead )
    {
        return pstDriver_->pstVTable->pfRead( pstDriver_, usSize_, pucData_ );
    }
    return usSize_;
}
//---------------------------------------------------------------------------
uint16_t Driver_Write( Driver_t *pstDriver_, uint16_t usSize_, uint8_t *pucData_ )
{
    if ( pstDriver_->pstVTable->pfWrite )
    {
        return pstDriver_->pstVTable->pfWrite( pstDriver_, usSize_, pucData_ );
    }
    return usSize_;
}
//---------------------------------------------------------------------------
uint16_t Driver_Control( Driver_t *pstDriver_, uint16_t usEvent_, uint16_t usInSize_, uint8_t *pucIn_, uint16_t usOutSize_, uint8_t *pucOut_)
{
    if ( pstDriver_->pstVTable->pfControl )
    {
        return pstDriver_->pstVTable->pfControl( pstDriver_, usEvent_, usInSize_, pucIn_, usOutSize_, pucOut_);
    }
    return 0;
}

#endif
