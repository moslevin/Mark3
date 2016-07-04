#include <avr/io.h>
#include "button.h"
#include "mark3.h"
#include "bsp_buttons.h"

//---------------------------------------------------------------------------
#define BUTTON1_PORT        (PORTC)
#define BUTTON1_PORTIN      (PINC)
#define BUTTON1_DDR         (DDRC)

#define BUTTON2_PORT        (PORTC)
#define BUTTON2_PORTIN      (PINC)
#define BUTTON2_DDR         (DDRC)

#define BUTTON1_PIN         (4)
#define BUTTON2_PIN         (5)

#define BUTTON_PCINT        (PCI2)
#define BUTTON_PCIE         (PCIE2)
#define BUTTON_PCMSK        (PCMSK2)

#define BUTTON_PCINT_BASE   (16)
#define BUTTON1_PCINT       (BUTTON1_PIN + BUTTON_PCINT_BASE)
#define BUTTON2_PCINT       (BUTTON2_PIN + BUTTON_PCINT_BASE)

//---------------------------------------------------------------------------
Button clButtons[2];

static Timer clButtonTimer;

static on_button_up_t apfButtonUp[2];
static on_button_down_t apfButtonDown[2];

static volatile bool bExpectedB[2];

//---------------------------------------------------------------------------
static void bsp_button_timer_callback(Thread* pclOwner_, void* pvData_)
{
    static bool bLastB[2];

    bool bB[2]; 

    bB[0] = clButtons[0].ReadState();
    bB[1] = clButtons[1].ReadState();

    for (uint8_t i = 0; i < 2; i++)
    {
        // If the button reads what it did in the ISR, confirm the press,
        // otherwise, assume spurious
        if (bB[i] == bExpectedB[i]) 
        {
            // If the button reads a change (not guaranteed -- we have one timer for multiple buttons)
            if (bB[i] != bLastB[i]) 
            {
                // Confirm the change and run the callback
                bLastB[i] = bB[i];
                if (apfButtonDown[i])
                {
                    if (bB[i])
                    {
                        apfButtonDown[i](&clButtons[i]);
                    }
                    else
                    {
                        apfButtonUp[i](&clButtons[i]);
                    }
                }
            }
        }
    }

    PCIFR &= ~(1 << BUTTON_PCIE);
    PCICR |= (1 << BUTTON_PCIE);
}

//---------------------------------------------------------------------------
void bsp_buttons_init(void)
{
    // Disable jtag to enable portc GPIOs.  !!MUST BE CALLED WITH INTERRUPTS DISABLED!!
    MCUCR = (1<<JTD);
    MCUCR = (1<<JTD);

    // Enable pullups
    BUTTON1_PORT |= (1 << BUTTON1_PIN);
    BUTTON2_PORT |= (1 << BUTTON2_PIN);

    BUTTON1_DDR &= ~(1 << BUTTON1_PIN);
    BUTTON2_DDR &= ~(1 << BUTTON2_PIN);

    clButtons[0].Init(&BUTTON1_PORTIN, &BUTTON1_DDR, BUTTON1_PIN);
    clButtons[1].Init(&BUTTON2_PORTIN, &BUTTON2_DDR, BUTTON2_PIN);

    // Enable pinchange interrupts on the button IOs
    PCIFR &= ~(1 << BUTTON_PCIE);
    PCICR |= (1 << BUTTON_PCIE);

    BUTTON_PCMSK |= (1 << BUTTON1_PIN) | (1 <<BUTTON2_PIN);

}

//---------------------------------------------------------------------------
void bsp_buttons_set_callbacks(Button* pclButton_, on_button_down_t pfOnDown_, on_button_up_t pfOnUp_)
{
    uint8_t i = 0;
    if (pclButton_ == &clButtons[0])
    {
        i = 0;
    }
    else if (pclButton_ == &clButtons[1])
    {
        i = 1;
    }
    apfButtonUp[i] = pfOnUp_;
    apfButtonDown[i] = pfOnDown_;
}

//---------------------------------------------------------------------------
ISR(PCINT2_vect)
{
    bExpectedB[0] = clButtons[0].ReadState();
    bExpectedB[1] = clButtons[1].ReadState();

    // One-shot timer for debounce.
    clButtonTimer.Start(false, 100, bsp_button_timer_callback, 0);

    // disable the pc interrupt for debounce.
    PCICR &= ~(1 << BUTTON_PCIE);
}

