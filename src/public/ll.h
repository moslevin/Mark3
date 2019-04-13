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
=========================================================================== */
/**

    @file   ll.h

    @brief  Core linked-list declarations, used by all kernel list types

    At the heart of RTOS data structures are linked lists.  Having a robust
    and efficient set of linked-list types that we can use as a foundation for
    building the rest of our kernel types allows u16 to keep our RTOS code
    efficient and logically-separated.

    So what data types rely on these linked-list classes?

    -Threads
    -ThreadLists
    -The Scheduler
    -Timers,
    -The Timer Scheduler
    -Blocking objects (Semaphores, Mutexes, etc...)

    Pretty much everything in the kernel uses these linked lists.  By
    having objects inherit from the base linked-list node type, we're able
    to leverage the double and circular linked-list classes to manager
    virtually every object type in the system without duplicating code.
    These functions are very efficient as well, allowing for very deterministic
    behavior in our code.

 */
#pragma once
#include "kerneltypes.h"

namespace Mark3
{
//---------------------------------------------------------------------------
/**
    Forward declarations of linked-list classes that are used further on in
    this module.  This allows u16 to also specify which friend classes can
    access the LinkListNode type.
 */
class LinkList;
class DoubleLinkList;
class CircularLinkList;

//---------------------------------------------------------------------------
/**
 *  Basic linked-list node data structure.  This data is managed by the
 *  linked-list class types, and can be used transparently between them.
 */
class LinkListNode
{
protected:
    LinkListNode* next; //!< Pointer to the next node in the list
    LinkListNode* prev; //!< Pointer to the previous node in the list

    LinkListNode() {}
    /**
     *  @brief ClearNode
     *
     *  Initialize the linked list node, clearing its next and previous node.
     */
    void ClearNode();

public:
    /**
     *  @brief GetNext
     *
     *  Returns a pointer to the next node in the list.
     *
     *  @return a pointer to the next node in the list.
     */
    inline LinkListNode* GetNext(void) { return next; }
    /**
     *  @brief GetPrev
     *
     *  Returns a pointer to the previous node in the list.
     *
     *  @return a pointer to the previous node in the list.
     */
    inline LinkListNode* GetPrev(void) { return prev; }
    friend class LinkList;
    friend class DoubleLinkList;
    friend class CircularLinkList;
};

//---------------------------------------------------------------------------
template <typename T>
class TypedLinkListNode : public LinkListNode
{
public:
    inline T* GetNext() { return reinterpret_cast<T*>(LinkListNode::GetNext()); }
    inline T* GetPrev() { return reinterpret_cast<T*>(LinkListNode::GetPrev()); }
};
//---------------------------------------------------------------------------
/**
 *  Abstract-data-type from which all other linked-lists are derived
 */
class LinkList
{
protected:
    LinkListNode* m_pclHead; //!< Pointer to the head node in the list
    LinkListNode* m_pclTail; //!< Pointer to the tail node in the list

public:
    /**
     *  @brief Init
     *
     *  Clear the linked list.
     */
    void Init()
    {
        m_pclHead = nullptr;
        m_pclTail = nullptr;
    }

    /**
     *  @brief GetHead
     *
     *  Get the head node in the linked list
     *
     *  @return Pointer to the head node in the list
     */
    inline LinkListNode* GetHead() { return m_pclHead; }
    inline void SetHead(LinkListNode* pclNode_) { m_pclHead = pclNode_; }

    /**
     *  @brief GetTail
     *
     *  Get the tail node of the linked list
     *
     *  @return Pointer to the tail node in the list
     */
    inline LinkListNode* GetTail() { return m_pclTail; }
    inline void SetTail(LinkListNode* pclNode_) { m_pclTail = pclNode_; }
};

//---------------------------------------------------------------------------
/**
 *  Doubly-linked-list data type, inherited from the base LinkList type.
 */
class DoubleLinkList : public LinkList
{
public:
    void* operator new(size_t sz, void* pv) { return reinterpret_cast<DoubleLinkList*>(pv); };
    /**
     *  @brief DoubleLinkList
     *
     *  Default constructor - initializes the head/tail nodes to nullptr
     */
    DoubleLinkList()
    {
        m_pclHead = nullptr;
        m_pclTail = nullptr;
    }

    /**
     *  @brief Add
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to add
     */
    void Add(LinkListNode* node_);

