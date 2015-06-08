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
    the command executed (by means of setting the g_ucKACommand variable, as
    is done in the command handlers in this module).  As a result, any changes
    to this struct by way of modifying or adding data must be mirrored in the
    kernel-aware simulator.
*/
typedef union
{
    volatile K_USHORT ausBuffer[5];         //!< Raw binary contents of the struct
    /*!
     * \brief The Profiler struct contains data related to the code-execution
     *        profiling functionality provided by a kernel-aware host simluator.
     */
    struct
    {
        volatile const K_CHAR *szName;      //!< Name of the profiling data to report
    } Profiler;
    /*!
     * \brief The Trace struct contains data related to the display and output
     *        of kernel-trace strings on a kernel-aware host.
     */
    struct
    {
        volatile K_USHORT usFile;           //!< File index
        volatile K_USHORT usLine;           //!< Line number
        volatile K_USHORT usCode;           //!< Print code
        volatile K_USHORT usArg1;           //!< (optional) argument code
        volatile K_USHORT usArg2;           //!< (optional) argument code
    } Trace;
    /*!
     * \brief The Print struct contains data related to the display of arbitrary
     *        null-terminated ASCII strings on the kernel-aware host.
     */
    struct
    {
        volatile const K_CHAR *szString;    //!< Pointer ot a string (in RAM) to print
    } Print;
} KernelAwareData_t;

//---------------------------------------------------------------------------
volatile K_BOOL           g_bIsKernelAware = false; //!< Will be set to true by a kernel-aware host.
volatile K_UCHAR        g_ucKACommand;            //!< Kernel-aware simulator command to execute
KernelAwareData_t       g_stKAData;               //!< Data structure used to communicate with host.


//---------------------------------------------------------------------------
void KernelAware::ProfileInit(const K_CHAR *szStr_)
{
    CS_ENTER();
    g_stKAData.Profiler.szName = szStr_;
    g_ucKACommand = KA_COMMAND_PROFILE_INIT;
    CS_EXIT();
}

//---------------------------------------------------------------------------
void KernelAware::ProfileStart(void)
{
    g_ucKACommand = KA_COMMAND_PROFILE_START;
}

//---------------------------------------------------------------------------
void KernelAware::ProfileStop(void)
{
    g_ucKACommand = KA_COMMAND_PROFILE_STOP;
}

//---------------------------------------------------------------------------
void KernelAware::ProfileReport(void)
{
    g_ucKACommand = KA_COMMAND_PROFILE_REPORT;
}

//---------------------------------------------------------------------------
void KernelAware::ExitSimulator(void)
{
    g_ucKACommand = KA_COMMAND_EXIT_SIMULATOR;
}

//---------------------------------------------------------------------------
void KernelAware::Trace( K_USHORT usFile_,
                          K_USHORT usLine_,
                          K_USHORT usCode_ )
{
    Trace_i( usFile_, usLine_, usCode_, 0, 0, KA_COMMAND_TRACE_0 );
}

//---------------------------------------------------------------------------
void KernelAware::Trace( K_USHORT usFile_,
                          K_USHORT usLine_,
                          K_USHORT usCode_,
                          K_USHORT usArg1_)
{
    Trace_i( usFile_, usLine_, usCode_, usArg1_, 0 ,KA_COMMAND_TRACE_1 );

}
//---------------------------------------------------------------------------
void KernelAware::Trace( K_USHORT usFile_,
                          K_USHORT usLine_,
                          K_USHORT usCode_,
                          K_USHORT usArg1_,
                          K_USHORT usArg2_)
{
    Trace_i( usFile_, usLine_, usCode_, usArg1_, usArg2_, KA_COMMAND_TRACE_2 );
}

//---------------------------------------------------------------------------
void KernelAware::Trace_i( K_USHORT usFile_,
                          K_USHORT usLine_,
                          K_USHORT usCode_,
                          K_USHORT usArg1_,
                          K_USHORT usArg2_,
                          KernelAwareCommand_t eCmd_ )
{
    CS_ENTER();
    g_stKAData.Trace.usFile = usFile_;
    g_stKAData.Trace.usLine = usLine_;
    g_stKAData.Trace.usCode = usCode_;
    g_stKAData.Trace.usArg1 = usArg1_;
    g_stKAData.Trace.usArg2 = usArg2_;
    g_ucKACommand = eCmd_;
    CS_EXIT();
}

//---------------------------------------------------------------------------
void KernelAware::Print(const K_CHAR *szStr_)
{
    CS_ENTER();
    g_stKAData.Print.szString = szStr_;
    g_ucKACommand = KA_COMMAND_PRINT;
    CS_EXIT();
}

//---------------------------------------------------------------------------
K_BOOL KernelAware::IsSimulatorAware(void)
{
    return g_bIsKernelAware;
}

#endif
