#include "mark3.h"
#include "mark3c.h"
#include "ut_platform.h"

namespace Mark3
{
//---------------------------------------------------------------------------
TEST(ut_mark3c_object_sizes)
{
    // Mark3c is pretty simple - it relies on static allocations of "Fake" objects
    // that are essentially raw memory blobs, initialized using placement-new
    // constructors.
    EXPECT_EQUALS(sizeof(Fake_LinkedListNode), sizeof(LinkListNode));
    EXPECT_EQUALS(sizeof(Fake_LinkedList), sizeof(LinkList));
    EXPECT_EQUALS(sizeof(Fake_ThreadList), sizeof(ThreadList));
    EXPECT_EQUALS(sizeof(Fake_Thread), sizeof(Thread));
    EXPECT_EQUALS(sizeof(Fake_Timer), sizeof(Timer));
    EXPECT_EQUALS(sizeof(Fake_Semaphore), sizeof(Semaphore));
    EXPECT_EQUALS(sizeof(Fake_Mutex), sizeof(Mutex));
    EXPECT_EQUALS(sizeof(Fake_Message), sizeof(Message));
    EXPECT_EQUALS(sizeof(Fake_MessageQueue), sizeof(MessageQueue));
    EXPECT_EQUALS(sizeof(Fake_MessagePool), sizeof(MessagePool));
    EXPECT_EQUALS(sizeof(Fake_Mailbox), sizeof(Mailbox));
    EXPECT_EQUALS(sizeof(Fake_Notify), sizeof(Notify));
    EXPECT_EQUALS(sizeof(Fake_EventFlag), sizeof(EventFlag));
    EXPECT_EQUALS(sizeof(Fake_ConditionVariable), sizeof(ConditionVariable));
    EXPECT_EQUALS(sizeof(Fake_ReaderWriterLock), sizeof(ReaderWriterLock));
}

//---------------------------------------------------------------------------
//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_mark3c_object_sizes),
TEST_CASE_END
} // namespace Mark3
