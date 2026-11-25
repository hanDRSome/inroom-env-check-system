#pragma once
#include "bsp_sys.h"


typedef struct {
   gpio_t scl;
   gpio_t sda; 
} i2c_t;

void i2c_init(i2c_t *p);
void i2c_start(i2c_t *p);
void i2c_stop(i2c_t *p);
void i2c_write_byte(i2c_t *p, uint8_t data);
uint8_t i2c_read_byte(i2c_t *p);
void i2c_send_ack(i2c_t *p, uint8_t ack_bit);
uint8_t i2c_read_ack(i2c_t *p);
void i2c_write_reg(i2c_t *p, uint8_t slave_addr,uint8_t reg,uint8_t data);
void i2c_write_regs(i2c_t *p, uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count);
uint8_t i2c_read_reg(i2c_t *p,uint8_t slave_addr,uint8_t reg);
void i2c_read_regs(i2c_t *p,uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count);
/*
   测试从机地址
    i2c_start(&i2c);
    i2c_write_byte(&i2c,0x6b<<1);
    uint8_t ack = i2c_read_ack(&i2c);
    i2c_stop(&i2c);
*/