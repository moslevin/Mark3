/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file gui.cpp
    \brief Graphical user Interface classes and data structure definitions.
*/

#include "message.h"
#include "kerneltypes.h"
#include "gui.h"
#include "system_heap.h"
#include "fixed_heap.h"
#include "memutil.h"
#include "kernelaware.h"

//---------------------------------------------------------------------------
void GuiWindow::AddControl( GuiControl *pclControl_, GuiControl *pclParent_ )
{
    GUI_DEBUG_PRINT("GuiWindow::AddControl\n");

    m_clControlList.Add(static_cast<LinkListNode*>(pclControl_));
    m_pclInFocus = pclControl_;
    m_u8ControlCount++;

    pclControl_->SetParentWindow(this);
    pclControl_->SetParentControl(pclParent_);
}

//---------------------------------------------------------------------------
void GuiWindow::RemoveControl( GuiControl *pclControl_ )
{
    GUI_DEBUG_PRINT("GuiWindow::RemoveControl\n");

    if (pclControl_->GetPrev())
    {
        m_pclInFocus = static_cast<GuiControl*>(pclControl_->GetPrev());
    }
    else if (pclControl_->GetNext())
    {
        m_pclInFocus = static_cast<GuiControl*>(pclControl_->GetNext());
    }
    else
    {
        m_pclInFocus = NULL;
    }
    m_clControlList.Remove(static_cast<LinkListNode*>(pclControl_));
    m_u8ControlCount--;
}

//---------------------------------------------------------------------------
uint8_t GuiWindow::GetMaxZOrder()
{
    GUI_DEBUG_PRINT("GuiWindow::GetMaxZOrder\n");

    LinkListNode *pclTempNode;
    uint8_t u8Z = 0;
    uint8_t u8TempZ;

    pclTempNode = m_clControlList.GetHead();

    while (pclTempNode)
    {
        u8TempZ = (static_cast<GuiControl*>(pclTempNode))->GetZOrder();
        if (u8TempZ > u8Z)
        {
            u8Z = u8TempZ;
        }
        pclTempNode = pclTempNode->GetNext();
    }

    return u8Z;
}

//---------------------------------------------------------------------------
void GuiWindow::Redraw( bool bRedrawAll_ )
{
    GUI_DEBUG_PRINT("GuiWindow::Redraw\n");

    uint8_t u8ControlsLeft = m_u8ControlCount;
    uint8_t u8CurrentZ = 0;
    uint8_t u8MaxZ;

    u8MaxZ = GetMaxZOrder();

    // While there are still controls left to process (and we're less than
    // the maximum Z-order, just a sanity check.), redraw each object that
    // has its stale flag set, or all controls if the bRedrawAll_ parameter
    // is true.
    while (u8ControlsLeft && (u8CurrentZ <= u8MaxZ))
    {
        LinkListNode *pclTempNode;

        pclTempNode = m_clControlList.GetHead();
        while (pclTempNode)
        {
            GuiControl* pclTempControl = static_cast<GuiControl*>(pclTempNode);
            if (pclTempControl->GetZOrder() == u8CurrentZ)
            {
                if ((bRedrawAll_) || (pclTempControl->IsStale()))
                {
                    pclTempControl->Draw();
                    pclTempControl->ClearStale();
                }

                u8ControlsLeft--;
            }

            pclTempNode = pclTempNode->GetNext();
        }
        u8CurrentZ++;
    }
    GUI_DEBUG_PRINT("  Current Z: %d\n", u8CurrentZ);
    GUI_DEBUG_PRINT("  Controls Left: %d\n", u8ControlsLeft);
}

