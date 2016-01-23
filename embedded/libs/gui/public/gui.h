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
=========================================================================== */
/*!
    \file gui.h
    \brief Graphical user Interface classes and data structure declarations.
 */

#ifndef __GUI_H__
#define __GUI_H__

#include "kerneltypes.h"
#include "ll.h"
#include "driver.h"
#include "graphics.h"

#include "message.h"

#include "keycodes.h"

#define GUI_DEBUG            (0)

#if GUI_DEBUG
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define GUI_DEBUG_PRINT        printf
#else
#define GUI_DEBUG_PRINT(...)
#endif


//---------------------------------------------------------------------------
/*!
    Event state defintions, used for determining whether or not a button or
    key is in the "up" or "down" contact state.
 */
#define EVENT_STATE_UP            (0)
#define EVENT_STATE_DOWN          (1)

//---------------------------------------------------------------------------
#define MAX_WINDOW_CONTROLS        (251) //!< Maximum number of controls per window

#define TARGET_ID_BROADCAST_Z      (252) //!< Broadcast event to all controls in the topmost window
#define TARGET_ID_BROADCAST        (253) //!< Send event to all controls in all windows
#define TARGET_ID_FOCUS            (254) //!< Send event to the in-focus control
#define TARGET_ID_HIGH_Z           (255) //!< Send event to the highest Z-order control


//---------------------------------------------------------------------------
/*!
    Enumeration defining the various UI event codes.
 */
typedef enum
{
    EVENT_TYPE_KEYBOARD,     //!< Keypress event
    EVENT_TYPE_MOUSE,        //!< Mouse movement or click event
    EVENT_TYPE_TOUCH,        //!< Touchscreen movement event
    EVENT_TYPE_JOYSTICK,     //!< Joystick event
    EVENT_TYPE_TIMER,        //!< Timer event
//---
    EVENT_TYPE_COUNT         //!< Count of different event types supported
} GuiEventType_t;

//---------------------------------------------------------------------------
/*!
    Keyboard UI event structure definition.
 */
typedef struct
{
    uint8_t u8KeyCode;        //!< 8-bit value representing a keyboard scan code
    union
    {
        uint8_t u8Flags;     //!< Flags indicating modifiers to the event
        struct
        {
            unsigned int bKeyState:1;        //!< Key is being pressed or released
            unsigned int bShiftState:1;      //!< Whether or not shift is pressed
            unsigned int bCtrlState:1;       //!< Whether or not CTRL is pressed
            unsigned int bAltState:1;        //!< Whether or not ALT it pressed
            unsigned int bWinState:1;        //!< Whether or not the Window/Clover key is pressed
            unsigned int bFnState:1;         //!< Whether or not a special function key is pressed
        };
    };
} KeyEvent_t;

//---------------------------------------------------------------------------
/*!
    Mouse UI event structure
 */
typedef struct
{
    uint16_t u16X;        //!< absolute X location of the mouse (pixel)
    uint16_t u16Y;        //!< absolute Y location of the mouse (pixel)

    union
    {
        uint8_t u8Flags;    //!< modifier flags for the event
        struct
        {
            unsigned int bLeftState:1;         //!< State of the left mouse button
            unsigned int bRightState:1;        //!< State of the right mouse button
            unsigned int bMiddleState:1;       //!< State of the middle mouse button
            unsigned int bScrollUp:1;          //!< State of the scroll wheel (UP)
            unsigned int bScrollDown:1;        //!< State of the scroll wheel (DOWN)
        };
    };
} MouseEvent_t;

//---------------------------------------------------------------------------
/*!
    Touch UI event structure
 */
typedef struct
{
    uint16_t u16X;        //!< Absolute touch location (pixels)
    uint16_t u16Y;        //!< Absolute touch location (pixels)

    union
    {
        uint16_t u8Flags;        //!< Modifier flags
        struct
        {
            unsigned int bTouch:1;    //!< Whether or not touch is up or down
        };
    };
} TouchEvent_t;

