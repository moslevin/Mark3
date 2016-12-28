#include "button.h"

#include <stdint.h>

//---------------------------------------------------------------------------
void Button::Init(volatile uint8_t* pu8Port_, volatile uint8_t* pu8Dir_, uint8_t u8Bit_)
{
    m_pu8Port = pu8Port_;
    *pu8Dir_ &= ~(1 << u8Bit_);
    m_u8Pin = u8Bit_;
}

//---------------------------------------------------------------------------
bool Button::ReadState(void)
{
    uint8_t u8Val = *m_pu8Port;

    return ((u8Val & (1 << m_u8Pin)) != 0);
}