//---------------------------------------------------------------------------
void GuiWindow::InvalidateRegion( uint16_t u16Left_, uint16_t u16Top_, uint16_t u16Width_, uint16_t u16Height_ )
{
    LinkListNode *pclTempNode;
    uint16_t u16Left1, u16Left2, u16Right1, u16Right2, u16Top1, u16Top2, u16Bottom1, u16Bottom2;

    pclTempNode = m_clControlList.GetHead();

    u16Left1 = u16Left_;
    u16Right1 = u16Left_ + u16Width_ - 1;
    u16Top1 = u16Top_;
    u16Bottom1 = u16Top_ + u16Height_ - 1;

    while (pclTempNode)
    {
        GuiControl *pclControl = static_cast<GuiControl*>(pclTempNode);
        uint16_t u16X, u16Y;

        bool bMatch = false;

        // Get the absolute display coordinates
        pclControl->GetControlOffset(&u16X, &u16Y);


        u16Left2 = pclControl->GetLeft() + u16X;
        u16Right2 = u16Left2 + pclControl->GetWidth() - 1;
        u16Top2 = pclControl->GetTop() + u16Y;
        u16Bottom2 = u16Top2 + pclControl->GetHeight() - 1;

        // If the control has any pixels in the bounding box.
        if (
                (
                    (
                        (u16Left1 >= u16Left2) &&
                        (u16Left1 <= u16Right2)
                    ) ||
                    (
                        (u16Right1 >= u16Left2) &&
                        (u16Right1 <= u16Right2)
                    ) ||
                    ((u16Left1 <= u16Left2) && (u16Right1 >= u16Right2))
                ) &&
                (
                    (
                        (u16Top1 >= u16Top2) &&
                        (u16Top1 <= u16Bottom2)
                    ) ||
                    (
                        (u16Bottom1 >= u16Top2) &&
                        (u16Bottom1 <= u16Bottom2)
                    ) ||
                    ((u16Top1 <= u16Top2) && (u16Bottom1 >= u16Bottom2))
                )
            )
        {
            bMatch = true;            
        }
        else if(
                (
                    (
                        (u16Left2 >= u16Left1) &&
                        (u16Left2 <= u16Right1)
                    ) ||
                    (
                        (u16Right2 >= u16Left1) &&
                        (u16Right2 <= u16Right1)
                    ) ||
                    ((u16Left2 <= u16Left1) && (u16Right2 >= u16Right1))
                ) &&
                (
                    (
                        (u16Top2 >= u16Top1) &&
                        (u16Top2 <= u16Bottom1)
                    ) ||
                    (
                        (u16Bottom2 >= u16Top1) &&
                        (u16Bottom2 <= u16Bottom1)
                    ) ||
                    ((u16Top2 <= u16Top1) && (u16Bottom2 >= u16Bottom1))
                )
            )
        {
            bMatch = true;            
        }


        if (bMatch)
        {
            pclControl->SetStale();

            // Invalidate all child controls as well (since redrawing a parent could cause them to disappear)
            GuiControl *pclChild = static_cast<GuiControl*>(m_clControlList.GetHead());

            // Go through all controls and check for parental ancestry
            while (pclChild)
            {
                GuiControl *pclParent = static_cast<GuiControl*>(pclChild->GetParentControl());

                // If this control is a descendant of the current control at some level
                while (pclParent)
                {
                    if (pclParent == pclControl)
                    {
                        // Set the control as stale
                        pclChild->SetStale();
                        break;
                    }
                    pclParent = pclParent->GetParentControl();
                }

                pclChild = static_cast<GuiControl*>((static_cast<LinkListNode*>(pclChild))->GetNext());
            }
        }

        pclTempNode = pclTempNode->GetNext();
    }
}

