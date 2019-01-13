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
Sat Jan 12 20:25:41 EST 2019

@section SIZEPROFILEVER  Compiler Version
avr-gcc (GCC) 5.4.0 Copyright (C) 2015 Free Software Foundation, Inc. This is free software; see the source for copying
conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

@section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 302 Bytes
    -  Allocate-once Heap.............................. : 872 Bytes
    -  Synchronization Objects - Base Class............ : 126 Bytes
    -  Condition Variables (Synchronization Object).... : 286 Bytes
    -  Synchronization Object - Event Flag............. : 794 Bytes
    -  Mark3 Kernel Base Class......................... : 92 Bytes
    -  Semaphore (Synchronization Object).............. : 504 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 560 Bytes
    -  RAII Locking Support based on Mark3 Mutex class. : 62 Bytes
    -  Mailbox IPC Support............................. : 934 Bytes
    -  Message-based IPC............................... : 202 Bytes
    -  Mutex (Synchronization Object).................. : 654 Bytes
    -  Notification Blocking Object.................... : 566 Bytes
    -  2D Priority Map Object - Scheduler.............. : 260 Bytes
    -  Performance-profiling timers.................... : 506 Bytes
    -  Round-Robin Scheduling Support.................. : 219 Bytes
    -  Reader-writer Locks (Synchronization Object).... : 206 Bytes
    -  Thread Scheduling............................... : 390 Bytes
    -  Thread Implementation........................... : 1337 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 254 Bytes
    -  Software Timer Kernel Object.................... : 235 Bytes
    -  Software Timer Management....................... : 412 Bytes
    -  Atmel AVR - Profiling Timer Implementation....... : 224 Bytes
    -  Atmel AVR - Kernel Interrupt Implemenation....... : 60 Bytes
    -  Atmel AVR - Kernel Timer Implementation.......... : 894 Bytes
    -  Atmel AVR - Basic Threading Support.............. : 504 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  3019 Bytes
    - Synchronization Objects  :  2708 Bytes
    - Port                     :  3182 Bytes
    - Features                 :  2546 Bytes
    - Total Size               :  11455 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
