/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   kernelswi.h

    @brief  Kernel Software interrupt declarations

 */
#pragma once
#include "kerneltypes.h"

//---------------------------------------------------------------------------
/**
 *  Class providing the software-interrupt required for context-switching in
 *  the kernel.
 */
namespace Mark3
{
class KernelSWI
{
public:
    /**
     *  @brief Config
     *
     *  Configure the software interrupt - must be called before any other
     *  software interrupt functions are called.
     */
    static void Config(void);

    /**
     *  @brief Start
     *
     *  Enable ("Start") the software interrupt functionality
     */
    static void Start(void);

    /**
     *  @brief Trigger
     *
     *  Call the software interrupt
     *
     */
    static void Trigger(void);
};
} // namespace Mark3
