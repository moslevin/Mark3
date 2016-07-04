#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

class Button
{
public:
    /*!
     * \brief Init
     *
     * Initialize the button prior to use.
     *
     * \param pu8Port_ Pointer to port register of port connected to button
     * \param pu8Dir_  Pointer to direction register of port connected to button
     * \param u8Bit_   Bit/Pin # on port connected to putton
     */
    void Init(volatile uint8_t* pu8Port_, volatile uint8_t* pu8Dir_, uint8_t u8Bit_);

    /*!
     * \brief ReadState
     *
     * Read the current state of the pushbutton.
     *
     * \return true -- button down, false -- button up.
     */
    bool ReadState(void);

private:
    volatile uint8_t* m_pu8Port;    //!< Pointer to port input register of port connected to Button
    volatile uint8_t m_u8Pin;       //!< Pointer to pin on port connected to Button
};

#endif // __BUTTON_H__