//---------------------------------------------------------------------------
void GuiWindow::ProcessEvent( GuiEvent_t *pstEvent_ )
{
    GUI_DEBUG_PRINT("GuiWindow::ProcessEvent\n");

    // If the event is for broadcast - send it to all controls,
    // without regard to order.
    if ((TARGET_ID_BROADCAST == pstEvent_->u8TargetID)
        || (TARGET_ID_BROADCAST_Z == pstEvent_->u8TargetID))
    {
        GUI_DEBUG_PRINT("  TARGET_ID_BROADCAST(_Z)\n");

        LinkListNode *pclTempNode;
        pclTempNode = m_clControlList.GetHead();

        while (pclTempNode)
        {
            GuiReturn_t eRet;
            eRet = (static_cast<GuiControl*>(pclTempNode))->ProcessEvent(pstEvent_);
            if (GUI_EVENT_CONSUMED == eRet)
            {
                break;
            }
            pclTempNode = pclTempNode->GetNext();
        }
    }
    // Send the event only to the currently-selected object.
    else if (TARGET_ID_FOCUS == pstEvent_->u8TargetID)
    {
        GUI_DEBUG_PRINT("  TARGET_ID_FOCUS\n");
        GuiReturn_t eReturn = GUI_EVENT_OK;

        // Try to let the control process the event on its own
        if (m_pclInFocus)
        {
            eReturn = m_pclInFocus->ProcessEvent(pstEvent_);
        }

        // If the event was not consumed, use default logic to process the event
        if (GUI_EVENT_CONSUMED != eReturn)
        {
            if (EVENT_TYPE_KEYBOARD == pstEvent_->u8EventType)
            {
                if (KEYCODE_TAB == pstEvent_->stKey.u8KeyCode)
                {
                    if (pstEvent_->stKey.bKeyState)
                    {
                        CycleFocus(true);
                    }
                }
            }
            else if (EVENT_TYPE_JOYSTICK == pstEvent_->u8EventType)
            {
                if ((pstEvent_->stJoystick.Current.bUp && !(pstEvent_->stJoystick.Previous.bUp))
                        || (pstEvent_->stJoystick.Current.bLeft && !(pstEvent_->stJoystick.Previous.bLeft)))
                {
                    // Cycle focus *backwards*
                    CycleFocus(false);
                }
                else if ((pstEvent_->stJoystick.Current.bRight && !(pstEvent_->stJoystick.Previous.bRight))
                         || (pstEvent_->stJoystick.Current.bDown && !(pstEvent_->stJoystick.Previous.bDown)))
                {
                    // Cycle focus *forewards*
                    CycleFocus(true);
                }
            }
        }
    }
    else if (TARGET_ID_HIGH_Z == pstEvent_->u8TargetID)
    {
        GUI_DEBUG_PRINT("  TARGET_ID_HIGH_Z\n");

        uint16_t u16TargetX, u16TargetY;
        uint16_t u16OffsetX, u16OffsetY;
        uint8_t u8MaxZ = 0;

        LinkListNode *pclTempNode;
        pclTempNode = m_clControlList.GetHead();

        switch (pstEvent_->u8EventType)
        {
            case EVENT_TYPE_MOUSE:
            case EVENT_TYPE_TOUCH:
            {
                GuiControl *pclTargetControl = NULL;

                // Read the target X/Y coordinates out of the event struct
                if (EVENT_TYPE_TOUCH == pstEvent_->u8EventType)
                {
                    u16TargetX = pstEvent_->stTouch.u16X;
                    u16TargetY = pstEvent_->stTouch.u16Y;
                }
                else
                {
                    u16TargetX = pstEvent_->stMouse.u16X;
                    u16TargetY = pstEvent_->stMouse.u16Y;
                }

                // Go through every control on the window, checking to see if the
                // event falls within the bounding box
                   while (pclTempNode)
                {
                    GuiControl *pclControl = (static_cast<GuiControl*>(pclTempNode));

                    pclControl->GetControlOffset(&u16OffsetX, &u16OffsetY);

                    // Compare event coordinates to bounding box (with offsets)
                    if ( ((u16TargetX >= (u16OffsetX + pclControl->GetLeft()) &&
                          (u16TargetX <= (u16OffsetX + pclControl->GetLeft() + pclControl->GetWidth() - 1)))) &&
                         ((u16TargetY >= (u16OffsetY + pclControl->GetTop()) &&
                          (u16TargetY <= (u16OffsetY + pclControl->GetTop() + pclControl->GetHeight() - 1)))) )
                    {
                        // If this control is higher in Z-Order, set this as the newest
                        // candidate control to accept the event
                        if (pclControl->GetZOrder() >= u8MaxZ)
                        {
                            pclTargetControl = pclControl;
                            u8MaxZ = pclControl->GetZOrder();
                        }
                    }

                    pclTempNode = pclTempNode->GetNext();
                }

                // If a suitable control was found on the event surface, pass the event off
                // for processing.
                if (pclTargetControl)
                {
                    // If the selected control is different from the current in-focus
                    // control, then deactive that control.
                    if (m_pclInFocus && (m_pclInFocus != pclTargetControl))
                    {
                        m_pclInFocus->Activate(false);
                        m_pclInFocus = NULL;
                    }
                    (static_cast<GuiControl*>(pclTargetControl))->ProcessEvent(pstEvent_);
                }
            }
                break;
            default:
                break;
        }
    }
}
//---------------------------------------------------------------------------
void GuiWindow::SetFocus( GuiControl *pclControl_ )
{
    GUI_DEBUG_PRINT("GuiWindow::SetFocus\n");

    m_pclInFocus = pclControl_;
}

