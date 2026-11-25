#pragma once
#include "bsp_i2c.h"

#define BH1750_ADDR (0x23<<1)


void bh1750_init();
uint16_t bh1750_read_data();