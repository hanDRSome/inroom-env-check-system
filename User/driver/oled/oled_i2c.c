#include "oled_data.h"
#include "oled_i2c.h"

i2c_t oled_i2c = {
    {oled_i2c_scl_GPIO_Port,oled_i2c_scl_Pin},
    {oled_i2c_sda_GPIO_Port,oled_i2c_sda_Pin}
};


static void write_cmd(uint8_t cmd){
    i2c_start(&oled_i2c);
    i2c_write_byte(&oled_i2c,OLED_I2C_ADDR);
    i2c_read_ack(&oled_i2c);
    i2c_write_byte(&oled_i2c,0x00);// 写命令
    i2c_read_ack(&oled_i2c);
    i2c_write_byte(&oled_i2c,cmd);
    i2c_read_ack(&oled_i2c);
    i2c_stop(&oled_i2c);
}
static void write_data(uint8_t data){
    i2c_start(&oled_i2c);
    i2c_write_byte(&oled_i2c,OLED_I2C_ADDR);
    i2c_read_ack(&oled_i2c);
    i2c_write_byte(&oled_i2c,0x40);// 写数据
    i2c_read_ack(&oled_i2c);
    i2c_write_byte(&oled_i2c,data);
    i2c_read_ack(&oled_i2c);
    i2c_stop(&oled_i2c);
}

void oled_i2c_soft_init(){
    delay_ms(100);
    write_cmd(0xAE);    //设置显示开启/关闭，0xAE关闭，0xAF开启

    write_cmd(0xD5);    // 时钟分频
    write_cmd(0x80);

    write_cmd(0xA8);    // 复用率
    write_cmd(0x3F);    // 1/64

    write_cmd(0xD3);    // 显示偏移
    write_cmd(0x00);    //0x00~0x7F

    write_cmd(0x40);    // 设置显示开始行，0x40~0x7F

    write_cmd(0x8D);    // 电荷泵
    write_cmd(0x14);    // 使能

    write_cmd(0x20);    // 寻址模式
    write_cmd(0x02);    // 页寻址

    write_cmd(0xA1);    // 设置左右方向，0xA1正常，0xA0左右反置
    write_cmd(0xC8);    // 设置上下方向，0xC8正常，0xC0上下反置

    write_cmd(0xDA);    // COM 配置
    write_cmd(0x12);

    write_cmd(0x81);    // 对比度
    write_cmd(0xCF);    //0x00~0xFF

    write_cmd(0xD9);    // 预充电
    write_cmd(0xF1);

    write_cmd(0xDB);    // VCOMH
    write_cmd(0x30);

    write_cmd(0xA4);    // 显示跟随 RAM
    write_cmd(0xA6);    // 设置正常/反色显示，0xA6正常，0xA7反色

    write_cmd(0xAF);    // 打开显示
}

void oled_i2c_clear()
{
    for (uint8_t page = 0; page < 8; page++) {
        write_cmd(0xB0 + page);
        write_cmd(0x10);
        write_cmd(0x00);

        for (uint16_t col = 0; col < 128; col++) {
            write_data(0x00);
        }
    }
}
//=======================字符显示=========================
/* 页寻址模式下定位到 (x, page) */
static void OLED_SetPageCol(uint8_t x, uint8_t page)
{
    write_cmd(0xB0 | (page & 0x07));
    write_cmd(0x10 | ((x >> 4) & 0x0F));
    write_cmd(0x00 | (x & 0x0F));
}

/*
 * 在 (x, y) 处画一列 8 个像素
 * colbits: 列数据，bit0 在上、bit7 在下（与字模一致）
 * y 不页对齐时，数据会跨两页写入
 */
static void OLED_DrawColumn(uint8_t x, uint8_t y, uint8_t colbits)
{
    uint8_t page   = y >> 3;        // y / 8
    uint8_t offset = y & 0x07;      // y % 8

    if (offset == 0) {
        /* 页对齐：一页写完，不移位（避免 >>8 的 UB） */
        OLED_SetPageCol(x, page);
        write_data(colbits);
    } else {
        /* 上半部分落在当前页，下半部分落到下一页 */
        uint8_t upper = (uint8_t)(colbits << offset);
        uint8_t lower = (uint8_t)(colbits >> (8 - offset));

        OLED_SetPageCol(x, page);
        write_data(upper);

        if (page < 7) {             // 下一页存在才写，避免绕回顶部
            OLED_SetPageCol(x, page + 1);
            write_data(lower);
        }
    }
}

/* 在 (x, y) 显示一个 ASCII 字符，font 取 OLED_6X8 或 OLED_8X16 */
static void oled_i2c_show_char(uint8_t x, uint8_t y, char ch, uint8_t font)
{
    uint8_t i;

    if (ch < ' ' || ch > '~') ch = ' ';     // 不支持的字符用空格顶替

    if (font == OLED_8X16) {
        const uint8_t *p = OLED_F8x16[(uint8_t)(ch - ' ')];
            for (i = 0; i < 8; i++) {
                OLED_DrawColumn((uint8_t)(x + i), y, p[i]);
            }
        for (i = 0; i < 8; i++) {
            OLED_DrawColumn((uint8_t)(x + i), (uint8_t)(y + 8), p[8 + i]);
        }
    } else {
        const uint8_t *p = OLED_F6x8[(uint8_t)(ch - ' ')];
        for (i = 0; i < 6; i++) {
            OLED_DrawColumn((uint8_t)(x + i), y, p[i]);
        }
    }
}

/* 显示字符串，超出屏幕右边自动截断 */
void oled_i2c_show_str(uint8_t x, uint8_t y, const char *str, uint8_t font)
{
    uint8_t width = (font == OLED_8X16) ? 8 : 6;

    while (*str != '\0') {
        if (x > 128 - width) break;         // 这一行放不下了
        oled_i2c_show_char(x, y, *str, font);
        x += width;
        str++;
    }
}

void oled_i2c_printf(uint8_t x, uint8_t y,uint8_t font, const char *fmt,...){
    char buf[128];
    va_list args;
    va_start(args,fmt);
    vsprintf(buf,fmt,args);
    va_end(args);
    oled_i2c_show_str(x,y,buf,font);
}