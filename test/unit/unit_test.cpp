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
/**
    @file unit_test.cpp
    @brief Unit test class definition
*/

#include "kerneltypes.h"
#include "unit_test.h"

namespace Mark3
{
//---------------------------------------------------------------------------
UnitTest::UnitTest()
{
    m_bIsActive     = false;
    m_u16Iterations = 0;
    m_u16Passed     = 0;
    m_bComplete     = false;
}

//---------------------------------------------------------------------------
void UnitTest::Pass()
{
    if (m_bComplete) {
        return;
    }

    if (m_bIsActive) {
        m_bIsActive = false;
        m_u16Iterations++;
        m_u16Passed++;
        m_bStatus = true;
    }
}

//---------------------------------------------------------------------------
void UnitTest::Fail()
{
    if (m_bComplete) {
        return;
    }

    if (m_bIsActive) {
        m_bIsActive = false;
        m_u16Iterations++;
        m_bStatus = false;
    }
}
} // namespace Mark3