//---------------------------------------------------------------------------
void GuiWindow::CycleFocus( bool bForward_ )
{
    GUI_DEBUG_PRINT("GuiWindow::CycleFocus\n");

    // Set starting point and cached copy of current nodes
    LinkListNode *pclTempNode = static_cast<GuiControl*>(m_clControlList.GetHead());
    LinkListNode *pclStartNode = m_pclInFocus;

    if (bForward_)
    {
        // If there isn't a current focus node, set the focus to the beginning
        // of the list
        if (!m_pclInFocus)
        {
            m_pclInFocus = static_cast<GuiControl*>(pclTempNode);
            if (!m_pclInFocus)
            {
                return;
            }
            pclTempNode = static_cast<GuiControl*>(m_pclInFocus);
            pclStartNode = NULL;
        }
        else
        {
            // Deactivate the control that's losing focus
            static_cast<GuiControl*>(m_pclInFocus)->Activate(false);

            // Otherwise start with the next node
            pclTempNode = pclStartNode->GetNext();
        }

        // Go through the whole control list and find the next one to accept
        // the focus
        while (pclTempNode && (pclTempNode != pclStartNode))
        {
            if (static_cast<GuiControl*>(pclTempNode)->AcceptsFocus())
            {
                m_pclInFocus = static_cast<GuiControl*>(pclTempNode);
                m_pclInFocus->Activate(true);
                SetFocus(m_pclInFocus);
                return;
            }

            pclTempNode = pclTempNode->GetNext();
        }

        // Reached the end of the list.  Start again from the beginning and
        // try again
        pclTempNode = static_cast<GuiControl*>(m_clControlList.GetHead());
        while (pclTempNode && (pclTempNode != pclStartNode))
        {
            if (static_cast<GuiControl*>(pclTempNode)->AcceptsFocus())
            {
                m_pclInFocus = static_cast<GuiControl*>(pclTempNode);
                m_pclInFocus->Activate(true);
                SetFocus(m_pclInFocus);
                return;
            }

            pclTempNode = pclTempNode->GetNext();
        }
    }
    else
    {
        pclTempNode = static_cast<GuiControl*>(m_clControlList.GetTail());
        pclStartNode = m_pclInFocus;

        // If there isn't a current focus node, set the focus to the end
        // of the list
        if (!m_pclInFocus)
        {
            m_pclInFocus = static_cast<GuiControl*>(pclTempNode);
            if (!m_pclInFocus)
            {
                return;
            }
            pclTempNode = static_cast<GuiControl*>(m_pclInFocus);
            pclStartNode = NULL;
        }
        else
        {
            // Deactivate the control that's losing focus
            static_cast<GuiControl*>(m_pclInFocus)->Activate(false);

            // Otherwise start with the previous node
            pclTempNode = pclStartNode->GetPrev();
        }

        // Go through the whole control list and find the next one to accept
        // the focus
        while (pclTempNode && (pclTempNode != pclStartNode))
        {
            if (static_cast<GuiControl*>(pclTempNode)->AcceptsFocus())
            {
                m_pclInFocus = static_cast<GuiControl*>(pclTempNode);
                m_pclInFocus->Activate(true);
                SetFocus(m_pclInFocus);
                return;
            }
            pclTempNode = pclTempNode->GetPrev();
        }

        pclTempNode = static_cast<GuiControl*>(m_clControlList.GetTail());
        while (pclTempNode && (pclTempNode != pclStartNode))
        {
            if (static_cast<GuiControl*>(pclTempNode)->AcceptsFocus())
            {
                m_pclInFocus = static_cast<GuiControl*>(pclTempNode);
                m_pclInFocus->Activate(true);
                SetFocus(m_pclInFocus);
                return;
            }
            pclTempNode = pclTempNode->GetPrev();
        }
    }
}
//---------------------------------------------------------------------------
GuiWindow *GuiEventSurface::FindWindowByName( const char *szName_ )
{
    LinkListNode *pclTempNode = static_cast<LinkListNode*>(m_clWindowList.GetHead());

    while (pclTempNode)
    {
        if (MemUtil::CompareStrings(szName_, static_cast<GuiWindow*>(pclTempNode)->GetName()))
        {
             return static_cast<GuiWindow*>(pclTempNode);
        }
        pclTempNode = pclTempNode->GetNext();
    }

    return NULL;
}

