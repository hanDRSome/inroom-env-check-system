#include "bsp_sys.h"

void DWT_Init(){
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// 开启 TRC
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //开启CYCCNT 计数器
}
void delay_us(uint32_t us){
    uint32_t start_tick = DWT->CYCCNT;
    uint32_t delay_time = us * (SystemCoreClock/1000000);
    while((DWT->CYCCNT - start_tick)<=delay_time);
}