#pragma once
#include "imu.h"

#define QMI8658_ADDR (0x6b<<1)

void qmi8658_init();
uint8_t qmi8658_get_id();
void qmi8658_get_data(imu_t *p);
