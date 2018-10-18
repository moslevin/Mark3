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

The results below are for profiling on none qemu_raspi3-based targets using gcc. 
Results are not necessarily indicative of relative or absolute performance
on other platforms or toolchains.
@section SIZEPROFILEINFO Information

Date Profiled:
Wed Oct 17 20:31:22 EDT 2018

@section SIZEPROFILEVER  Compiler Version
gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0 Copyright (C) 2017 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

@section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
@code
    -  Atomic Operations............................... : 1496 Bytes
    -  Allocate-once Heap.............................. : 1336 Bytes
    -  Synchronization Objects - Base Class............ : 188 Bytes
    -  Condition Variables (Synchronization Object).... : 376 Bytes
    -  Synchronization Object - Event Flag............. : 1244 Bytes
    -  Mark3 Kernel Base Class......................... : 164 Bytes
    -  Semaphore (Synchronization Object).............. : 940 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 492 Bytes
    -  RAII Locking Support based on Mark3 Mutex class. : 88 Bytes
    -  Mailbox IPC Support............................. : 1172 Bytes
    -  Message-based IPC............................... : 652 Bytes
    -  Mutex (Synchronization Object).................. : 796 Bytes
    -  Notification Blocking Object.................... : 968 Bytes
    -  2D Priority Map Object - Scheduler.............. : 284 Bytes
    -  Performance-profiling timers.................... : 676 Bytes
    -  Round-Robin Scheduling Support.................. : 507 Bytes
    -  Reader-writer Locks (Synchronization Object).... : 264 Bytes
    -  Thread Scheduling............................... : 8762 Bytes
    -  Thread Implementation........................... : 2817 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 304 Bytes
    -  Software Timer Kernel Object.................... : 416 Bytes
    -  Software Timer Management....................... : 432 Bytes
    -  none - Profiling Timer Implementation....... : 28 Bytes
    -  none - Kernel Interrupt Implemenation....... : 92 Bytes
    -  none - Kernel Timer Implementation.......... : 8884 Bytes
    -  none - Basic Threading Support.............. : 664 Bytes
    -  port_asm.s.obj : 1272 Bytes
@endcode

Mark3 Kernel Size Summary:
@code
    - Kernel                   :  13011 Bytes
    - Synchronization Objects  :  4360 Bytes
    - Port                     :  11808 Bytes
    - Features                 :  4863 Bytes
    - Total Size               :  34042 Bytes
@endcode
*/
#endif // __SIZE_REPORT_H__