//---------------------------------------------------------------------------
/*!
    Joystick UI event structure
 */
typedef struct
{
    union
    {
        uint16_t u16RawData;            //!< Raw joystick data
        struct
        {
            unsigned int bUp:1;         //!< D-pad UP state
            unsigned int bDown:1;       //!< D-pad DOWN state
            unsigned int bLeft:1;       //!< D-pad LEFT state
            unsigned int bRight:1;      //!< D-pad RIGHT state

            unsigned int bButton1:1;    //!< Joystick Button1 state
            unsigned int bButton2:1;    //!< Joystick Button2 state
            unsigned int bButton3:1;    //!< Joystick Button3 state
            unsigned int bButton4:1;    //!< Joystick Button4 state
            unsigned int bButton5:1;    //!< Joystick Button5 state
            unsigned int bButton6:1;    //!< Joystick Button6 state
            unsigned int bButton7:1;    //!< Joystick Button7 state
            unsigned int bButton8:1;    //!< Joystick Button8 state
            unsigned int bButton9:1;    //!< Joystick Button9 state
            unsigned int bButton10:1;   //!< Joystick Button10 state

            unsigned int bSelect:1;     //!< Start button state
            unsigned int bStart:1;      //!< Select button state
        };
    } Current;
    union
    {
        uint16_t u16RawData;            //!< Raw joystick data
        struct
        {
            unsigned int bUp:1;         //!< D-pad UP state
            unsigned int bDown:1;       //!< D-pad DOWN state
            unsigned int bLeft:1;       //!< D-pad LEFT state
            unsigned int bRight:1;      //!< D-pad RIGHT state

            unsigned int bButton1:1;    //!< Joystick Button1 state
            unsigned int bButton2:1;    //!< Joystick Button2 state
            unsigned int bButton3:1;    //!< Joystick Button3 state
            unsigned int bButton4:1;    //!< Joystick Button4 state
            unsigned int bButton5:1;    //!< Joystick Button5 state
            unsigned int bButton6:1;    //!< Joystick Button6 state
            unsigned int bButton7:1;    //!< Joystick Button7 state
            unsigned int bButton8:1;    //!< Joystick Button8 state
            unsigned int bButton9:1;    //!< Joystick Button9 state
            unsigned int bButton10:1;   //!< Joystick Button10 state

            unsigned int bSelect:1;     //!< Start button state
            unsigned int bStart:1;      //!< Select button state
        };
    } Previous;
} JoystickEvent_t;

//---------------------------------------------------------------------------
/*!
    Timer UI event structure
 */
typedef struct
{
    uint16_t u16Ticks;    //!< Number of clock ticks (arbitrary) that have elapsed
} TimerEvent_t;

//---------------------------------------------------------------------------
/*!
    Composite UI event structure.  Depending on the event type, can contain
    either a keyboard, mouse, touch, joystick, timer event, etc.
 */
typedef struct
{
    uint8_t u8EventType;        //!< GuiEventType_t event type
    uint8_t u8TargetID;         //!< Control index that this event is targeted towards
    union
    {
        KeyEvent_t        stKey;     //!< Keyboard data
        MouseEvent_t    stMouse;    //!< Mouse data
        TouchEvent_t    stTouch;    //!< Touchscreen data
        JoystickEvent_t stJoystick; //!< Joystick data
        TimerEvent_t    stTimer;    //!< Timer data
    };

} GuiEvent_t;

//---------------------------------------------------------------------------
typedef enum
{
    GUI_EVENT_OK = 0,       //!< No problem
    GUI_EVENT_CONSUMED,     //!< Event was consumed
    GUI_EVENT_CANCEL,       //!< Event processing canceled
    GUI_EVENT_RETRY,        //!< Retry processing the event
//---
    GUI_EVENT_COUNT
} GuiReturn_t;

