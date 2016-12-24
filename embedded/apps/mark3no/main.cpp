#include <avr/io.h>
#include "mark3.h"
#include "drvUART.h"
#include "drvSound.h"
#include "rtc.h"
#include "memutil.h"
#include "led.h"
#include "button.h"
#include "arena.h"
#include "song.h"
#include "tracebuffer.h"
#include "m3shell.h"

#include "bsp_buttons.h"
#include "bsp_heap.h"
#include "bsp_leds.h"
#include "bsp_rtc.h"
#include "bsp_uarts.h"
#include "bsp_sound.h"
extern "C" {
void __cxa_pure_virtual(void)
{
}
}

//---------------------------------------------------------------------------
typedef struct {
    uint32_t u32EpochTicks;
    uint32_t u32ContextSwitches;
} M3TLV_t;

//---------------------------------------------------------------------------
#define MAX_TRACKED_THREADS (16)
static uint16_t u16ThreadBits;
static Thread* apclThreadMap[MAX_TRACKED_THREADS];
static M3TLV_t astTLV[MAX_TRACKED_THREADS];

static volatile uint32_t u32EpochTime = 0;

//---------------------------------------------------------------------------
static Semaphore clRTCSem;
static Mutex     clRTCMutex;
static Thread    clRTCThread;
static uint8_t   au8RTCStack[320];

//---------------------------------------------------------------------------
static M3Shell clShell;
static Thread  clShellThread;
static uint8_t au8ShellStack[320];

//---------------------------------------------------------------------------
static Thread  clBlinkyThread;
static uint8_t au8BlinkyStack[320];

//---------------------------------------------------------------------------
static Thread clIdleThread;
static uint8_t au8IdleStack[320];

//---------------------------------------------------------------------------
static void WriteString(Driver* pclDriver_, const char* szData_)
{
    uint16_t u16Written = 0;
    uint8_t* src        = (uint8_t*)szData_;

    while (*src != '\0') {
        u16Written = pclDriver_->Write(1, src++);

        if (!u16Written) {
            Thread::Sleep(5);
        }
    }
}

//---------------------------------------------------------------------------
static void BlinkyTask(void* param)
{
    while (1) {
        clLED1.Toggle();
        Thread::Sleep(500);
    }
}

//---------------------------------------------------------------------------
static void OnRTCTick(void)
{
    clRTCSem.Post();
}

//---------------------------------------------------------------------------
static void RTCTask(void* param)
{
    bsp_rtc_set_on_rtc_tick(OnRTCTick);
    clRTCSem.Init(0, 1);
    clRTCMutex.Init();

    {
        calendar_t cal = { 0 };
        cal.u8Day      = 24;
        cal.eMonth     = MONTH_DECEMBER;
        cal.u16Year    = 2016;
        cal.u8Hour     = 20;
        cal.u8Minute   = 50;
        cal.u8Second   = 0;

        bsp_rtc_set_datetime(&cal);
    }

    while (1) {
        clRTCSem.Pend();
        bsp_rtc_kick();
    }
}

