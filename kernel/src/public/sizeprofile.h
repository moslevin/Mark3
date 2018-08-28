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

The results below are for profiling on ARM Cortex-M4F stm32f429-based targets using gcc. 
Results are not necessarily indicative of relative or absolute performance
on other platforms or toolchains.
@section SIZEPROFILEINFO Information

Date Profiled:
Mon Aug 27 23:26:39 EDT 2018

@section SIZEPROFILEVER  Compiler Version
arm-none-eabi-gcc (15:6.3.1+svn253039-1build1) 6.3.1 20170620 Copyright (C) 2016 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

@section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 336 Bytes
    -  Allocate-once Heap.............................. : 724 Bytes
    -  Synchronization Objects - Base Class............ : 82 Bytes
    -  Condition Variables (Synchronization Object).... : 216 Bytes
    -  Synchronization Object - Event Flag............. : 500 Bytes
    -  Mark3 Kernel Base Class......................... : 112 Bytes
    -  Semaphore (Synchronization Object).............. : 366 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 268 Bytes
    -  RAII Locking Support based on Mark3 Mutex class. : 58 Bytes
    -  Mailbox IPC Support............................. : 576 Bytes
    -  Message-based IPC............................... : 194 Bytes
    -  Mutex (Synchronization Object).................. : 448 Bytes
    -  Notification Blocking Object.................... : 404 Bytes
    -  2D Priority Map Object - Scheduler.............. : 138 Bytes
    -  Performance-profiling timers.................... : 252 Bytes
    -  Round-Robin Scheduling Support.................. : 337 Bytes
    -  Reader-writer Locks (Synchronization Object).... : 160 Bytes
    -  Thread Scheduling............................... : 4398 Bytes
    -  Thread Implementation........................... : 1297 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 152 Bytes
    -  Software Timer Kernel Object.................... : 310 Bytes
    -  Software Timer Management....................... : 252 Bytes
    -  ARM Cortex-M4F - Profiling Timer Implementation....... : 16 Bytes
    -  ARM Cortex-M4F - Kernel Interrupt Implemenation....... : 66 Bytes
    -  ARM Cortex-M4F - Kernel Timer Implementation.......... : 2530 Bytes
    -  ARM Cortex-M4F - Basic Threading Support.............. : 388 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  6447 Bytes
    - Synchronization Objects  :  1942 Bytes
    - Port                     :  3980 Bytes
    - Features                 :  2211 Bytes
    - Total Size               :  14580 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
