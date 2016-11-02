/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3.h"
#include "drvUART.h"
#include "terminal.h"
#include "terminal_input.h"
#include "memutil.h"

extern "C" {
void __cxa_pure_virtual(void)
{
}
}

typedef void(*command_action)(const char* szArgs_);

class CommandHandler : public LinkListNode
{
public:
    CommandHandler() {}

    CommandHandler(const char* szName_, command_action pfHandler_)
        { Set(szName_, pfHandler_); }

    void Set(const char* szName_, command_action pfHandler_)
        { m_szCommandName = szName_; m_pfHandler = pfHandler_; }

    const char* Name()
        { return m_szCommandName; }

    void Execute(const char* szArgs_)
        { m_pfHandler(szArgs_); }

private:
    const char*         m_szCommandName;
    command_action      m_pfHandler;
};

class CommandDispatcher
{
public:
    void AddCommand(CommandHandler* pclHandler_)
    {
        m_clCommandList.Add(pclHandler_);
    }

    bool Execute(const char* szCommandString_)
    {
        int iCommandLength = CommandLength(szCommandString_);
        const char* szArgs = (szCommandString_ + iCommandLength);
        if (!iCommandLength) {
            return true;
        }

        CommandHandler* pclCurr = static_cast<CommandHandler*>(m_clCommandList.GetHead());
        while (pclCurr) {
            const char* szCommandName = pclCurr->Name();
            int iHandlerLength = MemUtil::StringLength(szCommandName);
            if ((iCommandLength >= iHandlerLength) &&
                ((szCommandString_[iHandlerLength] == ' ') ||(szCommandString_[iHandlerLength] == '\0')) &&
                (MemUtil::CompareStrings(szCommandString_, szCommandName, iHandlerLength))) {
                pclCurr->Execute(szArgs);
                return true;
            }
            pclCurr = static_cast<CommandHandler*>(pclCurr->GetNext());
        }
        m_pclDefault->Execute(szCommandString_);
        return false;
    }

    void SetDefaultHandler(CommandHandler* pclHandler_) { m_pclDefault = pclHandler_; }

private:
    int CommandLength(const char* szCommandString_)
    {
        int iRc = 0;
        while ((szCommandString_[iRc] != '\0') &&
               (szCommandString_[iRc] != ' ')) {
            iRc++;
        }
        return iRc;
    }

    DoubleLinkList  m_clCommandList;
    CommandHandler*  m_pclDefault;
};


#define UART_SIZE_TX 32
#define UART_SIZE_RX 32

//---------------------------------------------------------------------------
// This block declares the thread data for the main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP_STACK_SIZE (320 / sizeof(K_WORD))
static Thread clAppThread;
static K_WORD awAppStack[APP_STACK_SIZE];
static void AppMain(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for the idle thread.  It defines a
// thread object, stack (in word-array form), and the entry-point function
// used by the idle thread.
#define IDLE_STACK_SIZE (320 / sizeof(K_WORD))
static Thread clIdleThread;
static K_WORD awIdleStack[IDLE_STACK_SIZE];
static void IdleMain(void* unused_);

//---------------------------------------------------------------------------
static uint8_t    aucTxBuffer[UART_SIZE_TX];
static uint8_t    aucRxBuffer[UART_SIZE_RX];
static ATMegaUART clUART; //!< UART device driver object

static const char* prompt = "Mark3$: ";
static CommandDispatcher clDispatcher;
static CommandHandler clActionHandler;
static CommandHandler clDefaultHandler;


const void* StringWrite(const char* szStr_)
{
    uint8_t*    src       = (uint8_t*)szStr_;
    uint16_t    u16Remain = MemUtil::StringLength(szStr_);

    while (u16Remain) {
        uint16_t u16Written = clUART.Write(u16Remain, src);
        src += u16Written;
        u16Remain -= u16Written;
    }
}

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    clAppThread.Init(awAppStack, sizeof(awAppStack), 1, AppMain, 0);
    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, 0);

    clAppThread.Start();
    clIdleThread.Start();

    clUART.SetName("/dev/tty"); //!< Add the serial driver
    clUART.Init();
    DriverList::Add(&clUART);

    Kernel::Start();

    return 0;
}

void ActionHandler(const char* args)
{
    StringWrite("Hello World!: ");
    StringWrite(args);
}

void DefaultHandler(const char* args)
{
    StringWrite("Unrecognized command!: ");
    StringWrite(args);
}

void AppMain(void* unused_)
{
    clDefaultHandler.Set("default", DefaultHandler);
    clActionHandler.Set("action", ActionHandler);

    clDispatcher.AddCommand(&clDefaultHandler);
    clDispatcher.AddCommand(&clActionHandler);

    clDispatcher.SetDefaultHandler(&clDefaultHandler);

    Driver* my_uart = DriverList::FindByPath("/dev/tty");

    uint32_t new_baud = 57600;
    my_uart->Control(CMD_SET_BAUDRATE, &new_baud, 0,0,0);

    my_uart->Control(CMD_SET_BUFFERS, aucRxBuffer, UART_SIZE_RX, aucTxBuffer, UART_SIZE_TX);

    my_uart->Open();

    Terminal term;
    term.SetDriver(my_uart);
    term.ClearScreen();

    TerminalInput term_input;
    term_input.Init();

    while (1) {

        // Output the prompt...
        term.SetForeColor(TERMINAL_COLOR_GREEN);
        term.SetBackColor(TERMINAL_COLOR_BLACK);
        term.Bold();
        StringWrite(prompt);
        term.ResetAttributes();

        // Terminal Input color is white...
        term.SetForeColor(TERMINAL_COLOR_WHITE);

        static char szLineBuf[80];

        bool done = false;
        MemUtil::SetMemory(szLineBuf, 0, sizeof(szLineBuf));
        uint8_t u8BufIdx = 0;

        while (!done) {

            char ch;
            uint16_t u16Read = my_uart->Read(1, (uint8_t*)&ch);

            if (u16Read) {
                if (term_input.ParseCharacter(ch)) {
                    KeyVal_t key;
                    term_input.GetLastKey(&key);
                    if (key.bEscaped) {
                        if (key.eKey == TERMINAL_KEY_CARRIAGE_RETURN) {
                            done = true;
                            StringWrite("\r\n");
                        } else if (key.eKey == TERMINAL_KEY_BACKSPACE) {
                            if (u8BufIdx > 0) {
                                szLineBuf[u8BufIdx] = 0;
                                u8BufIdx--;
                                term.Backspace();
                            }
                        }
                    } else {
                        if (u8BufIdx < (sizeof(szLineBuf) - 1)) {
                            my_uart->Write(1, (uint8_t*)&(key.cChar));
                            szLineBuf[u8BufIdx] = key.cChar;
                            u8BufIdx++;
                        }
                    }
                }
            }
        }
        if (u8BufIdx > 0) {
            clDispatcher.Execute(szLineBuf);
            StringWrite("\r\n");
        }
    }
}

//---------------------------------------------------------------------------
void IdleMain(void* unused_)
{
    while (1) {
        // Low priority task + power management routines go here.
        // The actions taken in this context must *not* cause the thread
        // to block, as the kernel requires that at least one thread is
        // schedulable at all times when not using an idle thread.

        // Note that if you have no special power-management code or idle
        // tasks, an empty while(1){} loop is sufficient to guarantee that
        // condition.
    }
}