//---------------------------------------------------------------------------
void GuiEventSurface::AddWindow( GuiWindow *pclWindow_ )
{
    GUI_DEBUG_PRINT("GuiEventSurface::AddWindow\n");

    m_clWindowList.Add(static_cast<LinkListNode*>(pclWindow_));
}

//---------------------------------------------------------------------------
void GuiEventSurface::RemoveWindow( GuiWindow *pclWindow_ )
{
    GUI_DEBUG_PRINT("GuiEventSurface::RemoveWindow\n");

    m_clWindowList.Remove(static_cast<LinkListNode*>(pclWindow_));
}

//---------------------------------------------------------------------------
bool GuiEventSurface::SendEvent( GuiEvent_t *pstEvent_ )
{
    GUI_DEBUG_PRINT("GuiEventSurface::SendEvent\n");

    // Allocate a message from the global message pool
    Message *pclMessage = GlobalMessagePool::Pop();

    // No messages available? Return a failure
    if (!pclMessage)
    {
        return false;
    }

    // Allocate a copy of the event from the heap
    GuiEvent_t *pstEventCopy = static_cast<GuiEvent_t*>(SystemHeap::Alloc(sizeof(GuiEvent_t)));

    // If the allocation fails, push the message back to the global pool and bail
    if (!pstEventCopy)
    {
        GlobalMessagePool::Push(pclMessage);
        return false;
    }

    // Copy the source event into the destination event buffer
    CopyEvent(pstEventCopy, pstEvent_);

    // Set the new event as the message payload
    pclMessage->SetData(static_cast<void*>(pstEventCopy));

    // Send the event to the message queue
    m_clMessageQueue.Send(pclMessage);

    return true;
}

