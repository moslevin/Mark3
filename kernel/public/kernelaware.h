/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   kernelaware.h

    \brief  Kernel aware simulation support
*/
#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

#if KERNEL_AWARE_SIMULATION
namespace Mark3
{
//---------------------------------------------------------------------------
/*!
 * \brief The KernelAware class
 *
 * This class contains functions that are used to trigger kernel-aware
 * functionality within a supported simulation environment (i.e. flAVR).
 *
 * These static methods operate on a singleton set of global variables,
 * which are monitored for changes from within the simulator.  The simulator
 * hooks into these variables by looking for the correctly-named symbols in
 * an elf-formatted binary being run and registering callbacks that are called
 * whenever the variables are changed.  On each change of the command variable,
 * the kernel-aware data is analyzed and interpreted appropriately.
 *
 * If these methods are run in an unsupported simulator or on actual hardware
 * the commands generally have no effect (except for the exit-on-reset command,
 * which will result in a jump-to-0 reset).
 */
namespace KernelAware
{
    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileInit
     *
     * Initializes the kernel-aware profiler.  This function instructs the
     * kernel-aware simulator to reset its accounting variables, and prepare to
     * start counting profiling data tagged to the given string.  How this is
     * handled is the responsibility of the simulator.
     *
     * \param szStr_ String to use as a tag for the profilng session.
     */
    void ProfileInit(const char* szStr_);

    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileStart
     *
     * Instruct the kernel-aware simulator to begin counting cycles towards the
     * current profiling counter.
     *
     */
    void ProfileStart(void);

    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileStop
     *
     * Instruct the kernel-aware simulator to end counting cycles relative to the
     * current profiling counter's iteration.
     */
    void ProfileStop(void);

    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileReport
     *
     * Instruct the kernel-aware simulator to print a report for its current
     * profiling data.
     *
     */
    void ProfileReport(void);

    //---------------------------------------------------------------------------
    /*!
     * \brief ExitSimulator
     *
     * Instruct the kernel-aware simulator to terminate (destroying the virtual
     * CPU).
     *
     */
    void ExitSimulator(void);

    //---------------------------------------------------------------------------
    /*!
     * \brief Print
     *
     * Instruct the kernel-aware simulator to print a char string
     *
     * \param szStr_
     */
    void Print(const char* szStr_);

    //---------------------------------------------------------------------------
    /*!
     * \brief Trace
     *
     * Insert a kernel trace statement into the kernel-aware simulator's debug
     * data stream.
     *
     * \param u16File_   16-bit code representing the file
     * \param u16Line_   16-bit code representing the line in the file
     */
    void Trace(uint16_t u16File_, uint16_t u16Line_);

    //---------------------------------------------------------------------------
    /*!
     * \brief Trace
     *
     * Insert a kernel trace statement into the kernel-aware simulator's debug
     * data stream.
     *
     * \param u16File_   16-bit code representing the file
     * \param u16Line_   16-bit code representing the line in the file
     * \param u16Arg1_   16-bit argument to the format string.
     */
    void Trace(uint16_t u16File_, uint16_t u16Line_, uint16_t u16Arg1_);

    //---------------------------------------------------------------------------
    /*!
     * \brief Trace
     *
     * Insert a kernel trace statement into the kernel-aware simulator's debug
     * data stream.
     *
     * \param u16File_   16-bit code representing the file
     * \param u16Line_   16-bit code representing the line in the file
     * \param u16Arg1_   16-bit argument to the format string.
     * \param u16Arg2_   16-bit argument to the format string.
     */
    void Trace(uint16_t u16File_, uint16_t u16Line_, uint16_t u16Arg1_, uint16_t u16Arg2_);

    //---------------------------------------------------------------------------
    /*!
     * \brief IsSimulatorAware
     *
     * use this function to determine whether or not the code is running on a
     * simulator that is aware of the kernel.
     *
     * \return true - the application is being run in a kernel-aware simulator.
     *         false - otherwise.
     */
    bool IsSimulatorAware(void);
} // namespace KernelAware
} //namespace Mark3
#endif
