#include "qmi8658.h"
#include "qmi8658_reg.h"

void qmi8658_init(){
    imu_write_reg(QMI8658_ADDR,CTRL1,0x60);//地址自增，传感器使能
    imu_write_reg(QMI8658_ADDR,CTRL2,0x33);//acc 16g量程,896.8hz速率
    imu_write_reg(QMI8658_ADDR,CTRL3,0x73);//gyr 2048dps 896.8hz
    imu_write_reg(QMI8658_ADDR,CTRL5,0x01);//acc开启滤波
    imu_write_reg(QMI8658_ADDR,CTRL7,0x03);//开启acc gyro
}
uint8_t qmi8658_get_id(){
    return imu_read_reg(QMI8658_ADDR,WHO_AM_I);
}
void qmi8658_get_data(imu_t *p){
    uint8_t data[12];
    imu_read_regs(QMI8658_ADDR,AX_L,data,12);
    p->acc_x = data[0]  | data[1]<<8;
    p->acc_y = data[2]  | data[3]<<8;
    p->acc_z = data[4]  | data[5]<<8;
    p->gyr_x = data[6]  | data[7]<<8;
    p->gyr_y = data[8]  | data[9]<<8;
    p->gyr_z = data[10] | data[11]<<8;
}