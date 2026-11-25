#pragma once

#include "bsp_i2c.h"
#include <stdio.h>
#include <stdarg.h>
#define  OLED_I2C_ADDR 0x78
#define OLED_8X16 8
#define OLED_6X8 6
void oled_i2c_hand_init();
void oled_i2c_soft_init();
void oled_i2c_clear();
void oled_i2c_show_str(uint8_t x, uint8_t y, const char *str, uint8_t font);
void oled_i2c_printf(uint8_t x, uint8_t y,uint8_t font, const char *fmt,...);