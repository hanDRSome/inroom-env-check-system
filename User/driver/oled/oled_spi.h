#pragma once
#include "bsp_spi.h"
#include <stdio.h>
#include <stdarg.h>

void oled_spi_hard_init();
void oled_spi_soft_init();

void oled_spi_clear(void);

/* 字体大小（字符横向占的像素宽度） */
#define OLED_6X8    6
#define OLED_8X16   8

/* 字符显示：x = 列(0~127)，page = 页(0~7，一页 8 像素高)，page 等价于 Y/8 */

void oled_spi_show_str(uint8_t x, uint8_t y, const char *str, uint8_t font);
void oled_spi_printf(uint8_t x, uint8_t y,uint8_t font, const char *fmt,...);
