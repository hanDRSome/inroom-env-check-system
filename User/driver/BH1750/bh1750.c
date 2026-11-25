#include "bh1750.h"

#define PWR_DOWN 0x00
#define PWR_ON 0x01
#define RESET 0x07
#define CONTINUOSUS_H_RESOLUTION 0x10


i2c_t bh1750_i2c = {
    .scl={bh1750_scl_GPIO_Port,bh1750_scl_Pin},
    .sda={bh1750_sda_GPIO_Port,bh1750_sda_Pin}
};

void bh1750_init(){
    //上电
    i2c_start(&bh1750_i2c);
    i2c_write_byte(&bh1750_i2c,BH1750_ADDR);
    i2c_read_ack(&bh1750_i2c);
    i2c_write_byte(&bh1750_i2c,PWR_ON);
    i2c_read_ack(&bh1750_i2c);
    i2c_stop(&bh1750_i2c);
    //连续H-分辨率
    i2c_start(&bh1750_i2c);
    i2c_write_byte(&bh1750_i2c,BH1750_ADDR);
    i2c_read_ack(&bh1750_i2c);
    i2c_write_byte(&bh1750_i2c,CONTINUOSUS_H_RESOLUTION);
    i2c_read_ack(&bh1750_i2c);
    i2c_stop(&bh1750_i2c);
}

uint16_t bh1750_read_data(){
    uint8_t data_arr[2] = {0};
    i2c_start(&bh1750_i2c);
    i2c_write_byte(&bh1750_i2c,BH1750_ADDR|0x01);
    i2c_read_ack(&bh1750_i2c);
    data_arr[0] = i2c_read_byte(&bh1750_i2c);
    i2c_send_ack(&bh1750_i2c,0);
    data_arr[1] = i2c_read_byte(&bh1750_i2c);
    i2c_send_ack(&bh1750_i2c,1);
    i2c_stop(&bh1750_i2c);
    return data_arr[0]<<8|data_arr[1];
}