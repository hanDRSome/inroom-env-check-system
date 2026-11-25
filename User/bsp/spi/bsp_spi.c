#include "bsp_spi.h"
#include "bsp_sys.h"


//
// spi_t my_spi = {
//     .sclk.port = my_spi_sclk_GPIO_Port,.sclk.pin = my_spi_sclk_Pin,
//     .mosi.port = my_spi_mosi_GPIO_Port,.mosi.pin = my_spi_mosi_Pin,
//     .miso.port = my_spi_miso_GPIO_Port,.miso.pin = my_spi_miso_Pin,
// };

void sclk_write(spi_t *p,uint8_t bit){
    gpio_write(p->sclk.port,p->sclk.pin,bit?GPIO_PIN_SET:GPIO_PIN_RESET);
}

void mosi_write(spi_t *p,uint8_t bit){  
    gpio_write(p->mosi.port,p->mosi.pin,bit?GPIO_PIN_SET:GPIO_PIN_RESET);
}

uint8_t miso_read(spi_t *p){
    return gpio_read(p->miso.port,p->miso.pin);
}
/**
    这个是对不同从机写入
*/
void cs_write(cs_t *p,uint8_t bit){
    gpio_write(p->cs.port,p->cs.pin,bit?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
    spi软件初始化
*/

void spi_init(spi_t *p,uint8_t cs_num){
    for(uint8_t i=0;i < cs_num;i++){
        cs_write(&p->cs_p[i],1);
    }
    sclk_write(p,0);
}

void spi_start(spi_t *p,uint8_t id){
    cs_write(&p->cs_p[id],0);
}

void spi_stop(spi_t *p,uint8_t id){
    cs_write(&p->cs_p[id],1);
}

uint8_t spi_swapbyte(spi_t *p,uint8_t cs_id,uint8_t byte_send){
    uint8_t byte_receive = 0;
    for(uint8_t i =0;i<8;i++){
        mosi_write(p,byte_send & (0x80>>i));
        sclk_write(p,1);
        byte_receive |= (miso_read(p)>0)?(0x80>>i):0;
        sclk_write(p,0);
    }
    return byte_receive;
}

/**
    example 
    cs_t my_cs[10] = {....};
    spi_t my_spi = {....,cs_p=my_cs}
    spi_t_init();
*/