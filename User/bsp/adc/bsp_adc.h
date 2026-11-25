#pragma once 

#include "bsp_sys.h"

typedef struct{
    ADC_HandleTypeDef* hadc;
} adc_t;

void adc_init(adc_t adc);
uint32_t adc_read(adc_t adc,uint32_t time_out);