class GuiControl;

//---------------------------------------------------------------------------
/*!
    \brief Basic Window Class

    A Window is loosely defined as a container of controls, all sharing a
    coordinate reference coordinate frame.  Events are managed on a per-window
    basis, and each window is isolated from eachother.
 */
class GuiWindow : public LinkListNode
{

public:
    /*!
     *  Initialize the GUI Window object prior to use.  Must be called before
     *  calling other methods on this object
     */
    void Init()
    {
        m_u8ControlCount = 0;
        m_pclDriver = NULL;
        m_szName = "";
        ClearNode();
    }

    /*!
     *  \brief SetDriver
     *
     *  Set the graphics driver to use for rendering controls on the window.
     *
     *  \param pclDriver_ Pointer to the graphics driver
     */
    void SetDriver( GraphicsDriver *pclDriver_ ) { m_pclDriver = pclDriver_; }

    /*!
     *  \brief GetDriver
     *
     *  Set the graphics driver to use for rendering controls on the window.
     *
     *  \return Pointer to the Window's graphics driver
     */
    GraphicsDriver *GetDriver() { return m_pclDriver; }

    /*!
     *  \brief AddControl
     *
     *  Assign a GUI Control to this window object.  Adding an object to a
     *  window ensures that the object will be drawn on the specific window
     *  surface, and ensures that events directed to this window will be
     *  forwarded to the controls appropriately.
     *
     *  \param pclControl_ Pointer to the control object to add
     *  \param pclParent_  Pointer to the control's "parent" object (or NULL)
     */
    void AddControl( GuiControl *pclControl_, GuiControl *pclParent_ );

    /*!
     *  \brief RemoveControl
     *
     *  Removes a previously-added control from the Window.
     *
     *  \param pclControl_ Pointer to the control object to remove
     */
    void RemoveControl( GuiControl *pclControl_ );

    /*!
     *  \brief GetMaxZOrder
     *
     *  Returns the highest Z-Order of all controls attached to this window.
     *
     *  \return The highest Z-Order used by controls in this window
     */
    uint8_t GetMaxZOrder();

    /*!
     *  \brief Redraw
     *
     *  Redraw objects in the window.  Typically, only the affected controls
     *  will need to be redrawn, but in some cases (such as window
     *  initialization), the entire window will need to be redrawn cleanly.
     *  This behavior is defined by the value of the bRedrawAll_ parameter.
     */
    void Redraw( bool bRedrawAll_ );

    /*!
     *  \brief ProcessEvent
     *
     *  Process an event sent to this window.  This method handles all
     *  of the plumbing required to target the event towards specific
     *  controls, or all controls in the window depending on the
     *  event payload.
     */
    void ProcessEvent( GuiEvent_t *pstEvent_ );

    /*!
     *  \brief SetFocus
     *
     *  Set the control used to accept "focus" events.  Such events include
     *  keyboard events.
     *
     *  \param pclControl_ Pointer to the control object to set focus on.
     */
    void SetFocus( GuiControl *pclControl_ );

    /*!
     *  \brief IsInFocus
     *
     *  Return whether or not the selected control is in focus or not.
     *
     *  \param pclControl_ Pointer to the control object to evaluate
     *
     *  \return true - the selected control is the active control on the window
     *          false - otherwise
     */
    bool IsInFocus( GuiControl *pclControl_ )
    {
        if (m_pclInFocus == pclControl_)
        {
            return true;
        }
        return false;
    }

    /*!
     *  \brief SetTop
     *
     *  Set the location of the topmost pixel of the window
     *
     *  \param u16Top_ Topmost pixel of the window
     */
    void SetTop( uint16_t u16Top_ )          { m_u16Top = u16Top_; }

    /*!
     *  \brief SetLeft
     *
     *  Set the location of the leftmost pixel of the window
     *
     *  \param u16Left_ Leftmost pixel of the window
     */
    void SetLeft( uint16_t u16Left_ )      { m_u16Left = u16Left_; }

