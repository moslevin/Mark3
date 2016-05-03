#include "mark3.h"
#include "gui.h"
#include "screen.h"
#include "arial_10.h"
#include "control_label.h"
#include "control_panel.h"
#include "control_checkbox.h"
#include "control_slickbutton.h"
#include "control_button.h"
#include "control_sevenseg.h"
#include "control_brushpanel.h"
#include "control_groupbox.h"
#include "control_progress.h"

#include "graphics_flavr.h"
#include "system_heap.h"
#include "fixed_heap.h"
#include "drvFlavrJoy.h"
#include "rand_lfsr.h"
#include "gui_palette.h"
//---------------------------------------------------------------------------
// Graphics driver used in our application
static GraphicsFlavr    clGfx;

//---------------------------------------------------------------------------
// Window management objects and GUI components.
static GuiEventSurface  clEventSurface;
static GuiWindow        clWindow;
static LabelControl     clLabel;
static BrushPanelControl clPanel;
static CheckBoxControl  clCheckBox1;
static CheckBoxControl  clCheckBox2;
static ButtonControl    clSlickButton;
static SevenSegControl  clSevenSeg;
static GroupBoxControl  clGroupBox;
static ProgressControl  clProgress;
//---------------------------------------------------------------------------
// Joystick driver and associated timer object, used to poll the joystick at
// a fixed frequency
static FlavrJoystick    clJoystick;
static Timer            clJoyTimer;

//---------------------------------------------------------------------------
// Callback function used to poll the joystick and pass the event back to the
// GUI thread.
static void JoyTimerCallback( Thread *pclOwner_, void *pvData_ );

//---------------------------------------------------------------------------
// Tick timer, used to drive animated display elements
static Timer            clTickTimer;
static volatile bool    bFlip = false;
//---------------------------------------------------------------------------
static void TickTimerCallback( Thread *pclOwner_, void *pvData_ );

//---------------------------------------------------------------------------
// Checkbox callback declarations
static uint8_t u8Progress16Val = 50;
static void CheckBox1Callback( bool bChecked_ );
static void CheckBox2Callback( bool bChecked_ );

//---------------------------------------------------------------------------
// GUI Application thread + thread stack
#define APP_STACK_SIZE      (320)
Thread clAppThread;
K_WORD awAppStack[APP_STACK_SIZE];

//---------------------------------------------------------------------------
static void AppMain(void *unused_);

extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    clAppThread.Init(awAppStack,APP_STACK_SIZE,1,AppMain,0);
    clAppThread.Start();

    Kernel::Start();
}

//---------------------------------------------------------------------------
void JoyTimerCallback( Thread *pclOwner_, void *pvData_ )
{
    // Read the joystick, and send the event to the "event surface" that
    // manages events for our application window.
    GuiEvent_t stEvent;
    stEvent.u8EventType = EVENT_TYPE_JOYSTICK;
    stEvent.u8TargetID = TARGET_ID_FOCUS;

    JoystickReport stReport;
    clJoystick.Scan();

    clJoystick.ReadPrevious(&stReport);
    stEvent.stJoystick.Previous.u16RawData = stReport.u16Digital;

    clJoystick.ReadCurrent(&stReport);
    stEvent.stJoystick.Current.u16RawData = stReport.u16Digital;

    clEventSurface.SendEvent(&stEvent);
}

static volatile bool bTimerRunning = false;

//---------------------------------------------------------------------------
void TickTimerCallback( Thread *pclOwner_, void *pvData_ )
{
    // If the stopwatch timer is running (i.e. user pressed the button)
    // then add time to the display + trigger a redraw.
    if (bTimerRunning)
    {
        clSevenSeg.SetValue( clSevenSeg.GetValue() + 3 );
        if ((clSevenSeg.GetValue() % 100) < 50)
        {
            clSevenSeg.SetColon(false);
        }
        else
        {
            clSevenSeg.SetColon(true);
        }
    }

    GuiEvent_t stEvent;
    stEvent.u8EventType = EVENT_TYPE_TIMER;
    stEvent.u8TargetID = TARGET_ID_BROADCAST;
    stEvent.stTimer.u16Ticks = 1;
    bFlip = true;
    clEventSurface.SendEvent(&stEvent);

}

