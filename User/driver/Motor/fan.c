#include "fan.h"



motor_t fan = {
    .duty = 100,
    .pwm = {
        .duty_p = &fan.duty,
        .channel = TIM_CHANNEL_1,
        .period = 0,
        .htim = &htim3
    },
};

void fan_init(){
    
    pwm_init(&fan.pwm);
}

void fan_set_speed(uint8_t speed){
    /**
     * @brief speed 0-100
     * 
     */
    pwm_set_compare(&fan.pwm,speed/100.0);
}