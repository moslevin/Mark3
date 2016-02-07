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

    \file   kernelswi.h    

    \brief  Kernel Software interrupt declarations

 */

#include "kerneltypes.h"
#ifndef __KERNELSWI_H_
#define __KERNELSWI_H_

//---------------------------------------------------------------------------
/*!
 *  Class providing the software-interrupt required for context-switching in 
 *  the kernel.
 */
class KernelSWI
{
public:
    /*!
     *  \brief Config
     *
     *  Configure the software interrupt - must be called before any other 
     *  software interrupt functions are called.
     */
    static void Config(void);

    /*!
     *  \brief Start
     *
     *  Enable ("Start") the software interrupt functionality
     */
    static void Start(void);
    
    /*!
     *  \brief Stop
     *
     *  Disable the software interrupt functionality
     */
    static void Stop(void);
    
    /*!
     *  \brief Clear
     *
     *  Clear the software interrupt
     */
    static void Clear(void);
    
    /*!
     *  \brief Trigger
     *
     *  Call the software interrupt
     *  
     */
    static void Trigger(void);
    
    /*!
     *  \brief DI
     *
     *  Disable the SWI flag itself
     *  
     *  \return previous status of the SWI, prior to the DI call
     */
    static uint8_t DI();
    
    /*!
     *  \brief RI
     *
     *  Restore the state of the SWI to the value specified
     *  
     *  \param bEnable_ true - enable the SWI, false - disable SWI
     */        
    static void RI(bool bEnable_);    
};


#endif // __KERNELSIW_H_
