/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**

    @file   fake_types.h

    @brief  C-struct definitions that mirror

    This header contains a set of "fake" structures that have the same memory
    layout as the kernel objects in C++ (taking into account inheritence, etc.).
    These are used for sizing the opaque data blobs that are declared in C, which
    then become instantiated as C++ kernel objects via the bindings provided.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "mark3cfg.h"

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

//---------------------------------------------------------------------------
typedef struct {
    void* prev;
    void* next;
} Fake_LinkedListNode;

//---------------------------------------------------------------------------
typedef struct {
    void* head;
    void* tail;
} Fake_LinkedList;

//---------------------------------------------------------------------------
typedef struct {
    Fake_LinkedList fake_list;
    PORT_PRIO_TYPE  m_uXPriority;
    void*           m_pclMap;
} Fake_ThreadList;

//---------------------------------------------------------------------------
typedef struct {
    Fake_LinkedListNode m_ll_node;
    uint8_t m_u8Initialized;
    uint8_t  m_u8Flags;
    void*    m_pfCallback;
    uint32_t m_u32Interval;
    uint32_t m_u32TimeLeft;
    void*    m_pclOwner;
    void*    m_pvData;
} Fake_Timer;

//---------------------------------------------------------------------------
typedef struct {
    Fake_LinkedListNode m_ll_node;
    K_WORD*             m_pwStackTop;
    K_WORD*             m_pwStack;
    uint8_t             m_u8ThreadID;
    PORT_PRIO_TYPE      m_uXPriority;
    PORT_PRIO_TYPE      m_uXCurPriority;
    uint8_t             m_eState;
#if KERNEL_EXTENDED_CONTEXT
    void* m_pvExtendedContext;
#endif // #if KERNEL_EXTENDED_CONTEXT
#if KERNEL_NAMED_THREADS
    const char* m_szName;
#endif // #if KERNEL_NAMED_THREADS
    uint16_t m_u16StackSize;
    void*    m_pclCurrent;
    void*    m_pclOwner;
    void*    m_pfEntryPoint;
    void*    m_pvArg;
#if KERNEL_ROUND_ROBIN
    uint16_t m_u16Quantum;
#endif // #if KERNEL_ROUND_ROBIN
#if KERNEL_EVENT_FLAGS
    uint16_t m_u16FlagMask;
    uint8_t  m_eFlagMode;
#endif // #if KERNEL_EVENT_FLAGS
    Fake_Timer m_clTimer;
    bool m_bExpired;
} Fake_Thread;

//---------------------------------------------------------------------------
typedef struct {
    Fake_ThreadList thread_list;
    uint8_t m_u8Initialized;
    uint16_t m_u16Value;
    uint16_t m_u16MaxValue;
} Fake_Semaphore;

//---------------------------------------------------------------------------
typedef struct {
    Fake_ThreadList thread_list;
    uint8_t m_u8Initialized;
    uint8_t m_u8Recurse;
    bool    m_bReady;
    bool    m_bRecursive;
    uint8_t m_u8MaxPri;
    void*   m_pclOwner;
} Fake_Mutex;

//---------------------------------------------------------------------------
typedef struct {
    Fake_LinkedListNode list_node;
    void*               m_pvData;
    uint16_t            m_u16Code;
} Fake_Message;

//---------------------------------------------------------------------------
typedef struct {
    Fake_Semaphore  m_clSemaphore;
    Fake_LinkedList m_clLinkList;
} Fake_MessageQueue;

//---------------------------------------------------------------------------
typedef struct {
    Fake_LinkedList m_clList;
} Fake_MessagePool;

//---------------------------------------------------------------------------
typedef struct {
    uint16_t       m_u16Head;
    uint16_t       m_u16Tail;
    uint16_t       m_u16Count;
    uint16_t       m_u16Free;
    uint16_t       m_u16ElementSize;
    void*          m_pvBuffer;
    Fake_Semaphore m_clRecvSem;
    Fake_Semaphore m_clSendSem;
} Fake_Mailbox;

//---------------------------------------------------------------------------
typedef struct {
    Fake_ThreadList thread_list;
    uint8_t m_u8Initialized;
    bool m_bPending;
} Fake_Notify;

//---------------------------------------------------------------------------
typedef struct {
    Fake_ThreadList thread_list;
    uint8_t m_u8Initialized;
    uint16_t m_u16EventFlag;
} Fake_EventFlag;

//---------------------------------------------------------------------------
typedef struct {
    Fake_Mutex m_clGlobalMutex;
    Fake_Mutex m_clReaderMutex;
    uint8_t    m_u8ReadCount;
} Fake_ReaderWriterLock;

//---------------------------------------------------------------------------
typedef struct {
    Fake_Mutex     m_clMutex;
    Fake_Semaphore m_clSemaphore;
    uint8_t        m_u8Waiters;
} Fake_ConditionVariable;

#if defined(__cplusplus)
}
#endif
