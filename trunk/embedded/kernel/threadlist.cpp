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
===========================================================================*/
/*!

    \file   threadlist.cpp

    \brief  Thread linked-list definitions

*/

#include "kerneltypes.h"
#include "ll.h"
#include "threadlist.h"
#include "thread.h"
#include "kerneldebug.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
	#undef __FILE_ID__
#endif
#define __FILE_ID__ 	THREADLIST_CPP       //!< File ID used in kernel trace calls

//---------------------------------------------------------------------------
void ThreadList::SetPriority(uint8_t u8Priority_)
{
    m_u8Priority = u8Priority_;
}

//---------------------------------------------------------------------------
void ThreadList::SetFlagPointer( uint8_t *pu8Flag_)
{
    m_pu8Flag = pu8Flag_;
}

//---------------------------------------------------------------------------
void ThreadList::Add(LinkListNode *node_) { 
    CircularLinkList::Add(node_);
    CircularLinkList::PivotForward();
    
    // We've specified a bitmap for this threadlist
    if (m_pu8Flag)
    {
        // Set the flag for this priority level
        *m_pu8Flag |= (1 << m_u8Priority);
    }
}

//---------------------------------------------------------------------------
void ThreadList::Add(LinkListNode *node_, uint8_t *pu8Flag_, uint8_t u8Priority_) {
    // Set the threadlist's priority level, flag pointer, and then add the
    // thread to the threadlist
    SetPriority(u8Priority_);
    SetFlagPointer(pu8Flag_);
    Add(node_);
}

//---------------------------------------------------------------------------
void ThreadList::Remove(LinkListNode *node_) {
    // Remove the thread from the list
    CircularLinkList::Remove(node_);
    
    // If the list is empty...
    if (!m_pstHead)
    {
        // Clear the bit in the bitmap at this priority level
        if (m_pu8Flag)
        {
            *m_pu8Flag &= ~(1 << m_u8Priority);
        }
    }
}

//---------------------------------------------------------------------------
Thread *ThreadList::HighestWaiter()
{
	Thread *pclTemp = static_cast<Thread*>(GetHead());
	Thread *pclChosen = pclTemp;
	
	uint8_t u8MaxPri = 0;
    
    // Go through the list, return the highest-priority thread in this list.
	while(1)
	{
        // Compare against current max-priority thread
		if (pclTemp->GetPriority() >= u8MaxPri)
		{
			u8MaxPri = pclTemp->GetPriority();
			pclChosen = pclTemp;
		}
        
        // Break out if this is the last thread in the list
		if (pclTemp == static_cast<Thread*>(GetTail()))
		{
			break;
		}
        
		pclTemp = static_cast<Thread*>(pclTemp->GetNext());		
	} 
	return pclChosen;
}