    /*!
     *  \brief SetHeight
     *
     *  Set the height of the window (in pixels)
     *
     *  \param u16Height_ Height of the window in pixels
     */
    void SetHeight( uint16_t u16Height_ ) { m_u16Height = u16Height_; }

    /*!
     *  \brief SetWidth
     *
     *  Set the width of the window (in pixels)
     *
     *  \param u16Width_ Width of the window in pixels
     */
    void SetWidth( uint16_t u16Width_ )      { m_u16Width = u16Width_; }

    /*!
     *  \brief GetTop
     *
     *  Return the topmost pixel of the window
     *
     *  \return Topmost pixel of the window
     */
    uint16_t GetTop()             { return m_u16Top; }

    /*!
     *  \brief GetLeft
     *
     *  Return the leftmost pixel of the window
     *
     *  \return Leftmost pixel of the window
     */
    uint16_t GetLeft()             { return m_u16Left; }

    /*!
     *  \brief GetHeight
     *
     *  Get the height of the window in pixels
     *
     *  \return Height of the window in pixels
     */
    uint16_t GetHeight()         { return m_u16Height; }

    /*!
     *  \brief GetWidth
     *
     *  Get the width of the window in pixels
     *
     *  \return Width of the window in pixels
     */
    uint16_t GetWidth()         { return m_u16Width; }

    /*!
     *  \brief GetZOrder
     *
     *  Get the Z-order of the window on the event surface
     */
    uint8_t GetZOrder()         { return m_u8Z; }

    /*!
     *  \brief SetZOrder
     *
     *  Set the Z-order of the window on the event surface
     */
    void SetZOrder( uint8_t u8Z_ ) { m_u8Z = u8Z_; }

    /*!
     *  \brief CycleFocus
     *
     *  Cycle the focus to the next active control in the window
     *
     *  \param bForward_ - Cycle to the next control when true,
     *                     previous control when false
     */
    void CycleFocus( bool bForward_ );

    /*!
     *  \brief SetName
     *
     *  Set the name for this window
     */
    void SetName( const char *szName_ ) { m_szName = szName_; }

    /*!
     *  \brief GetName

     *  Return the name of this window
     */
    const char *GetName() { return m_szName; }

    /*!
     *  \brief InvalidateRegion
     *
     *  Invalidate a region of the window, specified by the bounding box.  The
     *  coordinates specified in the parameters (top and left) refer to absolute
     *  display coordinates, and are not relative to coordinates within a window.
     */
    void InvalidateRegion( uint16_t u16Left_, uint16_t u16Top_, uint16_t u16Width_, uint16_t u16Height_ );

private:
    uint16_t m_u16Top;       //!< Topmost pixel of the window on the event surface
    uint16_t m_u16Left;      //!< Leftmost pixel of the window on the event surface
    uint16_t m_u16Height;    //!< Height of the window in pixels
    uint16_t m_u16Width;        //!< Width of the window in pixels

    uint8_t  m_u8Z;             //!< Z-order of the window on the event surface
    const char  *m_szName;      //!< Name applied to this window

    DoubleLinkList m_clControlList;  //!< List of controls managed by this window
    GuiControl *m_pclInFocus;        //!< Pointer to the control in event focus
    uint8_t m_u8ControlCount;        //!< Number of controls in this window
    GraphicsDriver *m_pclDriver;     //!< Graphics driver for this window.
};

//---------------------------------------------------------------------------
/*!
    \brief GUI Event Surface Object

    An event surface is the lowest-level UI object.  It maintains a list of
    windows which are associated with it, and manages the transmission and
    routing of events to each window, and their appropriate controls

    All windows located on the event surface are assumed to share a common
    display, and coordinate frame.  In this way, multiple GUIs can be
    implemented in the system, each tied to separate physical or virtual
    displays.
 */
