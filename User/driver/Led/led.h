#pragma once
#include "bsp_sys.h"

typedef enum {
    OFF,ON
}LED_STATE;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    LED_STATE state;
} led_t;

#define LED_NUM (4)
extern led_t leds[LED_NUM];
void led_init();
void led_update();