#include "imu.h"


// i2c_t imu_i2c = {
//     {imu_scl_GPIO_Port,imu_scl_Pin},
//     {imu_sda_GPIO_Port,imu_sda_Pin}
// };

void imu_write_reg(uint8_t slave_addr,uint8_t reg,uint8_t data){
    i2c_write_reg(&imu_i2c,slave_addr,reg,data);
}
uint8_t imu_read_reg(uint8_t slave_addr,uint8_t reg){
    return i2c_read_reg(&imu_i2c,slave_addr,reg);
}
void imu_write_regs(uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count){
    i2c_write_regs(&imu_i2c,slave_addr,reg,data_arr,count);
}
void imu_read_regs(uint8_t slave_addr,uint8_t reg,uint8_t* data_arr,uint8_t count){
    i2c_read_regs(&imu_i2c,slave_addr,reg,data_arr,count);
}