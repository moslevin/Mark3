/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "mark3.h"

namespace {
using namespace Mark3;

Thread clTestThread1;
K_WORD awThreadStack1[PORT_KERNEL_DEFAULT_STACK_SIZE];

Thread clTestThread2;
K_WORD awThreadStack2[PORT_KERNEL_DEFAULT_STACK_SIZE];

Thread clTestThread3;
K_WORD awThreadStack3[PORT_KERNEL_DEFAULT_STACK_SIZE];

volatile int iNumWrites;
volatile int iNumReads;

void WriterTask(void* param)
{
    auto pclRWLock = static_cast<ReaderWriterLock*>(param);
    pclRWLock->AcquireWriter();
    iNumWrites++;
    Scheduler::GetCurrentThread()->Sleep(50);
    pclRWLock->ReleaseWriter();
    Scheduler::GetCurrentThread()->Exit();
}

void ReaderTask(void* param)
{
    auto pclRWLock = static_cast<ReaderWriterLock*>(param);
    pclRWLock->AcquireReader();
    iNumReads++;
    Scheduler::GetCurrentThread()->Sleep(50);
    pclRWLock->ReleaseReader();
    Scheduler::GetCurrentThread()->Exit();
}
} // anonymous namespace

namespace Mark3 {
#if KERNEL_USE_READERWRITER
//===========================================================================
// Local Defines
//===========================================================================

//===========================================================================
// Define Test Cases Here
//===========================================================================

TEST(ut_rw_single_reader)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    clTestThread1.Start();

    Thread::Sleep(25);
    EXPECT_EQUALS(iNumReads, 1);

    // Wait for thread to die...
    Thread::Sleep(50);
}
TEST_END

TEST(ut_rw_single_writer)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    clTestThread1.Start();

    Thread::Sleep(25);
    EXPECT_EQUALS(iNumWrites, 1);

    // Wait for thread to die...
    Thread::Sleep(50);
}
TEST_END

TEST(ut_rw_multiple_writers_block)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    clTestThread1.Start();

    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    clTestThread2.Start();

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack3),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    clTestThread3.Start();

    Thread::Sleep(25);
    EXPECT_EQUALS(iNumWrites, 1);

    Thread::Sleep(50);
    EXPECT_EQUALS(iNumWrites, 2);

    Thread::Sleep(50);
    EXPECT_EQUALS(iNumWrites, 3);

    // Wait for threads to die...
    Thread::Sleep(50);
}
TEST_END

TEST(ut_rw_multiple_readers_no_block)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    clTestThread1.Start();

    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    clTestThread2.Start();

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack3),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    clTestThread3.Start();

    Thread::Sleep(25);
    EXPECT_EQUALS(iNumReads, 3);

    // Wait for threads to die...
    Thread::Sleep(50);
}
TEST_END

TEST(ut_rw_reader_blocks_writer)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    // 2x reader threads
    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    // 1x writer thread
    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    // Verify that reader blocks pending writer
    clTestThread1.Start();
    Thread::Sleep(10);
    clTestThread2.Start();
    Thread::Sleep(10);
    EXPECT_EQUALS(iNumReads, 1);
    EXPECT_EQUALS(iNumWrites, 0);

    // Start 2nd reader to verify that pending writer has no effect on concurrent reads
    clTestThread3.Start();
    Thread::Sleep(10);
    EXPECT_EQUALS(iNumReads, 2);
    EXPECT_EQUALS(iNumWrites, 0);

    // Wait for threads to die, verify that writer got the lock
    Thread::Sleep(100);
    EXPECT_EQUALS(iNumReads, 2);
    EXPECT_EQUALS(iNumWrites, 1);
}
TEST_END

TEST(ut_rw_writer_blocks_reader)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    // 2x writer threads
    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);

    // 1x writer thread
    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);

    // Verify that writer blocks pending writer
    clTestThread1.Start();
    Thread::Sleep(10);
    clTestThread2.Start();
    Thread::Sleep(10);
    EXPECT_EQUALS(iNumWrites, 1);
    EXPECT_EQUALS(iNumReads, 0);

    // Start 2nd writer, verify it also blocks
    clTestThread3.Start();
    Thread::Sleep(10);
    EXPECT_EQUALS(iNumWrites, 1);
    EXPECT_EQUALS(iNumReads, 0);
    Thread::Sleep(50);

    // Verify that the reader got the lock next (waiting longest)
    EXPECT_EQUALS(iNumWrites, 1);
    EXPECT_EQUALS(iNumReads, 1);

    // Wait until all threads done, then check to ensure 2nd writer eventually got the lock
    Thread::Sleep(100);
    EXPECT_EQUALS(iNumWrites, 2);
    EXPECT_EQUALS(iNumReads, 1);
}
TEST_END

TEST(ut_rw_writer_timeout)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);
    clTestThread1.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iNumReads, 1);

    auto success = clRWLock.AcquireWriter(10);
    EXPECT_EQUALS(iNumReads, 1);
    EXPECT_FALSE(success);

    Thread::Sleep(50);
}
TEST_END

TEST(ut_rw_reader_timeout)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);
    clTestThread1.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iNumWrites, 1);

    auto success = clRWLock.AcquireReader(10);
    EXPECT_EQUALS(iNumWrites, 1);
    EXPECT_FALSE(success);

    Thread::Sleep(50);
}
TEST_END

TEST(ut_rw_timed_read_success)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       WriterTask, &clRWLock);
    clTestThread1.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iNumWrites, 1);

    auto success = clRWLock.AcquireReader(50);
    EXPECT_EQUALS(iNumWrites, 1);
    EXPECT_TRUE(success);

}
TEST_END

TEST(ut_rw_timed_write_success)
{
    ReaderWriterLock clRWLock;
    clRWLock.Init();
    iNumReads = 0;
    iNumWrites = 0;

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       ReaderTask, &clRWLock);
    clTestThread1.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iNumReads, 1);

    auto success = clRWLock.AcquireWriter(50);
    EXPECT_EQUALS(iNumReads, 1);
    EXPECT_TRUE(success);
}
TEST_END

#endif // KERNEL_USE_READERWRITER
//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
#if KERNEL_USE_READERWRITER
 TEST_CASE(ut_rw_single_reader),
 TEST_CASE(ut_rw_single_writer),
 TEST_CASE(ut_rw_multiple_writers_block),
 TEST_CASE(ut_rw_multiple_readers_no_block),
 TEST_CASE(ut_rw_reader_blocks_writer),
 TEST_CASE(ut_rw_writer_blocks_reader),
 TEST_CASE(ut_rw_writer_timeout),
 TEST_CASE(ut_rw_reader_timeout),
 TEST_CASE(ut_rw_timed_read_success),
 TEST_CASE(ut_rw_timed_write_success),
#endif
TEST_CASE_END
} //namespace Mark3
