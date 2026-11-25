#pragma once

#include "bsp_sys.h"

#define SR501_Port sr501_in_GPIO_Port
#define SR501_Pin sr501_in_Pin

void sr501_init();
uint8_t sr501_read();