class GuiEventSurface
{
public:
    /*!
     *  \brief Init
     *
     *  Initialize an event surface before use.  Must be called prior to
     *  any other object methods.
     */
    void Init() { m_clMessageQueue.Init(); m_clWindowList.Init(); }

    /*!
     *  \brief AddWindow
     *
     *  Add a window to the event surface.
     *
     *  \param pclWindow_ Pointer to the window object to add to the sruface
     */
    void AddWindow( GuiWindow *pclWindow_ );

    /*!
     *  \brief RemoveWindow
     *
     *  Remove a window from the event surface.
     *
     *  \param pclWindow_ Pointer to the window object to remove from the surface
     */
    void RemoveWindow( GuiWindow *pclWindow_ );

    /*!
     *  \brief SendEvent
     *
     *  Send an event to this window surface.  The event will be forwraded to
     *  all windows managed by this service.
     *
     *  \param pstEvent_ Pointer to an event to send
     *  \return true on success, false on failure
     */
    bool SendEvent( GuiEvent_t *pstEvent_ );

    /*!
     *  \brief ProcessEvent
     *  Process an event in the event queue.  If no events are pending, the
     *  call will block until an event is available.
     */
    bool ProcessEvent();

    /*!
     *  \brief GetEventCount
     *
     *  Get the count of pending events in the event surface's queue.
     */
    uint8_t GetEventCount() { return m_clMessageQueue.GetCount(); }

    /*!
     *  \brief FindWindowByName
     *
     *  Return a pointer to a window by name, or NULL on failure
     */
    GuiWindow *FindWindowByName( const char *szName_ );

    /*!
     *  \brief InvalidateRegion
     *
     *  Invalidate a region of the window, specified by the bounding box.  The
     *  coordinates specified in the parameters (top and left) refer to absolute
     *  display coordinates, and are not relative to coordinates within a window.
     */
    void InvalidateRegion( uint16_t u16Left_, uint16_t u16Top_, uint16_t u16Width_, uint16_t u16Height_ );

private:
    /*!
     *  \brief CopyEvent
     *
     *  Copy the contents of one message structure to another.
     *
     *  \param pstDst_ Destination event pointer
     *  \param pstSrc_ Source event pointer
     */
    void CopyEvent( GuiEvent_t *pstDst_, GuiEvent_t *pstSrc_ );

private:
    /*!
     *  List of windows managed on this event surface
     */
    DoubleLinkList m_clWindowList;

    /*!
     *  Message queue used to manage window events
     */
    MessageQueue   m_clMessageQueue;
};

//---------------------------------------------------------------------------
/*!
    \brief GUI Control Base Class

    This class is the common ancestor to all GUI control elements.  It
    defines a base set of properties common to all controls, as well as
    methods for initialization, event handling, and redrawing.  Controls are
    directly related to Windows, which are used to manage and organize
    controls.
 */
class GuiControl : public LinkListNode
{
public:
    /*!
     *  \brief Init
     *
     *  Initiailize the control - must be called before use.
     *  Implementation is subclass specific.
     */
    virtual void Init() = 0;

    /*!
     *  \brief Draw
     *
     *  Redraw the control "cleanly".  Subclass specific.
     */
    virtual void Draw() = 0;

    /*!
     *  \brief ProcessEvent
     *
     *  Process an event sent to the control.  Subclass specific
     *  implementation.
     *
     *  \param pstEvent_ Pointer to a struct containing the event data
     */
    virtual GuiReturn_t ProcessEvent( GuiEvent_t *pstEvent_ ) = 0;

    /*!
     *  \brief SetTop
     *
     *  Set the location of the topmost pixel of the control
     *
     *  \param u16Top_ Topmost pixel of the control
     */
    void SetTop( uint16_t u16Top_ )          { m_u16Top = u16Top_; }

