#pragma once
#include "bsp_i2c.h"

typedef struct {
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t gyr_x;
    int16_t gyr_y;
    int16_t gyr_z;
} imu_t;

extern i2c_t imu_i2c;
void imu_write_reg(uint8_t slave_addr,uint8_t reg,uint8_t data);
uint8_t imu_read_reg(uint8_t slave_addr,uint8_t reg);
void imu_write_regs(uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count);
void imu_read_regs(uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count);