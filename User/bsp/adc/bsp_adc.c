#include "bsp_adc.h"

void adc_init(adc_t adc){
    if(HAL_ADCEx_Calibration_Start(adc.hadc)!=HAL_OK){
        Error_Handler();
    }//ADC复位校准
}

uint32_t adc_read(adc_t adc,uint32_t time_out){
    HAL_ADC_Start(adc.hadc);  // 启动转换
    if (HAL_ADC_PollForConversion(adc.hadc, time_out) == HAL_OK) {
        return  HAL_ADC_GetValue(adc.hadc);  // 读取结果

    }
}