    /*!
     *  \brief SetLeft
     *
     *  Set the location of the leftmost pixel of the control
     *
     *  \param u16Left_ Leftmost pixel of the control
     */
    void SetLeft( uint16_t u16Left_ )      { m_u16Left = u16Left_; }

    /*!
     *  \brief SetHeight
     *
     *  Set the height of the control (in pixels)
     *
     *  \param u16Height_ Height of the control in pixels
     */
    void SetHeight( uint16_t u16Height_ ) { m_u16Height = u16Height_; }

    /*!
     *  \brief SetWidth
     *
     *  Set the width of the control (in pixels)
     *
     *  \param u16Width_ Width of the control in pixels
     */
    void SetWidth( uint16_t u16Width_ )      { m_u16Width = u16Width_; }

    /*!
     *  \brief SetZOrder
     *
     *  Set the Z-order (depth) of the control
     *
     *  \param u8Z_ Z order of the control
     */
    void SetZOrder( uint8_t u8Z_ )          { m_u8ZOrder = u8Z_; }

    /*!
     *  \brief SetControlIndex
     *
     *  Set the index of the control, used for cycling through focus (ala
     *  tab order in VB).
     *
     *  \param u8Idx_ Focus index of the control
     */
    void SetControlIndex( uint8_t u8Idx_ ) { m_u8ControlIndex = u8Idx_; }

    /*!
     *  \brief GetTop
     *
     *  Return the topmost pixel of the control
     *
     *  \return Topmost pixel of the control
     */
    uint16_t GetTop()             { return m_u16Top; }

    /*!
     *  \brief GetLeft
     *
     *  Return the leftmost pixel of the control
     *
     *  \return Leftmost pixel of the control
     */
    uint16_t GetLeft()             { return m_u16Left; }

    /*!
     *  \brief GetHeight
     *
     *
     *  Get the height of the control in pixels
     *
     *  \return Height of the control in pixels
     */
    uint16_t GetHeight()         { return m_u16Height; }

    /*!
     *  \brief GetWidth
     *
     *  Get the width of the control in pixels
     *
     *  \return Width of the control in pixels
     */
    uint16_t GetWidth()         { return m_u16Width; }

    /*!
     *  \brief GetZOrder
     *
     *  Return the Z-order of the control
     *
     *  \return Z-order of the control
     */
    uint8_t  GetZOrder()        { return m_u8ZOrder; }

    /*!
     *  \brief GetControlIndex
     *
     *  Return the Control Index of the control
     *
     *  \return The control index of the control
     */
    uint8_t  GetControlIndex()     { return m_u8ControlIndex; }

    /*!
     *  \brief IsStale
     *
     *  Return whether or not the control needs to be redrawn or not
     *
     *  \return true - control needs redrawing, false - control is intact.
     */
    bool     IsStale()            { return m_bStale; }

    /*!
     *  \brief GetControlOffset
     *
     *  Return the absolute offset of the control within an event surface.
     *  This function will traverse through all of the object's parents,
     *  and their parents, until the root control and root window are
     *  identified.  The absolute pixel locations of the Topmost (Y)
     *  and Leftmost (X) pixels are popu32ated in the
     *
     *  \param pus16X_ Pointer to the uint16_t containing the leftmost pixel
     *  \param pus16Y_ Pointer to the uint16_t containing the topmost pixel
     */
    void GetControlOffset( uint16_t *pu16X_, uint16_t *pu16Y_ );

    /*!
     *  \brief IsInFocus
     *
     *  Return whether or not the current control has the focus in the window
     *
     *  \return true if this control is in focus, false otherwise
     */
    bool  IsInFocus()
    {
        return m_pclParentWindow->IsInFocus(this);
    }

    /*!
     *  \brief Activate
     *
     *  Activate or deactivate the current control - used when switching
     *  from one active control to another.
     *
     *  \param bActivate_ - true to activate, false to deactivate
     */
    virtual void Activate( bool bActivate_ ) = 0;

protected:
    friend class GuiWindow;
    friend class GuiEventSurface;

