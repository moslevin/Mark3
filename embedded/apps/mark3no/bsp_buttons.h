#ifndef __BSP_BUTTONS_H__
#define __BSP_BUTTONS_H__

#include "button.h"

extern Button clButtons[2];

typedef void (*on_button_down_t)(Button* pclButton_);
typedef void (*on_button_up_t)(Button* pclButton_);

void bsp_buttons_init(void);

void bsp_buttons_set_callbacks(Button* pclButton_, on_button_down_t pfOnDown_, on_button_up_t pfOnUp_);

#endif
