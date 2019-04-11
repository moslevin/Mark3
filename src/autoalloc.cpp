/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**
    @file   autoalloc.cpp

    @brief  Automatic memory allocation for kernel objects.
*/
#include "mark3.h"

#include <stdint.h>

//---------------------------------------------------------------------------
// Override new() and delete() using functions provided to AutoAlloc
//---------------------------------------------------------------------------
#if PORT_OVERLOAD_NEW
using namespace Mark3;
void* operator new(size_t n)
{
    return AutoAlloc::NewRawData(n);
}

//---------------------------------------------------------------------------
void* operator new[](size_t n)
{
    return AutoAlloc::NewRawData(n);
}

//---------------------------------------------------------------------------
void operator delete(void* p)
{
    AutoAlloc::DestroyRawData(p);
}

//---------------------------------------------------------------------------
void operator delete[](void* p)
{
    AutoAlloc::DestroyRawData(p);
}
#endif

namespace Mark3 {

AutoAllocAllocator_t    AutoAlloc::m_pfAllocator; //!< Function used to allocate objects
AutoAllocFree_t         AutoAlloc::m_pfFree;      //!< Funciton used to free objects

//---------------------------------------------------------------------------
void* AutoAlloc::Allocate(AutoAllocType eType_, size_t sSize_)
{
    if (!m_pfAllocator) {
        return nullptr;
    }
    return m_pfAllocator(eType_, sSize_);
}

//---------------------------------------------------------------------------
void AutoAlloc::Free(AutoAllocType eType_, void* pvObj_)
{
    if (!m_pfFree) {
        return;
    }
    m_pfFree(eType_, pvObj_);
}

//---------------------------------------------------------------------------
void AutoAlloc::SetAllocatorFunctions(AutoAllocAllocator_t pfAllocator_, AutoAllocFree_t pfFree_)
{
    m_pfAllocator = pfAllocator_;
    m_pfFree      = pfFree_;
}

//---------------------------------------------------------------------------
void AutoAlloc::Init()
{
    m_pfAllocator = nullptr;
    m_pfFree      = nullptr;
}

//---------------------------------------------------------------------------
void* AutoAlloc::NewUserTypeAllocation(uint8_t eType_)
{
    return Allocate(static_cast<AutoAllocType>(eType_), 0);
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyUserTypeAllocation(uint8_t eUserType_, void* pvObj_)
{
    Free(static_cast<AutoAllocType>(eUserType_), pvObj_);
}
//---------------------------------------------------------------------------
void* AutoAlloc::NewRawData(size_t sSize_)
{
    return Allocate(AutoAllocType::Raw, sSize_);
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyRawData(void* pvData_)
{
    Free(AutoAllocType::Raw, pvData_);
}

} // namespace Mark3
