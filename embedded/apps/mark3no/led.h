#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

class LED
{
public:
    /*!
     * \brief Init
     *
     * Initialize an LED object prior to its use.
     *
     * \param pu8Port_  Pointer to the port register associated with the LED
     * \param pu8Dir_   Pointer to the direction register associated with the LED
     * \param u8Bit_    Port pin # associated with the LED
     */
    void Init(volatile uint8_t* pu8Port_, volatile uint8_t* pu8Dir_, uint8_t u8Bit_);

    /*!
     * \brief Set
     *
     * Set the LED pin output high.
     */
    void Set();

    /*!
     * \brief Clear
     *
     * Set the LED pin output low.
     */
    void Clear();

    /*!
     * \brief Toggle
     *
     * Toggle the LED state from off to on, or from on to off
     */
    void Toggle();

private:
    volatile uint8_t* m_pu8Port;   //!< Port connected to the LED
    uint8_t m_u8Pin ;              //!< Port pin connected to the LED
};

#endif // __LED_H__
