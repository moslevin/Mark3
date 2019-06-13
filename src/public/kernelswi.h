/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   kernelswi.h

    @brief  Kernel Software interrupt declarations

 */
#pragma once
#include "kerneltypes.h"

//---------------------------------------------------------------------------
namespace Mark3
{
/**
 * @brief The KernelSWI Class provides the software-interrupt used to implement
 * the context-switching interrupt used by the kernel.  This interface must be
 * implemented by target-specific code in the porting layer.
 */
class KernelSWI
{
public:
    /**
     *  @brief Config
     *  Configure the software interrupt - must be called before any other
     *  software interrupt functions are called.
     */
    static void Config(void);

    /**
     *  @brief Start
     *  Enable ("Start") the software interrupt functionality
     */
    static void Start(void);

    /**
     *  @brief Trigger
     *  Call the software interrupt
     *
     */
    static void Trigger(void);
};
} // namespace Mark3
