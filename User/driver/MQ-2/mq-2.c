#include "mq-2.h"
#include "math.h"
adc_t mq_2_adc = {
    &hadc1
};

void mq_2_init(){
    adc_init(mq_2_adc);
}

float mq_2_read(){
    float temp=0;
    temp = adc_read(mq_2_adc,50);
    float vol = (temp*5/4096);
    float rs = (MQ2_VCC-vol)/(vol*0.5);
    float r0 = 6.64;

    //rs/r0 = 11.5428*ppm^(-0.6549)
    return pow(11.5428*r0/rs,0.6549f);
}