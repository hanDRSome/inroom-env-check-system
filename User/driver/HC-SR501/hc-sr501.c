#include "hc-sr501.h"

void sr501_init(){
    //上电等待1min

}

uint8_t sr501_read(){
    return gpio_read(SR501_Port,SR501_Pin);
}