//---------------------------------------------------------------------------
static void OnThreadCreate(Thread* pclThread_)
{
    CS_ENTER();
    for (uint16_t u16Bit = 0; u16Bit < MAX_TRACKED_THREADS; u16Bit++) {
        if (((1 << u16Bit) & u16ThreadBits) == 0) {
            apclThreadMap[u16Bit] = pclThread_;
            u16ThreadBits |= (1 << u16Bit);

            M3TLV_t* pstTLV = &astTLV[u16Bit];
            pclThread_->SetExtendedContext(pstTLV);
            break;
        }
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
static void OnThreadRemove(Thread* pclThread_)
{
    CS_ENTER();
    for (uint16_t u16Bit = 0; u16Bit < MAX_TRACKED_THREADS; u16Bit++) {
        if (apclThreadMap[u16Bit] == pclThread_) {
            u16ThreadBits &= ~(1 << u16Bit);
            break;
        }
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
static void OnContextSwitch(Thread* pclThread_)
{
    static bool bInit = false;
    static uint32_t u32LastTime = 0;

    uint32_t u32CurrentTime;
    uint16_t u16Ticks32KHz;

    M3TLV_t* pstTLV = (M3TLV_t*)pclThread_->GetExtendedContext();

    CS_ENTER();
    bsp_rtc_get_raw_uptime(&u32CurrentTime, &u16Ticks32KHz);
    u16Ticks32KHz >>= 5; // Divide by 32 ~ 1/1024 seconds
    u32CurrentTime = (u32CurrentTime << 11) | (uint32_t)(u16Ticks32KHz);

    if (!bInit) {
        u32LastTime = u32CurrentTime - 1;
    }
    uint32_t u32TimeDelta = u32CurrentTime - u32LastTime;

    u32EpochTime += u32TimeDelta;
    pstTLV->u32EpochTicks += u32TimeDelta;
    pstTLV->u32ContextSwitches++;
    CS_EXIT();
}

//---------------------------------------------------------------------------
static void ResetEpoch(void)
{
    CS_ENTER();
    for (uint8_t i = 0; i < MAX_TRACKED_THREADS; i++) {
        astTLV[i].u32EpochTicks = 0;
        astTLV[i].u32ContextSwitches = 0;
    }
    u32EpochTime = 0;
    CS_EXIT();
}

//---------------------------------------------------------------------------
static void PrintThreadStatHeader(Driver* pclDriver_)
{    
    WriteString(pclDriver_, "ID\tPrio\tStack\tCtxSw\tCpu%\r\n");
}

//---------------------------------------------------------------------------
static void PrintThreadStats(Thread* pclThread_, Driver* pclDriver_)
{
    char szTmp[16];

    MemUtil::DecimalToString(pclThread_->GetID(), szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, "\t");

    MemUtil::DecimalToString(pclThread_->GetPriority(), szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, "\t");

    MemUtil::DecimalToString(pclThread_->GetStackSlack(), szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, "/");

    MemUtil::DecimalToString(pclThread_->GetStackSize(), szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, "\t");

    M3TLV_t* pstTLV = (M3TLV_t*)pclThread_->GetExtendedContext();

    if (!pstTLV) {
        WriteString(pclDriver_, "\r\n");
        return;
    }

    MemUtil::DecimalToString(pstTLV->u32ContextSwitches, szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, "\t");

    if (!u32EpochTime) {
        u32EpochTime = 1;
    }

    uint32_t u32CPU;
    CS_ENTER();
    u32CPU = ((pstTLV->u32EpochTicks * 1000) + 500) / u32EpochTime;
    CS_EXIT();
    MemUtil::DecimalToString(u32CPU / 10, szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, ".");
    MemUtil::DecimalToString(u32CPU % 10, szTmp);
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, "%\r\n");
}

//---------------------------------------------------------------------------
static void ThreadCommand(const char* szArgs_)
{
    Driver* pclDriver = DriverList::FindByPath("/dev/tty0");
    PrintThreadStatHeader(pclDriver);

    for (uint8_t i = 0; i < MAX_TRACKED_THREADS; i++) {
        if (((1 << i) & u16ThreadBits) != 0) {
            PrintThreadStats(apclThreadMap[i], pclDriver);
        }
    }
}
//---------------------------------------------------------------------------
static void DefaultCommand(const char* szArgs_)
{
    Driver* pclDriver = DriverList::FindByPath("/dev/tty0");
    WriteString(pclDriver, " Invalid command: ");
    WriteString(pclDriver, szArgs_);
    WriteString(pclDriver, "\n\n");
}

//---------------------------------------------------------------------------
static void ResetStats(const char* szArgs_)
{
    ResetEpoch();
}

//---------------------------------------------------------------------------
static void GetTime(const char* szArgs_)
{
    Driver* pclUART = DriverList::FindByPath("/dev/tty0");

    calendar_t myTime = {0};

    bool state = Scheduler::SetScheduler(false);
    bsp_rtc_get_datetime(&myTime);

    const char* szDayOfWeek = bsp_rtc_get_day_of_week();
    const char* szMonth = bsp_rtc_get_month_name();
    Scheduler::SetScheduler(state);

    WriteString(pclUART, "Time: ");

    char szBuf[16];
    MemUtil::DecimalToString(myTime.u8Hour, szBuf);
    if (myTime.u8Hour < 10) {
        WriteString(pclUART, "0");
    }
    WriteString(pclUART, szBuf);
    WriteString(pclUART, ":");
    MemUtil::DecimalToString(myTime.u8Minute, szBuf);
    if (myTime.u8Minute < 10) {
        WriteString(pclUART, "0");
    }
    WriteString(pclUART, szBuf);
    WriteString(pclUART, ":");
    MemUtil::DecimalToString(myTime.u8Second, szBuf);
    if (myTime.u8Second < 10) {
        WriteString(pclUART, "0");
    }
    WriteString(pclUART, szBuf);
    WriteString(pclUART, ", ");

    WriteString(pclUART, szDayOfWeek);
    WriteString(pclUART, ", ");
    WriteString(pclUART, szMonth);
    WriteString(pclUART, " ");

    MemUtil::DecimalToString((uint8_t)myTime.u8Day, szBuf);
    WriteString(pclUART, szBuf);
    WriteString(pclUART, ", ");

    MemUtil::DecimalToString(myTime.u16Year, szBuf);
    WriteString(pclUART, szBuf);
    WriteString(pclUART, ".");

    WriteString(pclUART, "\r\n");
}

//---------------------------------------------------------------------------
static void Uptime(const char* szArgs_)
{
    Driver* pclUART = DriverList::FindByPath("/dev/tty0");

    uint32_t u32Seconds;
    uint32_t u32Ticks;
    bsp_rtc_get_uptime(&u32Seconds, &u32Ticks);

    char szBuf[16];
    MemUtil::DecimalToString(u32Seconds, szBuf);
    WriteString(pclUART, szBuf);
    WriteString(pclUART, "\r\n");
}

//---------------------------------------------------------------------------
static void SetTime(const char* szArgs_)
{
    calendar_t myCalendar = {0};
    char szBuf[6];
    uint8_t u8Len;
    Driver* pclUART = DriverList::FindByPath("/dev/tty0");
    char* szSrc = (char*)szArgs_;
    while (*szSrc == ' ') {
        szSrc++;
    }

    u8Len = MemUtil::StringLength(szSrc);
    if (u8Len != 14) {
        goto on_error;
    }

    szBuf[0] = *szSrc++;
    szBuf[1] = *szSrc++;
    szBuf[2] = 0;

    if (!MemUtil::StringToDecimal8(szBuf, &myCalendar.u8Day)) {
        goto on_error;
    }

    szBuf[0] = *szSrc++;
    szBuf[1] = *szSrc++;
    szBuf[2] = 0;
    uint8_t u8Tmp;
    if (!MemUtil::StringToDecimal8(szBuf, &u8Tmp)) {
        goto on_error;
    }
    myCalendar.eMonth = (month_t)(u8Tmp - 1);

    szBuf[0] = *szSrc++;
    szBuf[1] = *szSrc++;
    szBuf[2] = 0;
    if (!MemUtil::StringToDecimal8(szBuf, &myCalendar.u8Hour)) {
        goto on_error;
    }

    szBuf[0] = *szSrc++;
    szBuf[1] = *szSrc++;
    szBuf[2] = 0;
    if (!MemUtil::StringToDecimal8(szBuf, &myCalendar.u8Minute)) {
        goto on_error;
    }

    szBuf[0] = *szSrc++;
    szBuf[1] = *szSrc++;
    szBuf[2] = 0;
    if (!MemUtil::StringToDecimal8(szBuf, &myCalendar.u8Second)) {
        goto on_error;
    }

    szBuf[0] = *szSrc++;
    szBuf[1] = *szSrc++;
    szBuf[2] = *szSrc++;
    szBuf[3] = *szSrc;
    szBuf[4] = 0;

    if (!MemUtil::StringToDecimal16(szBuf, &myCalendar.u16Year)) {
        goto on_error;
    }

    u8Tmp = bsp_rtc_set_datetime(&myCalendar);

    if (u8Tmp != 0) {
        goto on_error;
    }

    GetTime(0);
    return;

on_error:

    WriteString(pclUART, "Invalid, expected: DDMMHHmmssCCYY\r\n");
    return;
}

//---------------------------------------------------------------------------
static void HexPrint(const uint8_t* pu8Src_, uint8_t u8Len_, K_ADDR kaOffset_, Driver* pclDriver_)
{
    char szTmp[8];
    MemUtil::DecimalToHex(kaOffset_, szTmp);

    // Print address
    uint8_t u8ZeroPad = 4 - MemUtil::StringLength(szTmp);
    for (uint8_t i = 0; i < u8ZeroPad; i++) {
        WriteString(pclDriver_, "0");
    }
    WriteString(pclDriver_, szTmp);
    WriteString(pclDriver_, " | ");

    // Print ascii
    szTmp[1] = 0;
    for (uint8_t i = 0; i < u8Len_; i++) {
        if (pu8Src_[i] >= 32) {
            szTmp[0] = pu8Src_[i];
        } else {
            szTmp[0] = '.';
        }
        WriteString(pclDriver_, (const char*)&pu8Src_[i]);
    }
    for (uint8_t i = 16; i > u8Len_; i--) {
        WriteString(pclDriver_, " ");
    }

    // Print hex bytes
    for (uint8_t i = 0; i < u8Len_; i++) {
        MemUtil::DecimalToHex(pu8Src_[i], szTmp);
        uint8_t u8ZeroPad = 2 - MemUtil::StringLength(szTmp);
        for (uint8_t j = 0; j < u8ZeroPad; j++) {
            WriteString(pclDriver_, "0");
        }
        WriteString(pclDriver_, szTmp);
        WriteString(pclDriver_, " ");
    }
    WriteString(pclDriver_, "\r\n");
}

//---------------------------------------------------------------------------
static void ReadRam(const char* szArgs)
{
    Driver* pclUART = DriverList::FindByPath("/dev/tty0");

    Token_t  astTokens[3];
    uint8_t u8Tokens = MemUtil::Tokenize(szArgs, astTokens, 3);

    uint8_t u8Tmp[16];
    K_ADDR kaAddr;
    K_ADDR kaLen;

    if (u8Tokens < 2) {
        goto on_error;
    }

    if (!MemUtil::StringToDecimal16(astTokens[0].pcToken, &kaAddr)) {
        goto on_error;
    }
    if (!MemUtil::StringToDecimal16(astTokens[0].pcToken, &kaLen)) {
        goto on_error;
    }

    for (K_ADDR i = kaAddr; i < kaAddr + kaLen; i+= 16 ) {
        uint8_t* pu8Src = (uint8_t*)kaAddr;
        uint8_t u8ToCopy = 16;
        if ((i + 16) > (kaAddr + kaLen)) {
            u8ToCopy = (kaAddr + kaLen) - i;
        }
        MemUtil::CopyMemory(u8Tmp, pu8Src, u8ToCopy);
        HexPrint(pu8Src, u8ToCopy, i, pclUART);
    }

on_error:
    WriteString(pclUART, "Invalid, expected: ADDR LEN\r\n");
}

static Song_t stJingle;
static volatile bool bSongOn = false;

//---------------------------------------------------------------------------
static void SoundOnCommand(const char* szArgs)
{
    if (bSongOn) {
        return;
    }

    stJingle.astNotes = (SongNote_t*)g_astJingleNotes;
    stJingle.u8TempoBPM = 192;

    clSound.Control(SOUND_EVENT_SONG, &stJingle, 0, 0, 0);
    bSongOn = true;
}

//---------------------------------------------------------------------------
static void SoundOffCommand(const char* szArgs)
{
    if (!bSongOn) {
        return;
    }


    clSound.Control(SOUND_EVENT_OFF, 0, 0, 0, 0);
    bSongOn = false;
}

//---------------------------------------------------------------------------
static CommandHandler clCommandDefault;
static CommandHandler clCommandThreads;
static CommandHandler clCommandRStats;
static CommandHandler clCommandGetTime;
static CommandHandler clCommandUptime;
static CommandHandler clCommandSetTime;
static CommandHandler clCommandReadRam;
static CommandHandler clCommandSoundOn;
static CommandHandler clCommandSoundOff;

//---------------------------------------------------------------------------
static void ShellTask(void* param)
{
    Driver* pclUART = DriverList::FindByPath("/dev/tty0");
    pclUART->Control(CMD_UARTPLUS_SET_BLOCKING, 0, 0, 0, 0 );

    clCommandThreads.Set("threads", ThreadCommand);
    clCommandDefault.Set("default", DefaultCommand);
    clCommandRStats.Set("rstats", ResetStats);
    clCommandGetTime.Set("gettime", GetTime);
    clCommandUptime.Set("uptime", Uptime);
    clCommandSetTime.Set("settime", SetTime);
    clCommandReadRam.Set("readram", ReadRam);
    clCommandSoundOn.Set("sndon", SoundOnCommand);
    clCommandSoundOff.Set("sndoff", SoundOffCommand);

    clShell.AddCommand(&clCommandDefault);
    clShell.AddCommand(&clCommandThreads);
    clShell.AddCommand(&clCommandRStats);
    clShell.AddCommand(&clCommandGetTime);
    clShell.AddCommand(&clCommandUptime);
    clShell.AddCommand(&clCommandSetTime);
    clShell.AddCommand(&clCommandReadRam);
    clShell.AddCommand(&clCommandSoundOn);
    clShell.AddCommand(&clCommandSoundOff);

    clShell.Init();
    clShell.SetDriver(pclUART);
    clShell.SetPrompt("Mark3 $ ");
    clShell.SetDefaultHandler(&clCommandDefault);

    while(1) {
        clShell.InputLoop();
    }
}
//---------------------------------------------------------------------------
static void IdleTask(void* param)
{
    volatile uint32_t u32Counter = 0;
    while (1) {
        /* DO NOTHING */
        u32Counter++;
    }
}

//---------------------------------------------------------------------------
static void Button1Down(Button* pclButton_)
{

}
//---------------------------------------------------------------------------
static void Button1Up(Button* pclButton_)
{
    SoundOnCommand("");
}

//---------------------------------------------------------------------------
static void Button2Down(Button* pclButton_)
{

}

//---------------------------------------------------------------------------
static void Button2Up(Button* pclButton_)
{
    SoundOffCommand("");
}


//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    Kernel::SetThreadContextSwitchCallout(OnContextSwitch);
    Kernel::SetThreadCreateCallout(OnThreadCreate);
    Kernel::SetThreadExitCallout(OnThreadRemove);

    u16ThreadBits = 0;
    for (int i = 0; i < MAX_TRACKED_THREADS; i++) {
        apclThreadMap[i] = 0;
    }

    clRTCThread.Init(au8RTCStack, sizeof(au8RTCStack)/sizeof(K_WORD), 7, RTCTask, 0);
    clRTCThread.Start();

    clBlinkyThread.Init(au8BlinkyStack, sizeof(au8BlinkyStack)/sizeof(K_WORD), 6, BlinkyTask, 0);
    clBlinkyThread.Start();

    clShellThread.Init(au8ShellStack, sizeof(au8ShellStack)/sizeof(K_WORD), 5, ShellTask, 0);
    clShellThread.Start();

    clIdleThread.Init(au8IdleStack, sizeof(au8IdleStack)/sizeof(K_WORD), 0, IdleTask, 0);
    clIdleThread.Start();

    bsp_leds_init();
    bsp_uarts_init();
    bsp_heap_init();
    bsp_rtc_init();
    bsp_sound_init();

    bsp_buttons_init();

    bsp_buttons_set_callbacks(&clButtons[0], Button1Down, Button1Up);
    bsp_buttons_set_callbacks(&clButtons[1], Button2Down, Button2Up);

    Kernel::Start();
}
