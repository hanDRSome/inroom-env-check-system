#include "timer.h"


void timer_init(){
    timer_start(&htim2);  // 启动定时器并使能更新中断
}


