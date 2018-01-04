/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------
#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "mark3.h"
#include "memutil.h"

namespace {
using namespace Mark3;
//===========================================================================
// Local Defines
//===========================================================================
Thread clMBoxThread;
K_WORD akMBoxStack[PORT_KERNEL_DEFAULT_STACK_SIZE];

#define MBOX_BUFFER_SIZE (128)
Mailbox clMbox;
uint8_t aucMBoxBuffer[MBOX_BUFFER_SIZE];

volatile uint8_t aucTxBuf[17] = "abcdefghijklmnop"; // allocate a byte of slack for null-termination
volatile uint8_t aucRxBuf[16];
volatile bool    exit_flag;
} // anonymous namespace

//===========================================================================
// Define Test Cases Here
//===========================================================================
namespace Mark3 {
TEST(mailbox_blocking_receive)
{
    auto lMboxText = [](void* /*unused_*/) {
        while (1) {
            clMbox.Receive((void*)aucRxBuf);
        }
    };
    clMbox.Init((void*)aucMBoxBuffer, sizeof(aucMBoxBuffer), 16);
    clMBoxThread.Init(akMBoxStack, sizeof(akMBoxStack), 7, lMboxText, 0);
    clMBoxThread.Start();

    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(clMbox.Send((void*)aucTxBuf));
        EXPECT_TRUE(MemUtil::CompareMemory((void*)aucRxBuf, (void*)aucTxBuf, 16));
        for (int j = 0; j < 16; j++) {
            aucTxBuf[j]++;
        }
    }
    clMBoxThread.Exit();
}
TEST_END

volatile uint16_t u16Timeouts = 0;
void mbox_timed_test(void* param)
{
    u16Timeouts = 0;
    exit_flag   = false;
    while (!exit_flag) {
        if (!clMbox.Receive((void*)aucRxBuf, 10)) {
            u16Timeouts++;
        }
    }
    clMBoxThread.Exit();
}

TEST(mailbox_blocking_timed)
{
    u16Timeouts = 0;
    clMbox.Init((void*)aucMBoxBuffer, MBOX_BUFFER_SIZE, 16);
    clMBoxThread.Init(akMBoxStack, sizeof(akMBoxStack), 7, mbox_timed_test, (void*)&u16Timeouts);
    clMBoxThread.Start();

    for (int j = 0; j < 16; j++) {
        aucTxBuf[j] = 'x';
    }

    //!! Note -- give extra slack here, as the kernel only specifies a *minimum* timing
    //!! guarantee when a sleep occurs.  Using ms-accuracy timing will make these types
    //!! of failures more prevalent.
    Thread::Sleep(115);
    EXPECT_GTE(u16Timeouts, 10);

    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(clMbox.Send((void*)aucTxBuf));
        EXPECT_TRUE(MemUtil::CompareMemory((void*)aucRxBuf, (void*)aucTxBuf, 16));
        for (int j = 0; j < 16; j++) {
            aucTxBuf[j]++;
        }
        Thread::Sleep(5);
    }
    exit_flag = true;
    Thread::Sleep(100);
}
TEST_END

TEST(mailbox_send_recv)
{
    clMbox.Init((void*)aucMBoxBuffer, MBOX_BUFFER_SIZE, 16);

    for (int i = 0; i < 8; i++) {
        EXPECT_TRUE(clMbox.Send((void*)aucTxBuf));
        for (int j = 0; j < 16; j++) {
            aucTxBuf[j]++;
        }
    }
    EXPECT_FALSE(clMbox.Send((void*)aucTxBuf));

    for (int i = 0; i < 8; i++) {
        clMbox.Receive((void*)aucRxBuf);
        for (int j = 0; j < 16; j++) {
            aucTxBuf[j]--;
        }
        EXPECT_TRUE(MemUtil::CompareMemory((void*)aucRxBuf, (void*)aucTxBuf, 16));
    }
    EXPECT_FALSE(clMbox.Receive((void*)aucRxBuf, 10));
}
TEST_END

void mbox_recv_test(void* unused)
{
    exit_flag = false;
    while (!exit_flag) {
        clMbox.Receive((void*)aucRxBuf, 10);
    }
    clMBoxThread.Exit();
}

TEST(mailbox_send_blocking)
{
    u16Timeouts = 0;
    clMbox.Init((void*)aucMBoxBuffer, MBOX_BUFFER_SIZE, 16);
    clMBoxThread.Init(akMBoxStack, sizeof(akMBoxStack), 7, mbox_recv_test, (void*)&u16Timeouts);

    for (int j = 0; j < 16; j++) {
        aucTxBuf[j] = 'x';
    }

    for (int i = 0; i < 8; i++) {
        clMbox.Send((void*)aucTxBuf);
    }

    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(clMbox.Send((void*)aucTxBuf, 20));
    }

    clMBoxThread.Start();
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(clMbox.Send((void*)aucTxBuf, 20));
    }

    exit_flag = true;
    Thread::Sleep(100);
}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(mailbox_send_recv)
, TEST_CASE(mailbox_blocking_receive), TEST_CASE(mailbox_blocking_timed), TEST_CASE(mailbox_send_blocking),
    TEST_CASE_END
} //namespace Mark3
