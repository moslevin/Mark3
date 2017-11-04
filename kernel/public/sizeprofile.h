#ifndef __SIZE_REPORT_H__
#define __SIZE_REPORT_H__

/*!
\page SIZEPROFILE  Code Size Profiling
The following report details the size of each module compiled into the
kernel. The size of each component is dependent on the flags specified
in mark3cfg.h at compile time.  Note that these sizes represent the
maximum size of each module before dead code elimination and any
additional link-time optimization, and represent the maximum possible
size that any module can take.

The results below are for profiling on Atmel AVR atmega328p-based targets using gcc. 
Results are not necessarily indicative of relative or absolute performance
on other platforms or toolchains.
\section SIZEPROFILEINFO Information

Date Profiled:
Wed Jun 28 23:33:26 EDT 2017

\section SIZEPROFILEVER  Compiler Version
avr-gcc.exe (AVR_8_bit_GNU_Toolchain_3.5.4_1709) 4.9.2 Copyright (C) 2014 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

\section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 0 Bytes
    -  Allocate-once Heap.............................. : 0 Bytes
    -  Synchronization Objects - Base Class............ : 126 Bytes
    -  Device Driver Framework (including /dev/null)... : 208 Bytes
    -  Synchronization Object - Event Flag............. : 746 Bytes
    -  Mark3 Kernel Base Class......................... : 161 Bytes
    -  Atmel AVR - Kernel Aware Simulation Support...... : 184 Bytes
    -  Semaphore (Synchronization Object).............. : 516 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 458 Bytes
    -  Mailbox IPC Support............................. : 858 Bytes
    -  Message-based IPC............................... : 386 Bytes
    -  Mutex (Synchronization Object).................. : 660 Bytes
    -  Notification Blocking Object.................... : 566 Bytes
    -  2D Priority Map Object - Scheduler.............. : 112 Bytes
    -  Performance-profiling timers.................... : 510 Bytes
    -  Round-Robin Scheduling Support.................. : 267 Bytes
    -  Thread Scheduling............................... : 324 Bytes
    -  Thread Implementation........................... : 1675 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 254 Bytes
    -  Software Timer Kernel Object.................... : 560 Bytes
    -  Software Timer Management....................... : 625 Bytes
    -  Runtime Kernel Trace Implementation............. : 0 Bytes
    -  Atmel AVR - Profiling Timer Implementation....... : 216 Bytes
    -  Atmel AVR - Kernel Interrupt Implemenation....... : 56 Bytes
    -  Atmel AVR - Kernel Timer Implementation.......... : 382 Bytes
    -  Atmel AVR - Basic Threading Support.............. : 558 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  3110 Bytes
    - Synchronization Objects  :  2308 Bytes
    - Port                     :  2820 Bytes
    - Features                 :  2170 Bytes
    - Total Size               :  10408 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