//---------------------------------------------------------------------------
static void MyButtonCallback( void *pvData_ )
{
    // Toggle the stopwatch timer on/off.
    if (!bTimerRunning)
    {
        KernelAware::Print("StopWatchON!\n");
        bTimerRunning = true;
    }
    else
    {
        KernelAware::Print("StopWatchOFF!\n");
        bTimerRunning = false;
    }
}

//---------------------------------------------------------------------------
static void CheckBox1Callback( bool bChecked_ )
{
    if (bChecked_)
    {
        KernelAware::Print("CB1_Chk\n");
        u8Progress16Val += 50;
    }
    else
    {
        KernelAware::Print("CB1_NCh\n");
        u8Progress16Val -=50;
    }
    clProgress.SetProgress(u8Progress16Val);
}

//---------------------------------------------------------------------------
static void CheckBox2Callback( bool bChecked_ )
{
    if (bChecked_)
    {
        KernelAware::Print("CB2_Chk\n");
        u8Progress16Val += 25;
    }
    else
    {
        KernelAware::Print("CB2_NCh\n");
        u8Progress16Val -= 25;
    }
    clProgress.SetProgress(u8Progress16Val);
}

//---------------------------------------------------------------------------
void AppMain(void *unused_)
{
    // -- Setup the system heap (it's needed by the GUI lib)
    SystemHeap::Init();

    PseudoRandom clRand;
    clRand.Seed(1,2);
    clRand.GetRandom();

    // -- Setup a timer to poll the joystick for input regularly
    clJoystick.SetName("/dev/joy");
    clJoystick.Init();
    clJoystick.Open();

    clJoyTimer.Init();
    clJoyTimer.Start(true, 66, JoyTimerCallback, 0);

    // -- Setup a system-tick timer for driving animated controls
    clTickTimer.Init();
    clTickTimer.Start(true, 33, TickTimerCallback, 0);

    // -- Graphics init
    clGfx.SetName("/dev/gfx");
    clGfx.Init();

    {
        DrawWindow_t stWindow;
        stWindow.u16Top = 0;
        stWindow.u16Bottom = 159;
        stWindow.u16Left = 0;
        stWindow.u16Right = 127;
        clGfx.SetWindow(&stWindow);
    }

    clGfx.Open();
    clGfx.ClearScreen();

    // -- Initialize our GUI components.
    clLabel.Init();
    clLabel.SetLeft(10);
    clLabel.SetTop(10);
    clLabel.SetFont(&fntArial_10_False_False_False_);
    clLabel.SetCaption("Mark3   Rocks!");
    clLabel.SetBackColor(COLOR_BLACK);
    clLabel.SetFontColor(COLOR_WHITE);
    clLabel.SetWidth(100);
    clLabel.SetHeight(16);
    clLabel.SetZOrder(2);
    clLabel.SetControlIndex(4);
    clLabel.SetTransparent(true);

    clCheckBox1.Init();
    clCheckBox1.SetLeft(2);
    clCheckBox1.SetTop(16);
    clCheckBox1.SetWidth(60);
    clCheckBox1.SetHeight(16);
    clCheckBox1.SetCaption("Check1");
    clCheckBox1.SetCheck(true);
    clCheckBox1.SetFont(&fntArial_10_False_False_False_);
    clCheckBox1.SetZOrder(3);
    clCheckBox1.SetControlIndex(3);
    clCheckBox1.SetBackColor(SECONDARY_2_0);
    clCheckBox1.SetCheckCallback(CheckBox1Callback);


    clCheckBox2.Init();
    clCheckBox2.SetLeft(2);
    clCheckBox2.SetTop(32);
    clCheckBox2.SetWidth(60);
    clCheckBox2.SetHeight(16);
    clCheckBox2.SetCaption("Check2");
    clCheckBox2.SetCheck(false);
    clCheckBox2.SetFont(&fntArial_10_False_False_False_);
    clCheckBox2.SetZOrder(3);
    clCheckBox2.SetControlIndex(2);
    clCheckBox2.SetBackColor(SECONDARY_2_0);
    clCheckBox2.SetCheckCallback(CheckBox2Callback);

    clProgress.Init();
    clProgress.SetProgress(50);
    clProgress.SetTop(50);
    clProgress.SetLeft(10);
    clProgress.SetHeight(10);
    clProgress.SetWidth(90);
    clProgress.SetZOrder(3);
    clProgress.SetControlIndex(7);

    clSlickButton.Init();
    clSlickButton.SetLeft(16);
    clSlickButton.SetTop(110);
    clSlickButton.SetWidth(96);
    clSlickButton.SetHeight(16);
    clSlickButton.SetCallback(MyButtonCallback, 0);
    clSlickButton.SetFont(&fntArial_10_False_False_False_);
    clSlickButton.SetZOrder(2);
    clSlickButton.SetControlIndex(1);
    clSlickButton.SetCaption("Start/Stop");

    clSevenSeg.Init();
    clSevenSeg.SetLeft(16);
    clSevenSeg.SetWidth(96) ;
    clSevenSeg.SetTop(128);
    clSevenSeg.SetHeight(30);
    clSevenSeg.SetColon(true);
    clSevenSeg.SetValue(0);
    clSevenSeg.SetControlIndex(6);
    clSevenSeg.SetZOrder(2);


    clPanel.Init();
    clPanel.SetLeft(0);
    clPanel.SetTop(0);
    clPanel.SetWidth(128);
    clPanel.SetHeight(160);
    clPanel.SetZOrder(1);
    clPanel.SetControlIndex(5);

    clGroupBox.Init();
    clGroupBox.SetLeft(10);
    clGroupBox.SetTop(32);
    clGroupBox.SetWidth(110);
    clGroupBox.SetHeight(64);
    clGroupBox.SetFont(&fntArial_10_False_False_False_);
    clGroupBox.SetZOrder(2);
    clGroupBox.SetCaption("GroupBox");
    clGroupBox.SetControlIndex(6);

    // -- Add GUI components to the window
    clWindow.Init();
    clWindow.SetDriver(&clGfx);

    clWindow.SetHeight(160);
    clWindow.SetWidth(128);
    clWindow.SetTop(0);
    clWindow.SetLeft(0);
    clWindow.SetZOrder(1);

    clWindow.AddControl(&clPanel, 0);

    clWindow.AddControl(&clLabel, &clPanel);
    clWindow.AddControl(&clSevenSeg, &clPanel);
    clWindow.AddControl(&clSlickButton, &clPanel);
    clWindow.AddControl(&clGroupBox, &clPanel);

    clWindow.AddControl(&clCheckBox1, &clGroupBox);
    clWindow.AddControl(&clCheckBox2, &clGroupBox);
    clWindow.AddControl(&clProgress, &clGroupBox);

    // -- Initialize the "event surface", which is the
    // object that ties everything together.
    clEventSurface.Init();

    // Use the global system heap and message pool for dynamic allocations
    // on this event surface.
    clEventSurface.SetHeap(SystemHeap::GetHeap());
    clEventSurface.SetMessagePool(GlobalMessagePool::GetPool());

    clEventSurface.AddWindow(&clWindow);

    // -- Draw the initial screen state
    clWindow.Redraw(true);
    clGfx.Flip();

    // -- This becomes our main event loop for the application.
    while(1)
    {
        if (!clEventSurface.ProcessEvent())
        {
            KernelAware::Print("Error processing event\n");
        }

        if (bFlip == true)
        {
            clWindow.Redraw(false);
            clGfx.Flip();
            bFlip = false;
        }
    }
}
