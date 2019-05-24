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

The results below are for profiling on Atmel AVR atmega1284p-based targets using gcc. 
Results are not necessarily indicative of relative or absolute performance
on other platforms or toolchains.
@section SIZEPROFILEINFO Information

Date Profiled:
Thu May 23 20:49:34 EDT 2019

@section SIZEPROFILEVER  Compiler Version
avr-gcc (GCC) 5.4.0 Copyright (C) 2015 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

@section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 56 Bytes
    -  Allocate-once Heap.............................. : 120 Bytes
    -  Synchronization Objects - Base Class............ : 126 Bytes
    -  Condition Variables (Synchronization Object).... : 286 Bytes
    -  Coroutine task-list management.................. : 122 Bytes
    -  Main coroutine task object...................... : 336 Bytes
    -  Coroutine task scheduler........................ : 318 Bytes
    -  Synchronization Object - Event Flag............. : 872 Bytes
    -  Mark3 Kernel Base Class......................... : 142 Bytes
    -  Semaphore (Synchronization Object).............. : 568 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 560 Bytes
    -  RAII Locking Support based on Mark3 Mutex class. : 62 Bytes
    -  Mailbox IPC Support............................. : 1064 Bytes
    -  Message-based IPC............................... : 288 Bytes
    -  Mutex (Synchronization Object).................. : 658 Bytes
    -  Notification Blocking Object.................... : 626 Bytes
    -  Performance-profiling timers.................... : 366 Bytes
    -  Round-Robin Scheduling Support.................. : 265 Bytes
    -  Reader-writer Locks (Synchronization Object).... : 206 Bytes
    -  Thread Scheduling............................... : 570 Bytes
    -  Thread Implementation........................... : 1501 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 440 Bytes
    -  ThreadListList Data Structures.................. : 20 Bytes
    -  Software Timer Kernel Object.................... : 241 Bytes
    -  Software Timer Management....................... : 412 Bytes
    -  Atmel AVR - Kernel Interrupt Implemenation....... : 28 Bytes
    -  Atmel AVR - Kernel Timer Implementation.......... : 810 Bytes
    -  Atmel AVR - Basic Threading Support.............. : 506 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  3359 Bytes
    - Synchronization Objects  :  4630 Bytes
    - Port                     :  1344 Bytes
    - Features                 :  1460 Bytes
    - Coroutines               :  776 Bytes
    - Untracked Objects        :  0 Bytes
    - Total Size               :  11569 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
