#ifndef __SIZE_REPORT_H__
#define __SIZE_REPORT_H__

/**
@page SIZEPROFILE  Code Size Profiling
The following report details the size of each module compiled into the
kernel. The size of each component is dependent on the flags specified
in mark3cfg.h at compile time.  Note that these sizes represent the
maximum size of each module before dead code elimination and any
additional link-time optimization, and represent the maximum possible
size that any module can take.

The results below are for profiling on ARM Cortex-M3 qemu_lm3s6965evb-based targets using gcc. 
Results are not necessarily indicative of relative or absolute performance
on other platforms or toolchains.
@section SIZEPROFILEINFO Information

Date Profiled:
Mon Sep 24 22:30:34 EDT 2018

@section SIZEPROFILEVER  Compiler Version
arm-none-eabi-gcc (15:6.3.1+svn253039-1build1) 6.3.1 20170620 Copyright (C) 2016 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

@section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 336 Bytes
    -  Allocate-once Heap.............................. : 724 Bytes
    -  Synchronization Objects - Base Class............ : 82 Bytes
    -  Condition Variables (Synchronization Object).... : 216 Bytes
    -  Synchronization Object - Event Flag............. : 494 Bytes
    -  Mark3 Kernel Base Class......................... : 112 Bytes
    -  Semaphore (Synchronization Object).............. : 366 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 268 Bytes
    -  RAII Locking Support based on Mark3 Mutex class. : 58 Bytes
    -  Mailbox IPC Support............................. : 588 Bytes
    -  Message-based IPC............................... : 194 Bytes
    -  Mutex (Synchronization Object).................. : 464 Bytes
    -  Notification Blocking Object.................... : 396 Bytes
    -  2D Priority Map Object - Scheduler.............. : 58 Bytes
    -  Performance-profiling timers.................... : 240 Bytes
    -  Round-Robin Scheduling Support.................. : 207 Bytes
    -  Reader-writer Locks (Synchronization Object).... : 158 Bytes
    -  Thread Scheduling............................... : 798 Bytes
    -  Thread Implementation........................... : 1157 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 150 Bytes
    -  Software Timer Kernel Object.................... : 298 Bytes
    -  Software Timer Management....................... : 228 Bytes
    -  ARM Cortex-M3 - Profiling Timer Implementation....... : 119 Bytes
    -  ARM Cortex-M3 - Kernel Interrupt Implemenation....... : 66 Bytes
    -  ARM Cortex-M3 - Kernel Timer Implementation.......... : 2518 Bytes
    -  ARM Cortex-M3 - Basic Threading Support.............. : 340 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  2625 Bytes
    - Synchronization Objects  :  1950 Bytes
    - Port                     :  4027 Bytes
    - Features                 :  2033 Bytes
    - Total Size               :  10635 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
