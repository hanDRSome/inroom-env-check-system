#include "oled_spi.h"
#include "OLED_Data.h"

#ifdef my_spi
//==========================移植区==========================
cs_t my_spi_cs[10];
#define OLED1_54_ID  0


static inline void W_RES(uint8_t bit){
    gpio_write(oled_res_GPIO_Port,oled_res_Pin,bit?1:0);
}
static inline void W_DC(uint8_t bit){
    gpio_write(oled_dc_GPIO_Port,oled_dc_Pin,bit?1:0);
}
static inline void W_CS(uint8_t bit){
    gpio_write(my_spi_cs[OLED1_54_ID].cs.port,my_spi_cs[OLED1_54_ID].cs.pin,bit);
}


//=======================逻辑区============================
static void OLED_SwapByte(uint8_t byte){
    spi_swapbyte(&my_spi,OLED1_54_ID,byte);
}

static void OLED_Reset(void)
{
    W_RES(1);
    delay_ms(10);
    W_RES(0);
    delay_ms(10);
    W_RES(1);
    delay_ms(10);
}
static void OLED_WriteCmd(uint8_t cmd)
{
    W_DC(0);
    W_CS(0);
    OLED_SwapByte(cmd);
    W_CS(1);
}
static void OLED_WriteData(uint8_t cmd)
{
    W_DC(1);
    W_CS(0);
    OLED_SwapByte(cmd);
    W_CS(1);
}


// 功能区

void oled_spi_hard_init(){
    // 自定义
    my_spi.cs_p = my_spi_cs;

    my_spi_cs[OLED1_54_ID].id = OLED1_54_ID;
    my_spi_cs[OLED1_54_ID].cs.port = oled_cs_GPIO_Port;
    my_spi_cs[OLED1_54_ID].cs.pin = oled_cs_Pin;
}
void oled_spi_soft_init(){

    //
    OLED_Reset();
    OLED_WriteCmd(0xAE);    //设置显示开启/关闭，0xAE关闭，0xAF开启

    OLED_WriteCmd(0xD5);    // 时钟分频
    OLED_WriteCmd(0x80);

    OLED_WriteCmd(0xA8);    // 复用率
    OLED_WriteCmd(0x3F);    // 1/64

    OLED_WriteCmd(0xD3);    // 显示偏移
    OLED_WriteCmd(0x00);    //0x00~0x7F

    OLED_WriteCmd(0x40);    // 设置显示开始行，0x40~0x7F

    OLED_WriteCmd(0x8D);    // 电荷泵
    OLED_WriteCmd(0x14);    // 使能

    OLED_WriteCmd(0x20);    // 寻址模式
    OLED_WriteCmd(0x02);    // 页寻址

    OLED_WriteCmd(0xA1);    // 设置左右方向，0xA1正常，0xA0左右反置
    OLED_WriteCmd(0xC8);    // 设置上下方向，0xC8正常，0xC0上下反置

    OLED_WriteCmd(0xDA);    // COM 配置
    OLED_WriteCmd(0x12);

    OLED_WriteCmd(0x81);    // 对比度
    OLED_WriteCmd(0xCF);    //0x00~0xFF

    OLED_WriteCmd(0xD9);    // 预充电
    OLED_WriteCmd(0xF1);

    OLED_WriteCmd(0xDB);    // VCOMH
    OLED_WriteCmd(0x30);

    OLED_WriteCmd(0xA4);    // 显示跟随 RAM
    OLED_WriteCmd(0xA6);    // 设置正常/反色显示，0xA6正常，0xA7反色

    OLED_WriteCmd(0xAF);    // 打开显示
}
// ===== 清屏 =====
void oled_spi_clear()
{
    for (uint8_t page = 0; page < 8; page++) {
        OLED_WriteCmd(0xB0 + page);
        OLED_WriteCmd(0x10);
        OLED_WriteCmd(0x00);

        for (uint16_t col = 0; col < 128; col++) {
            OLED_WriteData(0x00);
        }
    }
}

//=======================字符显示=========================
/* 页寻址模式下定位到 (x, page) */
static void OLED_SetPageCol(uint8_t x, uint8_t page)
{
    OLED_WriteCmd(0xB0 | (page & 0x07));
    OLED_WriteCmd(0x10 | ((x >> 4) & 0x0F));
    OLED_WriteCmd(0x00 | (x & 0x0F));
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
        OLED_WriteData(colbits);
    } else {
        /* 上半部分落在当前页，下半部分落到下一页 */
        uint8_t upper = (uint8_t)(colbits << offset);
        uint8_t lower = (uint8_t)(colbits >> (8 - offset));

        OLED_SetPageCol(x, page);
        OLED_WriteData(upper);

        if (page < 7) {             // 下一页存在才写，避免绕回顶部
            OLED_SetPageCol(x, page + 1);
            OLED_WriteData(lower);
        }
    }
}

/* 在 (x, y) 显示一个 ASCII 字符，font 取 OLED_6X8 或 OLED_8X16 */
static void oled_spi_show_char(uint8_t x, uint8_t y, char ch, uint8_t font)
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
void oled_spi_show_str(uint8_t x, uint8_t y, const char *str, uint8_t font)
{
    uint8_t width = (font == OLED_8X16) ? 8 : 6;

    while (*str != '\0') {
        if (x > 128 - width) break;         // 这一行放不下了
        oled_spi_show_char(x, y, *str, font);
        x += width;
        str++;
    }
}

void oled_spi_printf(uint8_t x, uint8_t y,uint8_t font, const char *fmt,...){
    char buf[128];
    va_list args;
    va_start(args,fmt);
    vsprintf(buf,fmt,args);
    va_end(args);
    oled_spi_show_str(x,y,buf,font);
}

#endif