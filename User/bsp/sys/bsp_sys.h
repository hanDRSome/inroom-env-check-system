#pragma once

#include "main.h"
#include "adc.h"
#include "tim.h"

#define gpio_write(port, pin, value) HAL_GPIO_WritePin(port, pin, value)
#define gpio_toggle(port, pin) HAL_GPIO_TogglePin(port, pin)
#define gpio_read(port, pin) HAL_GPIO_ReadPin(port, pin)
#define delay_ms(ms) HAL_Delay(ms)
#define get_ms_tick() HAL_GetTick()
void DWT_Init();
void delay_us(uint32_t us);
#define get_us_tick() (DWT->CYCCNT/(SystemCoreClock/1000000))
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} gpio_t;

#define timer_start(htimp) HAL_TIM_Base_Start_IT(htimp)

#define pwm_start(htim,channel)  HAL_TIM_PWM_Start(htim,channel)
#define pwm_compare_set(htim,channel,duty,period) __HAL_TIM_SetCompare(htim,channel,(uint32_t)(duty*period))