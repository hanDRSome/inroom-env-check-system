#pragma once 
#include "bsp_sys.h"

typedef struct {
    // PWM configuration parameters
    TIM_HandleTypeDef *htim; // Pointer to the timer handle
    uint32_t channel;        // Timer channel for PWM output
    uint32_t period;         // Timer period (ARR value)
    float *duty_p;       // Pointer to the duty cycle variable (0.0 to 1.0)
} pwm_t;

void pwm_init(pwm_t *pwm);
void pwm_set_compare(pwm_t *pwm,float duty);