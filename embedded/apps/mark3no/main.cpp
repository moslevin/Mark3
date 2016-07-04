/*
 * Mark3no_Bringup.cpp
 *
 * Created: 6/12/2016 7:56:41 PM
 * Author : moslevin
 */ 

#include <avr/io.h>
#include "mark3.h"
#include "drvUART.h"
#include "rtc.h"
#include "memutil.h"
#include "led.h"
#include "button.h"
#include "arena.h"
#include "tracebuffer.h"

#include "bsp_buttons.h"
#include "bsp_heap.h"
#include "bsp_leds.h"
#include "bsp_rtc.h"
#include "bsp_tracelogger.h"
#include "bsp_uarts.h"

static Thread clApp1;
static uint8_t au8Stack1[320];

static Thread clApp2;
static uint8_t au8Stack2[320];

static Thread clApp3;
static uint8_t au8Stack3[320];

static Thread clApp4;
static uint8_t au8Stack4[320];

static Thread clApp5;
static uint8_t au8Stack5[320];

static void App1(void* param)
{
	while(1) {
        clLED1.Toggle();
		Thread::Sleep(100);
	}
}

static void App2(void* param)
{
	while(1) {
        clLED2.Toggle();
		Thread::Sleep(250);
	}
}

static void WriteString(Driver* pclDriver_, const char* szData_)
{
    uint16_t u16Written = 0;
    uint8_t* src = (uint8_t*)szData_;

    while (*src != '\0')
    {
        u16Written = pclDriver_->Write(1, src++);

        if (!u16Written)
        {
            Thread::Sleep(5);
        }
    }
}

static Semaphore clRTCSem;
static void OnRTCTick(void)
{
    clRTCSem.Post();
}

static void App3(void* param)
{
    bsp_rtc_set_on_rtc_tick(OnRTCTick);
    clRTCSem.Init(0,1);

    {        
        calendar_t cal = {0};
        cal.u8Day = 27;
        cal.eMonth = MONTH_JUNE;
        cal.u16Year = 2016;
        cal.u8Hour = 20;
        cal.u8Minute = 50;
        cal.u8Second = 0;

        bsp_rtc_set_datetime(&cal);
    }

    Driver* pclUART = DriverList::FindByPath("/dev/tty0");

	while(1) {
        clLED3.Toggle();

        calendar_t myCal;

        clRTCSem.Pend();

        bsp_rtc_kick();
        bsp_rtc_get_datetime(&myCal);
        char szBuf[12];

        MemUtil::DecimalToString(myCal.u8Hour, szBuf);
        WriteString(pclUART, szBuf);
        WriteString(pclUART, ":");

        MemUtil::DecimalToString(myCal.u8Minute, szBuf);
        WriteString(pclUART, szBuf);
        WriteString(pclUART, ":");

        MemUtil::DecimalToString(myCal.u8Second, szBuf);
        WriteString(pclUART, szBuf);
        WriteString(pclUART, " - ");

        const char* szDay   = bsp_rtc_get_day_of_week();
        WriteString(pclUART, szDay);
        WriteString(pclUART, ", ");

        const char* szMonth = bsp_rtc_get_month_name();
        WriteString(pclUART, szMonth);
        WriteString(pclUART, " ");

        MemUtil::DecimalToString(myCal.u8Day, szBuf);
        WriteString(pclUART, szBuf);
        WriteString(pclUART, ", ");

        MemUtil::DecimalToString(myCal.u16Year, szBuf);
        WriteString(pclUART, szBuf);
        WriteString(pclUART, "\n");
	}
}

static void App4(void* param)
{
	Driver* pclUART = DriverList::FindByPath("/dev/tty0");
    bool bState;

	while(1) {
		const char* szStr = "Tommy can you hear me?\r\n"; // 24 chars?
        WriteString(pclUART, szStr);
        Thread::Sleep(100);

        bState = clButtons[0].ReadState();
        if (bState) {
            WriteString(pclUART, "B1 UP\n");
        } else {
            WriteString(pclUART, "B1 DOWN\n");
        }

        bState = clButtons[1].ReadState();
        if (bState) {
            WriteString(pclUART, "B2 UP\n");
        } else {
            WriteString(pclUART, "B2 DOWN\n");
        }
        Thread::Sleep(300);
	}
}

static void App5(void* param)
{
	Driver* pclUART = DriverList::FindByPath("/dev/tty1");
	
	while(1) {
		const char* szStr = "Ground control to major Tom\r\n"; // 29 chars?
        WriteString(pclUART, szStr);
		Thread::Sleep(750);
	}
}

int main(void)
{
	Kernel::Init();
	
	clApp1.Init(au8Stack1, 320, 1, App1, 0);	
	clApp2.Init(au8Stack2, 320, 1, App2, 0);
	clApp3.Init(au8Stack3, 320, 1, App3, 0);
	clApp4.Init(au8Stack4, 320, 1, App4, 0);
	clApp5.Init(au8Stack5, 320, 1, App5, 0);
	
	clApp1.Start();
	clApp2.Start();
	clApp3.Start();
	clApp4.Start();
	clApp5.Start();

    bsp_leds_init();
    bsp_buttons_init();
    bsp_uarts_init();
    bsp_heap_init();
    bsp_rtc_init();
    bsp_tracelogger_init();

    Kernel::Start();
	
}

