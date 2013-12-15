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

The results below are for profiling on AVR-based targets.  Results are
not necessarily indicative of relative or absolute performance on other
platforms or toolchains.
\section SIZEPROFILEINFO Information
Subversion Repository Information:
    - Repository Root: https://mark3.googlecode.com/svn
    - Revision: 139
    - URL: https://mark3.googlecode.com/svn/trunk/embedded
    .

Date Profiled:
Sun Dec 15 09:43:07 EST 2013

\section SIZEPROFILEVER  Compiler Version
avr-gcc (GCC) 4.7.2 Copyright (C) 2012 Free Software Foundation, Inc. This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

\section SIZEPROFILERES  Profiling Results
Mark3 Module Size Report:
    -  Atomic Operations............................... : 478 Bytes
    -  Synchronization Objects - Base Class............ : 266 Bytes
    -  Device Driver Framework (including /dev/null)... : 236 Bytes
    -  Synchronization Object - Event Flag............. : 1160 Bytes
    -  Fundamental Kernel Linked-List Classes.......... : 610 Bytes
    -  Message-based IPC............................... : 500 Bytes
    -  Mutex (Synchronization Object).................. : 1004 Bytes
    -  Performance-profiling timers.................... : 556 Bytes
    -  Round-Robin Scheduling Support.................. : 299 Bytes
    -  Thread Scheduling............................... : 499 Bytes
    -  Semaphore (Synchronization Object).............. : 868 Bytes
    -  Thread Implementation........................... : 1441 Bytes
    -  Fundamental Kernel Thread-list Data Structures.. : 212 Bytes
    -  Mark3 Kernel Base Class......................... : 80 Bytes
    -  Software Timer Implementation................... : 1035 Bytes
    -  Kernel Transaction Queues....................... : 308 Bytes
    -  Runtime Kernel Trace Implementation............. : 0 Bytes
    -  Circular Logging Buffer Base Class.............. : 524 Bytes
    -  AVR Port - Basic Threading Support.............. : 528 Bytes
    -  AVR Port - Kernel Interrupt Implemenation....... : 56 Bytes
    -  AVR Port - Kernel Timer Implementation.......... : 338 Bytes
    -  AVR Port - Profiling Timer Implementation....... : 256 Bytes
    .

Mark3 Kernel Size Summary:
    - Kernel                   :  3416 Bytes
    - Synchronization Objects  :  3532 Bytes
    - Port                     :  1178 Bytes
    - Features                 :  2650 Bytes
    - Total Size               :  10776 Bytes
    .
*/
#endif // __SIZE_REPORT_H__
