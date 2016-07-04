#include <avr/io.h>
#include "mark3.h"
#include "led.h"

LED clLED1;
LED clLED2;
LED clLED3;

void bsp_leds_init(void)
{
    clLED1.Init(&PORTA, &DDRA, 0);
    clLED2.Init(&PORTA, &DDRA, 1);
    clLED3.Init(&PORTA, &DDRA, 2);
}
