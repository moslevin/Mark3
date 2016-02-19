/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   fake_types.h

    \brief  C-struct definitions that mirror

    This header contains a set of "fake" structures that have the same memory
    layout as the kernel objects in C++ (taking into account inheritence, etc.).
    These are used for sizing the opaque data blobs that are declared in C, which
    then become instantiated as C++ kernel objects via the bindings provided.
*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#ifndef __FAKE_TYPES_H__
#define __FAKE_TYPES_H__

#if defined(__cplusplus)
extern "C" {
#endif

//---------------------------------------------------------------------------
typedef struct
{
    void *prev;
    void *next;
} Fake_LinkedListNode;

//---------------------------------------------------------------------------
typedef struct
{
    void    *vtab_ptr;
    void    *head;
    void    *tail;
} Fake_LinkedList;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_LinkedList fake_list;
    PRIO_TYPE m_uXPriority;
    void     *m_pclMap;
} Fake_ThreadList;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_LinkedListNode m_ll_node;
    uint8_t m_u8Flags;
    void*   m_pfCallback;
    uint32_t m_u32Interval;
    uint32_t m_u32TimeLeft;
    uint32_t m_u32TimerTolerance;
    void  *m_pclOwner;
    void  *m_pvData;
} Fake_Timer;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_LinkedListNode m_ll_node;
    K_WORD *m_pwStackTop;
    K_WORD *m_pwStack;
    uint8_t m_u8ThreadID;
    PRIO_TYPE m_uXPriority;
    PRIO_TYPE m_uXCurPriority;
    uint8_t m_eState;
#if KERNEL_USE_THREADNAME
    const char *m_szName;
#endif
    uint16_t m_u16StackSize;
    void *m_pclCurrent;
    void *m_pclOwner;
    void *m_pfEntryPoint;
    void *m_pvArg;
#if KERNEL_USE_QUANTUM
    uint16_t m_u16Quantum;
#endif
#if KERNEL_USE_EVENTFLAG
    uint16_t m_u16FlagMask;
    uint8_t  m_eFlagMode;
#endif
#if KERNEL_USE_TIMEOUTS || KERNEL_USE_SLEEP
    Fake_Timer   m_clTimer;
#endif
#if KERNEL_USE_TIMEOUTS
    bool    m_bExpired;
#endif
} Fake_Thread;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_ThreadList thread_list;
    uint16_t m_u16Value;
    uint16_t m_u16MaxValue;
} Fake_Semaphore;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_ThreadList thread_list;
    uint8_t m_u8Recurse;
    bool m_bReady;
    uint8_t m_u8MaxPri;
    void *m_pclOwner;
} Fake_Mutex;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_LinkedListNode list_node;
    void *m_pvData;
    uint16_t m_u16Code;
} Fake_Message;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_Semaphore  m_clSemaphore;
    Fake_LinkedList m_clLinkList;
} Fake_MessageQueue;

//---------------------------------------------------------------------------
typedef struct
{
    uint16_t m_u16Head;
    uint16_t m_u16Tail;
    uint16_t m_u16Count;
    uint16_t m_u16Free;
    uint16_t m_u16ElementSize;
    void *m_pvBuffer;
    Fake_Semaphore m_clRecvSem;
#if KERNEL_USE_TIMEOUTS
    Fake_Semaphore m_clSendSem;
#endif
} Fake_Mailbox;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_ThreadList thread_list;
} Fake_Notify;

//---------------------------------------------------------------------------
typedef struct
{
    Fake_ThreadList thread_list;
    uint16_t        m_u16EventFlag;
} Fake_EventFlag;

#if defined(__cplusplus)
}
#endif

#endif // __FAKE_TYPES_H__

