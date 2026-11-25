#pragma once

#include "bsp_pwm.h"


typedef struct {
    float duty;
    pwm_t pwm;
} motor_t;

void fan_init();
void fan_set_speed(uint8_t speed);