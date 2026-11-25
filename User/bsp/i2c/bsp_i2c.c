#include "bsp_i2c.h"

void scl_write(i2c_t *p, uint8_t bit) {
    gpio_write(p->scl.port, p->scl.pin, bit ? 1 : 0);
}

void sda_write(i2c_t *p, uint8_t bit) {
    gpio_write(p->sda.port, p->sda.pin, bit ? 1 : 0);
}
uint8_t sda_read(i2c_t *p) {
    return gpio_read(p->sda.port, p->sda.pin);
}

void i2c_init(i2c_t *p) {
    scl_write(p, 1);
    sda_write(p, 1);
}

void i2c_start(i2c_t *p) {
    // --_ scl
    // -__ sda
    scl_write(p, 1);
    sda_write(p, 1);
    sda_write(p, 0);
    scl_write(p, 0);
}

void i2c_stop(i2c_t *p) {
    // _-- scl
    // __- sda
    scl_write(p, 0);
    sda_write(p, 0);
    scl_write(p, 1);
    sda_write(p, 1);
}
void i2c_write_byte(i2c_t *p, uint8_t data) {
    for (int i = 0; i < 8; i++) {
        sda_write(p, (data & (0x80 >> i)) ? 1 : 0);
        scl_write(p, 1);
        scl_write(p, 0);
    }
}
uint8_t i2c_read_byte(i2c_t *p) {
    uint8_t data = 0;
    sda_write(p, 1); // Release SDA for reading
    for (int i = 0; i < 8; i++) {
        scl_write(p, 1);
        if (sda_read(p) == 1) {
            data |= (0x80 >> i);
        }
        scl_write(p, 0);
    }
    return data;
}
// 一般不用，MCU做从机是用的
void i2c_send_ack(i2c_t *p, uint8_t ack_bit) {
    sda_write(p, ack_bit ? 1 : 0);
    scl_write(p, 1);
    scl_write(p, 0);
}
uint8_t i2c_read_ack(i2c_t *p) {
    uint8_t ack_bit;
    sda_write(p, 1); // Release SDA for reading
    scl_write(p, 1);
    ack_bit = sda_read(p); 
    scl_write(p, 0);
    return ack_bit;
}
void i2c_write_reg(i2c_t *p, uint8_t slave_addr,uint8_t reg,uint8_t data){
    i2c_start(p);
    i2c_write_byte(p,slave_addr);
    i2c_read_ack(p);
    i2c_write_byte(p,reg);
    i2c_read_ack(p);
    i2c_write_byte(p,data);
    i2c_read_ack(p);
    i2c_stop(p);
}
void i2c_write_regs(i2c_t *p, uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count){
    i2c_start(p);
    i2c_write_byte(p,slave_addr);
    i2c_read_ack(p);
    i2c_write_byte(p,reg);
    i2c_read_ack(p);
    for(uint8_t i=0;i<count;i++){
        i2c_write_byte(p,data_arr[i]);
        i2c_read_ack(p);
    }
    i2c_stop(p);
}
uint8_t i2c_read_reg(i2c_t *p,uint8_t slave_addr,uint8_t reg){
    uint8_t data;
    i2c_start(p);
    i2c_write_byte(p,slave_addr);
    i2c_read_ack(p);
    i2c_write_byte(p,reg);
    i2c_read_ack(p);

    i2c_start(p);
    i2c_write_byte(p,slave_addr+1);
    i2c_read_ack(p);
    data = i2c_read_byte(p);
    i2c_send_ack(p,1);
    i2c_stop(p);
    return data;
}
void i2c_read_regs(i2c_t *p,uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count){
    i2c_start(p);
    i2c_write_byte(p,slave_addr);
    i2c_read_ack(p);
    i2c_write_byte(p,reg);
    i2c_read_ack(p);

    i2c_start(p);
    i2c_write_byte(p,slave_addr+1);
    i2c_read_ack(p);
    for(uint8_t i=0;i<count;i++){
        data_arr[i] = i2c_read_byte(p);
        if(i<(count-1))
            i2c_send_ack(p,0);
        else
            i2c_send_ack(p,1);//最后一次应答
    }
    i2c_stop(p);
}