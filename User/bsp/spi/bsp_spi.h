#pragma once 
#include "bsp_sys.h"


typedef struct {
    uint8_t id;
    gpio_t cs;
}cs_t;// 片选


typedef struct {
    gpio_t sclk;
    gpio_t mosi;
    gpio_t miso;
    cs_t *cs_p;
}spi_t;// my_spi

// 外部调用
void spi_init(spi_t *spi_p,uint8_t cs_num);
void spi_start(spi_t *p,uint8_t id);
void spi_stop(spi_t *p,uint8_t id);
uint8_t spi_swapbyte(spi_t *p,uint8_t cs_id,uint8_t byte_send);

// 单根线的读写（接线自检、特殊时序时直接用）
void sclk_write(spi_t *p,uint8_t bit);
void mosi_write(spi_t *p,uint8_t bit);
uint8_t miso_read(spi_t *p);
void cs_write(cs_t *p,uint8_t bit);



extern spi_t my_spi;
