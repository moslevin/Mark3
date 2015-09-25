/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
=========================================================================== */
/*!

    \file   threadlist.h    

    \brief  Thread linked-list declarations

 */

#ifndef __THREADLIST_H__
#define __THREADLIST_H__

#include "kerneltypes.h"
#include "ll.h"

class Thread;

/*!
    This class is used for building thread-management facilities, such as 
    schedulers, and blocking objects.
 */
class ThreadList : public CircularLinkList
{
public:    
    /*! 
     *  Default constructor - zero-initializes the data.
     */
    ThreadList() { m_u8Priority = 0; m_pu8Flag = NULL; }
    
    /*!
     *  \fn void SetPriority(uint8_t u8Priority_)
     *  
     *  Set the priority of this threadlist (if used for a scheduler).
     *  
     *  \param u8Priority_ Priority level of the thread list
     */
    void SetPriority(uint8_t u8Priority_);
    
    /*!
     *  \fn void SetFlagPointer(uint8_t *pu8Flag_)
     *  
     *  Set the pointer to a bitmap to use for this threadlist.  Once again,
     *  only needed when the threadlist is being used for scheduling purposes.
     *  
     *  \param pu8Flag_ Pointer to the bitmap flag
     */
    void SetFlagPointer(uint8_t *pu8Flag_);
    
    /*!
     *  \fn void Add(LinkListNode *node_)
     *  
     *  Add a thread to the threadlist.
     *  
     *  \param node_ Pointer to the thread (link list node) to add to the list
     */
    void Add(LinkListNode *node_);
    
    /*!
     *  \fn void Add(LinkListNode *node_, 
     *                  uint8_t *pu8Flag_, 
     *                  uint8_t u8Priority_)
     *  
     *  Add a thread to the threadlist, specifying the flag and priority at
     *  the same time.
     *  
     *  \param node_        Pointer to the thread to add (link list node)
     *  \param pu8Flag_     Pointer to the bitmap flag to set (if used in 
     *                      a scheduler context), or NULL for non-scheduler.
     *  \param u8Priority_  Priority of the threadlist
     */
    void Add(LinkListNode *node_, uint8_t *pu8Flag_, uint8_t u8Priority_);

    /*!
     *  \fn void Remove(LinkListNode *node_)
     *  
     *  Remove the specified thread from the threadlist
     *  
     *  \param node_ Pointer to the thread to remove
     */
    void Remove(LinkListNode *node_);
    
    /*!
     *  \fn Thread *HighestWaiter()
     *  
     *  Return a pointer to the highest-priority thread in the thread-list.
     *  
     *  \return Pointer to the highest-priority thread
     */
    Thread *HighestWaiter();
private:

    //! Priority of the threadlist
    uint8_t m_u8Priority;
    
    //! Pointer to the bitmap/flag to set when used for scheduling.
    uint8_t *m_pu8Flag;
};

#endif

