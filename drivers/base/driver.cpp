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
===========================================================================*/
/**

    @file   driver.cpp

    @brief  Device driver/hardware abstraction layer
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "driver.h"

#include "kerneldebug.h"

namespace Mark3
{
namespace
{
    //---------------------------------------------------------------------------
    /**
        This class implements the "default" driver (/dev/null)
    */
    class DevNull : public Driver
    {
    public:
        virtual int Init()
        {
            SetName("/dev/null");
            return 0;
        }
        virtual int    Open() { return 0; }
        virtual int    Close() { return 0; }
        virtual size_t Read(void* /*pu8Data_*/, size_t uBytes_) { return uBytes_; }
        virtual size_t Write(const void* /*pu8Data_*/, size_t uBytes_) { return uBytes_; }
        virtual int    Control(uint16_t /*u16EventID_*/,
                               void* /*pvDataIn_*/,
                               size_t /*uSizeIn_*/,
                               const void* /*pvDataOut_*/,
                               size_t /*uSizeOut_*/)
        {
            return 0;
        }
    };

    //---------------------------------------------------------------------------
    DevNull clDevNull; //!< Default driver included to allow for run-time "stubbing"

    //---------------------------------------------------------------------------
    /**
     * @brief DrvCmp
     *
     * String comparison function used to compare input driver name against a
     * known driver name in the existing driver list.
     *
     * @param szStr1_   user-specified driver name
     * @param szStr2_   name of a driver, provided from the driver table
     * @return  1 on match, 0 on no-match
     */
    static uint8_t DrvCmp(const char* szStr1_, const char* szStr2_)
    {
        auto* szTmp1 = szStr1_;
        auto* szTmp2 = szStr2_;

        while ((*szTmp1 != 0) && (*szTmp2 != 0)) {
            if (*szTmp1++ != *szTmp2++) {
                return 0;
            }
        }

        // Both terminate at the same length
        if (((*szTmp1) == 0) && ((*szTmp2) == 0)) {
            return 1;
        }

        return 0;
    }
} // anonymous namespace

DoubleLinkList DriverList::m_clDriverList;

//---------------------------------------------------------------------------
void DriverList::Init()
{
    // Ensure we always have at least one entry - a default in case no match
    // is found (/dev/null)
    clDevNull.Init();
    Add(&clDevNull);
}

//---------------------------------------------------------------------------
Driver* DriverList::FindByPath(const char* m_pcPath)
{
    KERNEL_ASSERT(m_pcPath);
    auto* pclTemp = static_cast<Driver*>(m_clDriverList.GetHead());

    // Iterate through the list of drivers until we find a match, or we
    // exhaust our list of installed drivers
    while (pclTemp != nullptr) {
        if (DrvCmp(m_pcPath, pclTemp->GetPath()) != 0u) {
            return pclTemp;
        }
        pclTemp = static_cast<Driver*>(pclTemp->GetNext());
    }
    // No matching driver found - return a pointer to our /dev/null driver
    return &clDevNull;
}
} // namespace Mark3
