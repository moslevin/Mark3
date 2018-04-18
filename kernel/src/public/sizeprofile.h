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

The results below are for profiling on Atmel AVR atmega1284p-based targets using gcc. 
Results are not necessarily indicative of relative or absolute performance
on other platforms or toolchains.
\section SIZEPROFILEINFO Information

Date Profiled:
Tue Apr 17 23:26:11 DST 2018

\section SIZEPROFILEVER  Compiler Version
avr-gcc (GCC) 4.9.2 Copyright (C) 2014 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

\section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 302 Bytes
    -  Allocate-once Heap.............................. : 872 Bytes
    -  Synchronization Objects - Base Class............ : 126 Bytes
    -  Condition Variables (Synchronization Object).... : 284 Bytes
    -  Synchronization Object - Event Flag............. : 746 Bytes
    -  Mark3 Kernel Base Class......................... : 163 Bytes
    -  Atmel AVR - Kernel Aware Simulation Support...... : 184 Bytes
    -  Semaphore (Synchronization Object).............. : 516 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 458 Bytes
    -  Mailbox IPC Support............................. : 914 Bytes
    -  Message-based IPC............................... : 204 Bytes
    -  Mutex (Synchronization Object).................. : 660 Bytes
    -  Notification Blocking Object.................... : 566 Bytes
    -  2D Priority Map Object - Scheduler.............. : 112 Bytes
    -  Performance-profiling timers.................... : 516 Bytes
    -  Round-Robin Scheduling Support.................. : 269 Bytes
    -  Reader-writer Locks (Synchronization Object).... : 202 Bytes
    -  Thread Scheduling............................... : 324 Bytes
    -  Thread Implementation........................... : 1767 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 254 Bytes
    -  Software Timer Kernel Object.................... : 585 Bytes
    -  Software Timer Management....................... : 600 Bytes
    -  Runtime Kernel Trace Implementation............. : 0 Bytes
    -  Atmel AVR - Profiling Timer Implementation....... : 224 Bytes
    -  Atmel AVR - Kernel Interrupt Implemenation....... : 60 Bytes
    -  Atmel AVR - Kernel Timer Implementation.......... : 390 Bytes
    -  Atmel AVR - Basic Threading Support.............. : 590 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  3204 Bytes
    - Synchronization Objects  :  2612 Bytes
    - Port                     :  2928 Bytes
    - Features                 :  3144 Bytes
    - Total Size               :  11888 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
