#ifndef __BSP_BUTTONS_H__
#define __BSP_BUTTONS_H__

#include "button.h"

extern Button clButtons[2];

typedef void (*OnButtonDown_t)(Button* pclButton_);
typedef void (*OnButtonUp_t)(Button* pclButton_);

void bsp_buttons_init(void);

void bsp_buttons_set_callbacks(Button* pclButton_, OnButtonDown_t pfOnDown_, OnButtonUp_t pfOnUp_);

#endif