    /**
     *  @brief Remove
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to remove
     */
    void Remove(LinkListNode* node_);
};

//---------------------------------------------------------------------------
/**
    Circular-linked-list data type, inherited from the base LinkList type.
 */
class CircularLinkList : public LinkList
{
public:
    void* operator new(size_t sz, void* pv) { return (CircularLinkList*)pv; };
    CircularLinkList()
    {
        m_pclHead = nullptr;
        m_pclTail = nullptr;
    }

    /**
     *  @brief
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to add
     */
    void Add(LinkListNode* node_);

    /**
     *  @brief Remove
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to remove
     */
    void Remove(LinkListNode* node_);

    /**
     *  @brief PivotForward
     *
     *  Pivot the head of the circularly linked list forward
     *  ( Head = Head->next, Tail = Tail->next )
     */
    void PivotForward();

    /**
     *  @brief PivotBackward
     *
     *  Pivot the head of the circularly linked list backward
     *  ( Head = Head->prev, Tail = Tail->prev )
     */
    void PivotBackward();

    /**
     * @brief InsertNodeBefore
     *
     * Insert a linked-list node into the list before the specified insertion
     * point.
     *
     * @param node_     Node to insert into the list
     * @param insert_   Insert point.
     */
    void InsertNodeBefore(LinkListNode* node_, LinkListNode* insert_);
};

template <typename T>
class TypedDoubleLinkList : public DoubleLinkList
{
public:
    void* operator new(size_t sz, void* pv) { return reinterpret_cast<TypedDoubleLinkList<T>*>(pv); }

    TypedDoubleLinkList<T>() {
        DoubleLinkList();
    }

    inline T* GetHead() { return reinterpret_cast<T*>(DoubleLinkList::GetHead()); }
    inline void SetHead(T* pclNode_) { DoubleLinkList::SetHead(reinterpret_cast<LinkListNode*>(pclNode_)); }

    /**
     *  @brief GetTail
     *
     *  Get the tail node of the linked list
     *
     *  @return Pointer to the tail node in the list
     */
    inline T* GetTail() { return reinterpret_cast<T*>(DoubleLinkList::GetTail()); }
    inline void SetTail(T* pclNode_) { DoubleLinkList::SetTail(reinterpret_cast<LinkListNode*>(pclNode_)); }

    /**
     *  @brief Add
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to add
     */
    inline void Add(T* pNode_)
    {
        DoubleLinkList::Add(reinterpret_cast<LinkListNode*>(pNode_));
    }

    /**
     *  @brief Remove
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to remove
     */
    inline void Remove(T* pNode_)
    {
        DoubleLinkList::Remove(reinterpret_cast<LinkListNode*>(pNode_));
    }
};

//---------------------------------------------------------------------------
/**
    Circular-linked-list data type, inherited from the base LinkList type.
 */
template <typename T>
class TypedCircularLinkList : public CircularLinkList
{
public:
    void* operator new(size_t sz, void* pv) { return reinterpret_cast<TypedCircularLinkList<T>*>(pv); }

    TypedCircularLinkList<T>() {
        CircularLinkList();
    }

    inline T* GetHead() { return reinterpret_cast<T*>(CircularLinkList::GetHead()); }
    inline void SetHead(T* pclNode_) { CircularLinkList::SetHead(reinterpret_cast<LinkListNode*>(pclNode_)); }

    /**
     *  @brief GetTail
     *
     *  Get the tail node of the linked list
     *
     *  @return Pointer to the tail node in the list
     */
    inline T* GetTail() { return reinterpret_cast<T*>(CircularLinkList::GetTail()); }
    inline void SetTail(T* pclNode_) { CircularLinkList::SetTail(reinterpret_cast<LinkListNode*>(pclNode_)); }

    /**
     *  @brief
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to add
     */
    inline void Add(T* pNode_)
    {
        CircularLinkList::Add(reinterpret_cast<LinkListNode*>(pNode_));
    }

    /**
     *  @brief Remove
     *
     *  Add the linked list node to this linked list
     *
     *  @param node_ Pointer to the node to remove
     */
    inline void Remove(T* pNode_)
    {
        CircularLinkList::Remove(reinterpret_cast<LinkListNode*>(pNode_));
    }

    /**
     * @brief InsertNodeBefore
     *
     * Insert a linked-list node into the list before the specified insertion
     * point.
     *
     * @param node_     Node to insert into the list
     * @param insert_   Insert point.
     */
    inline void InsertNodeBefore(T* pNode_, T* pInsert_)
    {
        CircularLinkList::InsertNodeBefore(reinterpret_cast<LinkListNode*>(pNode_), reinterpret_cast<LinkListNode*>(pInsert_));
    }
};
} // namespace Mark3
