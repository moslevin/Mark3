/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "message.h"
#include "thread.h"

namespace
{
//===========================================================================
// Local Defines
//===========================================================================

using namespace Mark3;
Thread           clMsgThread;
K_WORD           aucMsgStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
MessageQueue     clMsgQ;
volatile uint8_t u8PassCount = 0;
#define MESSAGE_POOL_SIZE (3)
MessagePool   s_clMessagePool;
Message       s_clMessages[MESSAGE_POOL_SIZE];
volatile bool bTimedOut = false;

//===========================================================================
void MsgTimed(void* /*unused*/)
{
    u8PassCount  = 0;
    auto* pclRet = clMsgQ.Receive(10);
    if (0 == pclRet) {
        u8PassCount++;
    } else {
        s_clMessagePool.Push(pclRet);
    }

    pclRet = clMsgQ.Receive(1000);
    if (0 != pclRet) {
        u8PassCount++;
    } else {
        s_clMessagePool.Push(pclRet);
    }

    while (1) {
        pclRet = clMsgQ.Receive();
        s_clMessagePool.Push(pclRet);
    }
}

void MsgConsumer(void* /*unused_*/)
{
    uint8_t i;
    for (i = 0; i < 20; i++) {
        auto* pclMsg = clMsgQ.Receive();
        u8PassCount  = 0;

        if (pclMsg) {
            u8PassCount++;
        } else {
            u8PassCount = 0;
            continue;
        }

        switch (i) {
            case 0:
                if (0 == pclMsg->GetCode()) {
                    u8PassCount++;
                }
                if (0 == pclMsg->GetData()) {
                    u8PassCount++;
                }
                break;
            case 1:
                if (1337 == (pclMsg->GetCode())) {
                    u8PassCount++;
                }
                if (7331 == reinterpret_cast<K_ADDR>(pclMsg->GetData())) {
                    u8PassCount++;
                }

            case 2:
                if (0xA0A0 == (pclMsg->GetCode())) {
                    u8PassCount++;
                }
                if (0xC0C0 == reinterpret_cast<K_ADDR>(pclMsg->GetData())) {
                    u8PassCount++;
                }

                break;

            default: break;
        }
        s_clMessagePool.Push(pclMsg);
    }
}

} // anonymous namespace

namespace Mark3
{
//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_message_tx_rx)
{
    // Test - verify that we can use a message queue object to send data
    // from one thread to another, and that the receiver can block on the
    // message queue.  This test also relies on priority scheduling working
    // as expected.

    s_clMessagePool.Init();
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++) {
        s_clMessages[i].Init();
        s_clMessagePool.Push(&s_clMessages[i]);
    }

    clMsgThread.Init(aucMsgStack, sizeof(aucMsgStack), 7, MsgConsumer, 0);

    clMsgQ.Init();

    clMsgThread.Start();

    // Get a message from the pool
    auto* pclMsg = s_clMessagePool.Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetData(0);
    pclMsg->SetCode(0);

    clMsgQ.Send(pclMsg);
    EXPECT_EQUALS(u8PassCount, 3);

    pclMsg = s_clMessagePool.Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetCode(1337);
    pclMsg->SetData(reinterpret_cast<void*>(7331));

    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 3);

    pclMsg = s_clMessagePool.Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetCode(0xA0A0);
    pclMsg->SetData((void*)0xC0C0);

    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 3);

    clMsgThread.Exit();
}

//===========================================================================
TEST(ut_message_exhaust)
{
    s_clMessagePool.Init();
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++) {
        s_clMessages[i].Init();
        s_clMessagePool.Push(&s_clMessages[i]);
    }

    // Test - exhaust the global message pool and ensure that we eventually
    // get "nullptr" returned when the pool is depleted, and not some other
    // unexpected condition/system failure.
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++) { EXPECT_FAIL_FALSE(s_clMessagePool.Pop()); }
    EXPECT_FALSE(s_clMessagePool.Pop());

    // Test is over - re-init the pool..
    s_clMessagePool.Init();
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++) {
        s_clMessages[i].Init();
        s_clMessagePool.Push(&s_clMessages[i]);
    }
}

//===========================================================================
TEST(ut_message_timed_rx)
{
    s_clMessagePool.Init();
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++) {
        s_clMessages[i].Init();
        s_clMessagePool.Push(&s_clMessages[i]);
    }

    auto* pclMsg = s_clMessagePool.Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetData(0);
    pclMsg->SetCode(0);

    // Test - Verify that the timed blocking in the message queues works
    clMsgThread.Init(aucMsgStack, sizeof(aucMsgStack), 7, MsgTimed, 0);
    clMsgThread.Start();

    // Just let the timeout expire
    Thread::Sleep(11);
    EXPECT_EQUALS(u8PassCount, 1);

    // other thread has a timeout set... Don't leave them waiting!
    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 2);

    clMsgQ.Send(pclMsg);

    clMsgThread.Exit();
}

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_message_tx_rx), TEST_CASE(ut_message_exhaust), TEST_CASE(ut_message_timed_rx), TEST_CASE_END
} // namespace Mark3
