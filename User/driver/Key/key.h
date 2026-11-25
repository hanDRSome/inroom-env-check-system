#pragma once
#include "bsp_sys.h"

typedef enum {
    UNPRESSED,PRESSED
}KEY_STATE;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    KEY_STATE state;
}key_t;

#define KEY_NUM 4
#define KEY_2 0
#define KEY_3 1
#define KEY_4 2
#define KEY_5 3

#define KEY_HOLD 0x01
#define KEY_UP 0x02
#define KEY_DOWN 0x04
#define KEY_SINGLE 0x08
#define KEY_DOUBLE 0x10
#define KEY_LONG 0x20
#define KEY_REPEAT 0x40

void key_tick();
uint8_t key_check(uint8_t id,uint8_t flag);
