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
=========================================================================== */
/*!
    \file slab.cpp
    \brief Slab allocator class implementation
*/

#include "slab.h"
#include "mark3.h"
namespace Mark3 {
//---------------------------------------------------------------------------
void SlabPage::InitPage(uint32_t u32PageSize_, uint32_t u32ObjSize_)
{
    LinkListNode::ClearNode();
    void* pvBlock = (void*)((uint32_t)this + sizeof(SlabPage));        
    m_clAllocator.Init(pvBlock, u32PageSize_ - sizeof(SlabPage), u32ObjSize_);
}

//---------------------------------------------------------------------------
void* SlabPage::Alloc(void* pvTag_)
{
    return m_clAllocator.Allocate(pvTag_);
}

//---------------------------------------------------------------------------
void SlabPage::Free(void* pvObject_)
{
    m_clAllocator.Free(pvObject_);
}

//---------------------------------------------------------------------------
bool SlabPage::IsEmpty(void)
{
    return m_clAllocator.IsEmpty();
}

//---------------------------------------------------------------------------
bool SlabPage::IsFull(void)
{
    return m_clAllocator.IsFull();
}

//---------------------------------------------------------------------------
void Slab::Init(uint32_t u32ObjSize_, slab_alloc_page_function_t pfAlloc_, slab_free_page_function_t pfFree_)
{
    m_pfSlabAlloc = pfAlloc_;
    m_pfSlabFree = pfFree_;
    m_u32ObjSize = u32ObjSize_;
    m_clFreeList.Init();
    m_clFullList.Init();
}

//---------------------------------------------------------------------------
void* Slab::Alloc(void)
{
    // Allocate from free page list
    SlabPage* pclCurr = (SlabPage*)m_clFreeList.GetHead();
    if (!pclCurr) {
        pclCurr = AllocSlabPage();
        if (!pclCurr) {
            return NULL;
        }
    }
    void* pvRC = pclCurr->Alloc(pclCurr);
    if (pclCurr->IsFull()) {
        MoveToFull(pclCurr);
    }
    return pvRC;
}

//---------------------------------------------------------------------------
void Slab::Free(void* pvObj_)
{
    // Get page from object data
    bitmap_alloc_t *pstObj_ = (bitmap_alloc_t*)((uint32_t)pvObj_ - (sizeof(bitmap_alloc_t) - sizeof(K_WORD)));
    SlabPage* pclPage = (SlabPage*)pstObj_->pvTag;
    if (pclPage->IsFull()) {
        MoveToFree(pclPage);
    }

    pclPage->Free(pvObj_);

    if (pclPage->IsEmpty()) {
        FreeSlabPage(pclPage);
    }
}

//---------------------------------------------------------------------------
SlabPage* Slab::AllocSlabPage(void)
{
    uint32_t u32PageSize;
    SlabPage* pclNewPage = (SlabPage*)m_pfSlabAlloc(&u32PageSize);
    if (!pclNewPage) {
        return NULL;
    }
    
    pclNewPage->InitPage(u32PageSize, m_u32ObjSize);

    m_clFreeList.Add(pclNewPage);
    return pclNewPage;
}

//---------------------------------------------------------------------------
void Slab::FreeSlabPage(SlabPage* pclPage_)
{
    m_clFreeList.Remove(pclPage_);
    m_pfSlabFree(pclPage_);
}

//---------------------------------------------------------------------------
void Slab::MoveToFull(SlabPage* pclPage_)
{
    m_clFreeList.Remove(pclPage_);
    m_clFullList.Add(pclPage_);
}

//---------------------------------------------------------------------------
void Slab::MoveToFree(SlabPage* pclPage_)
{
    m_clFullList.Remove(pclPage_);
    m_clFreeList.Add(pclPage_);
}
} //namespace Mark3
