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

    \file   kernelaware.h

    \brief  Kernel aware simulation support
*/

#ifndef __KERNEL_AWARE_H__
#define __KERNEL_AWARE_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#if KERNEL_AWARE_SIMULATION
//---------------------------------------------------------------------------
/*!
    This enumeration contains a list of supported commands that can be
    executed to invoke a response from a kernel aware host.
*/
typedef enum
{
    KA_COMMAND_IDLE = 0,        //!< Null command, does nothing.
    KA_COMMAND_PROFILE_INIT,    //!< Initialize a new profiling session
    KA_COMMAND_PROFILE_START,   //!< Begin a profiling sample
    KA_COMMAND_PROFILE_STOP,    //!< End a profiling sample
    KA_COMMAND_PROFILE_REPORT,  //!< Report current profiling session
    KA_COMMAND_EXIT_SIMULATOR,  //!< Terminate the host simulator
    KA_COMMAND_TRACE_0,         //!< 0-argument kernel trace
    KA_COMMAND_TRACE_1,         //!< 1-argument kernel trace
    KA_COMMAND_TRACE_2,         //!< 2-argument kernel trace
    KA_COMMAND_PRINT            //!< Print an arbitrary string of data
} KernelAwareCommand_t;

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
class KernelAware
{
public:
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
    static void ProfileInit( const char *szStr_ );

    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileStart
     *
     * Instruct the kernel-aware simulator to begin counting cycles towards the
     * current profiling counter.
     *
     */
    static void ProfileStart( void );

    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileStop
     *
     * Instruct the kernel-aware simulator to end counting cycles relative to the
     * current profiling counter's iteration.
     */
    static void ProfileStop( void );

    //---------------------------------------------------------------------------
    /*!
     * \brief ProfileReport
     *
     * Instruct the kernel-aware simulator to print a report for its current
     * profiling data.
     *
     */
    static void ProfileReport( void );

    //---------------------------------------------------------------------------
    /*!
     * \brief ExitSimulator
     *
     * Instruct the kernel-aware simulator to terminate (destroying the virtual
     * CPU).
     *
     */
    static void ExitSimulator( void );

    //---------------------------------------------------------------------------
    /*!
     * \brief Print
     *
     * Instruct the kernel-aware simulator to print a char string
     *
     * \param szStr_
     */
    static void Print( const char *szStr_ );

    //---------------------------------------------------------------------------
    /*!
     * \brief Trace
     *
     * Insert a kernel trace statement into the kernel-aware simulator's debug
     * data stream.
     *
     * \param u16File_   16-bit code representing the file
     * \param u16Line_   16-bit code representing the line in the file
     * \param u16Code_   16-bit data code, which indicates the line's format.
     */
    static void Trace( uint16_t u16File_,
                  uint16_t u16Line_,
                  uint16_t u16Code_ );

    //---------------------------------------------------------------------------
    /*!
     * \brief Trace
     *
     * Insert a kernel trace statement into the kernel-aware simulator's debug
     * data stream.
     *
     * \param u16File_   16-bit code representing the file
     * \param u16Line_   16-bit code representing the line in the file
     * \param u16Code_   16-bit data code, which indicates the line's format
     * \param u16Arg1_   16-bit argument to the format string.
     */
    static void Trace( uint16_t u16File_,
                  uint16_t u16Line_,
                  uint16_t u16Code_,
                  uint16_t u16Arg1_);

    //---------------------------------------------------------------------------
    /*!
     * \brief Trace
     *
     * Insert a kernel trace statement into the kernel-aware simulator's debug
     * data stream.
     *
     * \param u16File_   16-bit code representing the file
     * \param u16Line_   16-bit code representing the line in the file
     * \param u16Code_   16-bit data code, which indicates the line's format
     * \param u16Arg1_   16-bit argument to the format string.
     * \param u16Arg2_   16-bit argument to the format string.
     */
    static void Trace( uint16_t u16File_,
                  uint16_t u16Line_,
                  uint16_t u16Code_,
                  uint16_t u16Arg1_,
                  uint16_t u16Arg2_);

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
    static bool IsSimulatorAware(void);

private:

   //---------------------------------------------------------------------------
   /*!
    * \brief Trace_i
    *
    * Private function by which the class's Trace() methods are reflected, which
    * allows u16 to realize a modest code saving.
    *
    * \param u16File_   16-bit code representing the file
    * \param u16Line_   16-bit code representing the line in the file
    * \param u16Code_   16-bit data code, which indicates the line's format
    * \param u16Arg1_   16-bit argument to the format string.
    * \param u16Arg2_   16-bit argument to the format string.
    * \param eCmd_     Code indicating the number of arguments to emit.
    */
    static void Trace_i( uint16_t u16File_,
                         uint16_t u16Line_,
                         uint16_t u16Code_,
                         uint16_t u16Arg1_,
                         uint16_t u16Arg2_,
                         KernelAwareCommand_t eCmd_);
};

#endif

#endif
