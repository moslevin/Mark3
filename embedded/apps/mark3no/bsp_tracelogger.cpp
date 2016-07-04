#include "mark3.h"
#include "tracebuffer.h"

//---------------------------------------------------------------------------
static Thread clLoggerThread;
static uint8_t au8LoggerStack[320];

//---------------------------------------------------------------------------
static Semaphore clLoggerSem;
static volatile bool bPingPong;
static volatile uint16_t u16LogLen;
static volatile uint16_t *pu16Log;

//---------------------------------------------------------------------------
static void LoggerCallback(uint16_t *pu16Data_, uint16_t u16Len_, bool bPingPong_)
{
    CS_ENTER();
    bPingPong = bPingPong_;
    pu16Log = pu16Data_;
    u16LogLen = u16Len_;
    CS_EXIT();

    clLoggerSem.Post();
}

//---------------------------------------------------------------------------
static void LoggerMain(void* unused_)
{
    Driver* pclUART = DriverList::FindByPath("/dev/tty0");

    while (1)
    {
        uint8_t* src;
        uint16_t len;

        clLoggerSem.Pend();

        CS_ENTER();
        src = (uint8_t*)pu16Log;
        len = u16LogLen * sizeof(uint16_t);
        CS_EXIT();

        uint16_t written = 0;
        while (len != written)
        {
            written += pclUART->Write(len - written, src + written);
        }
    }
}

//---------------------------------------------------------------------------
void bsp_tracelogger_init(void)
{
    //-- Init the trace logger thread
    clLoggerThread.Init(  au8LoggerStack,  320,  1, LoggerMain,  0);
    clLoggerThread.Start();

    clLoggerSem.Init(0,1);
    TraceBuffer::SetCallback( LoggerCallback );
}
