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

    \file   kernelaware.cpp

    \brief  Kernel aware simulation support
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "kernelaware.h"
#include "threadport.h"

#if KERNEL_AWARE_SIMULATION

//---------------------------------------------------------------------------
/*!
    This structure is used to communicate between the kernel and a kernel-
    aware host.  Its data contents is interpreted differently depending on
    the command executed (by means of setting the g_u8KACommand variable, as
    is done in the command handlers in this module).  As a result, any changes
    to this struct by way of modifying or adding data must be mirrored in the
    kernel-aware simulator.
*/
typedef union
{
    volatile uint16_t au16Buffer[5];         //!< Raw binary contents of the struct
    /*!
     * \brief The Profiler struct contains data related to the code-execution
     *        profiling functionality provided by a kernel-aware host simluator.
     */
    struct
    {
        volatile const char *szName;      //!< Name of the profiling data to report
    } Profiler;
    /*!
     * \brief The Trace struct contains data related to the display and output
     *        of kernel-trace strings on a kernel-aware host.
     */
    struct
    {
        volatile uint16_t u16File;           //!< File index
        volatile uint16_t u16Line;           //!< Line number
        volatile uint16_t u16Code;           //!< Print code
        volatile uint16_t u16Arg1;           //!< (optional) argument code
        volatile uint16_t u16Arg2;           //!< (optional) argument code
    } Trace;
    /*!
     * \brief The Print struct contains data related to the display of arbitrary
     *        null-terminated ASCII strings on the kernel-aware host.
     */
    struct
    {
        volatile const char *szString;    //!< Pointer ot a string (in RAM) to print
    } Print;
} KernelAwareData_t;

//---------------------------------------------------------------------------
volatile bool           g_bIsKernelAware = false; //!< Will be set to true by a kernel-aware host.
volatile uint8_t        g_u8KACommand;            //!< Kernel-aware simulator command to execute
KernelAwareData_t       g_stKAData;               //!< Data structure used to communicate with host.


//---------------------------------------------------------------------------
void KernelAware::ProfileInit(const char *szStr_)
{
    CS_ENTER();
    g_stKAData.Profiler.szName = szStr_;
    g_u8KACommand = KA_COMMAND_PROFILE_INIT;
    CS_EXIT();
}

//---------------------------------------------------------------------------
void KernelAware::ProfileStart(void)
{
    g_u8KACommand = KA_COMMAND_PROFILE_START;
}

//---------------------------------------------------------------------------
void KernelAware::ProfileStop(void)
{
    g_u8KACommand = KA_COMMAND_PROFILE_STOP;
}

//---------------------------------------------------------------------------
void KernelAware::ProfileReport(void)
{
    g_u8KACommand = KA_COMMAND_PROFILE_REPORT;
}

//---------------------------------------------------------------------------
void KernelAware::ExitSimulator(void)
{
    g_u8KACommand = KA_COMMAND_EXIT_SIMULATOR;
}

//---------------------------------------------------------------------------
void KernelAware::Trace( uint16_t u16File_,
                          uint16_t u16Line_,
                          uint16_t u16Code_ )
{
    Trace_i( u16File_, u16Line_, u16Code_, 0, 0, KA_COMMAND_TRACE_0 );
}

//---------------------------------------------------------------------------
void KernelAware::Trace( uint16_t u16File_,
                          uint16_t u16Line_,
                          uint16_t u16Code_,
                          uint16_t u16Arg1_)
{
    Trace_i( u16File_, u16Line_, u16Code_, u16Arg1_, 0 ,KA_COMMAND_TRACE_1 );

}
//---------------------------------------------------------------------------
void KernelAware::Trace( uint16_t u16File_,
                          uint16_t u16Line_,
                          uint16_t u16Code_,
                          uint16_t u16Arg1_,
                          uint16_t u16Arg2_)
{
    Trace_i( u16File_, u16Line_, u16Code_, u16Arg1_, u16Arg2_, KA_COMMAND_TRACE_2 );
}

//---------------------------------------------------------------------------
void KernelAware::Trace_i( uint16_t u16File_,
                          uint16_t u16Line_,
                          uint16_t u16Code_,
                          uint16_t u16Arg1_,
                          uint16_t u16Arg2_,
                          KernelAwareCommand_t eCmd_ )
{
    CS_ENTER();
    g_stKAData.Trace.u16File = u16File_;
    g_stKAData.Trace.u16Line = u16Line_;
    g_stKAData.Trace.u16Code = u16Code_;
    g_stKAData.Trace.u16Arg1 = u16Arg1_;
    g_stKAData.Trace.u16Arg2 = u16Arg2_;
    g_u8KACommand = eCmd_;
    CS_EXIT();
}

//---------------------------------------------------------------------------
void KernelAware::Print(const char *szStr_)
{
    CS_ENTER();
    g_stKAData.Print.szString = szStr_;
    g_u8KACommand = KA_COMMAND_PRINT;
    CS_EXIT();
}

//---------------------------------------------------------------------------
bool KernelAware::IsSimulatorAware(void)
{
    return g_bIsKernelAware;
}

#endif
