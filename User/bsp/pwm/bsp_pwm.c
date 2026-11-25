#include "bsp_pwm.h"
#include "stm32f1xx_hal_tim.h"


void pwm_init(pwm_t *pwm)
{
    pwm->period = pwm->htim->Init.Period + 1;
    // 设置占空比
    pwm_compare_set(pwm->htim, pwm->channel, *(pwm->duty_p),pwm->period);
    // 启动定时器通道
    pwm_start(pwm->htim, pwm->channel);

}

void pwm_set_compare(pwm_t *pwm,float duty){
    *pwm->duty_p = duty;
    pwm_compare_set(pwm->htim, pwm->channel, *(pwm->duty_p),pwm->period);
}