    /*!
     *  \brief SetParentControl
     *
     *  Set the parent control of this control.  When a control has its parent
     *  set, it is considered "nested" within that control.  Moving the control
     *  will thus result in all of its child controls to become invalidated,
     *  thus requiring redraws.  The control's object offsets (Top, Bottom,
     *  Height, and Width) also become relative to the origin of the parent
     *  control.
     *
     *  \param pclParent_ Pointer to the control's parent control
     */
    void SetParentControl( GuiControl *pclParent_ ) { m_pclParentControl = pclParent_; }

    /*!
     *  \brief SetParentWindow
     *
     *  Set the parent window of this control.  All controls within the same window
     *  are all associated together, and share events targetted towards a specific
     *  window.  Event tabbing, focus, and Z-ordering is also shared between controls
     *  within a window.
     *
     *  \param pclWindow_ Pointer to the control's parent window.
     */
    void SetParentWindow( GuiWindow *pclWindow_ )    { m_pclParentWindow  = pclWindow_; }

    /*!
     *  \brief GetParentControl
     *
     *  Return the pointer to the control's currently-assigned parent control.
     *
     *  \return Pointer to the Control's currently assigned parent control.
     */
    GuiControl *GetParentControl()                     { return m_pclParentControl; }

    /*!
     *  \brief GetParentWindow
     *
     *  Get the parent window of this control
     *
     *  \return Pointer to the control's window
     */
    GuiWindow *GetParentWindow()                     { return m_pclParentWindow; }

    /*!
     *  \brief ClearStale
     *
     *  Clear the stale flag for this control.  Should only be done after a
     *  redraw has been completed
     *
     */
    void ClearStale()                                 { m_bStale = false; }

    /*!
     *  \brief SetStale
     *
     *  Signal that the object needs to be redrawn.
     */
    void SetStale()                                 { m_bStale = true; }

    /*!
     *  \brief SetAcceptFocus
     *
     *  Tell the control whether or not to accept focus.
     */
    void SetAcceptFocus( bool bFocus_ )             { m_bAcceptsFocus = bFocus_; }

    /*!
     *  \brief AcceptsFocus
     *
     *  Returns whether or not this control accepts focus.
     */
    bool AcceptsFocus()                             { return m_bAcceptsFocus; }
private:
    /*! true if the control is stale and needs to be redrawn, false otherwise */
    bool     m_bStale;

    /*! Whether or not the control accepts focus or not */
    bool   m_bAcceptsFocus;

    /*! The Z-Order (depth) of the control. Only the highest order controls
        are visible at any given location  */
    uint8_t  m_u8ZOrder;

    /*! Index of the control in the window.  This is used for setting focus
        when transitioning from control to control on a window  */
    uint8_t  m_u8ControlIndex;

    /*! Topmost location of the control on the window  */
    uint16_t m_u16Top;

    /*! Leftmost location of the control on the window  */
    uint16_t m_u16Left;

    /*! Width of the control in pixels  */
    uint16_t m_u16Width;

    /*! Height of the control in pixels  */
    uint16_t m_u16Height;

    /*! Pointer to the parent control  */
    GuiControl *m_pclParentControl;

    /*! Pointer to the parent window associated with this control  */
    GuiWindow  *m_pclParentWindow;
};

//---------------------------------------------------------------------------
/*!
 *  Stub control class, used for testing out the GUI framework where certain
 *  controls have not yet been implemented.
 */
class StubControl : public GuiControl
{
public:
    virtual void Init() {  }
    virtual void Draw() {  }
    virtual GuiReturn_t ProcessEvent( GuiEvent_t *pstEvent_ ) { return GUI_EVENT_OK; }
    virtual void Activate( bool bActivate_ ) { }
};

#endif