//---------------------------------------------------------------------------
bool GuiEventSurface::ProcessEvent()
{
    GUI_DEBUG_PRINT("GuiEventSurface::ProcessEvent\n");

    // read the event from the queue (blocking call)
    Message *pclMessage = m_clMessageQueue.Receive();
    GuiEvent_t stLocalEvent;

    // If we failed to get something from the queue,
    // bail out
    if (!pclMessage)
    {
        return false;
    }

    // Copy the event data from the message into a local copy
    CopyEvent(&stLocalEvent,
        static_cast<GuiEvent_t*>(pclMessage->GetData()));

    // Free the message and event as soon as possible, since
    // they are shared system resources
    SystemHeap::Free(pclMessage->GetData());
    GlobalMessagePool::Push(pclMessage);

    // Special case check - target ID is the highest Z-ordered window(s) ONLY.
    if (stLocalEvent.u8TargetID == TARGET_ID_BROADCAST_Z)
    {
        LinkListNode* pclTempNode = m_clWindowList.GetHead();
        uint8_t u8MaxZ = 0;

        while (pclTempNode)
        {
            if (u8MaxZ < (static_cast<GuiWindow*>(pclTempNode))->GetZOrder() )
            {
                u8MaxZ = static_cast<GuiWindow*>(pclTempNode)->GetZOrder();
            }
            pclTempNode = pclTempNode->GetNext();
        }

        // Iterate through all windows again - may have multiple windows
        // at the same z-order.
        pclTempNode = m_clWindowList.GetHead();
        while (pclTempNode)
        {
            if (u8MaxZ == (static_cast<GuiWindow*>(pclTempNode))->GetZOrder())
            {
                (static_cast<GuiWindow*>(pclTempNode))->ProcessEvent(&stLocalEvent);
            }
            pclTempNode = pclTempNode->GetNext();
        }
    }
    // Broadcast the event - sending it to *all* windows.  Let the individual
    // windows figure out what to do with the events.
    else
    {
        LinkListNode* pclTempNode = m_clWindowList.GetHead();
        while (pclTempNode)
        {
            (static_cast<GuiWindow*>(pclTempNode))->ProcessEvent(&stLocalEvent);
            pclTempNode = pclTempNode->GetNext();
        }
    }

    // Return out
    return true;
}

//---------------------------------------------------------------------------
void GuiEventSurface::CopyEvent( GuiEvent_t *pstDst_, GuiEvent_t *pstSrc_ )
{
    GUI_DEBUG_PRINT("GuiEventSurface::CopyEvent\n");
    uint8_t *pu8Dst_ = (uint8_t*)pstDst_;
    uint8_t *pu8Src_ = (uint8_t*)pstSrc_;
    uint8_t i;
    for (i = 0; i < sizeof(GuiEvent_t); i++)
    {
        *pu8Dst_++ = *pu8Src_++;
    }
}

//---------------------------------------------------------------------------
void GuiEventSurface::InvalidateRegion( uint16_t u16Left_, uint16_t u16Top_, uint16_t u16Width_, uint16_t u16Height_ )
{
    LinkListNode* pclTempNode = m_clWindowList.GetHead();
    while (pclTempNode)
    {
        (static_cast<GuiWindow*>(pclTempNode))->InvalidateRegion(u16Left_, u16Top_, u16Width_, u16Width_);
        pclTempNode = pclTempNode->GetNext();
    }
}

//---------------------------------------------------------------------------
void GuiControl::GetControlOffset( uint16_t *pu16X_, uint16_t *pu16Y_ )
{
    GUI_DEBUG_PRINT("GuiControl::GetControlOffset\n");
    GuiControl *pclTempControl = m_pclParentControl;
    *pu16X_ = 0;
    *pu16Y_ = 0;
    while (pclTempControl)
    {
        *pu16X_ += pclTempControl->GetLeft();
        *pu16Y_ += pclTempControl->GetTop();
        pclTempControl = pclTempControl->GetParentControl();
    }

    if (m_pclParentWindow)
    {
        *pu16X_ += m_pclParentWindow->GetLeft();
        *pu16Y_ += m_pclParentWindow->GetTop();
    }
}
