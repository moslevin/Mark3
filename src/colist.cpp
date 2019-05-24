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
=========================================================================== */
/**
    @file   colist.cpp

    @brief  CoRoutine List structure implementation

 */
#include "colist.h"

namespace Mark3 {
//---------------------------------------------------------------------------
void CoList::SetPrioMap(CoPrioMap* pclPrioMap_)
{
    m_pclPrioMap = pclPrioMap_;
}

//---------------------------------------------------------------------------
void CoList::SetPriority(PORT_PRIO_TYPE uPriority)
{
    m_uPriority = uPriority;
}

//---------------------------------------------------------------------------
void CoList::Add(Coroutine* pclCoroutine_)
{
    TypedCircularLinkList<Coroutine>::Add(pclCoroutine_);
    PivotForward();
    if (m_pclPrioMap) {
        m_pclPrioMap->Set(m_uPriority);
    }
}

//---------------------------------------------------------------------------
void CoList::Remove(Coroutine* pclCoroutine_)
{
    TypedCircularLinkList<Coroutine>::Remove(pclCoroutine_);
    if (m_pclPrioMap) {
        if (nullptr == GetHead()) {
            m_pclPrioMap->Clear(m_uPriority);
        }
    }
}
} // namespace Mark3
