#include "led.h"

led_t leds[LED_NUM] = {
    {.port = GPIOB, .pin = GPIO_PIN_6,},
    {.port = GPIOB, .pin = GPIO_PIN_7,},
    {.port = GPIOB, .pin = GPIO_PIN_8,},
    {.port = GPIOB, .pin = GPIO_PIN_9,},
};
void led_init() {   

    for (uint8_t i = 0; i < LED_NUM; i++) {
        leds[i].state = OFF;
        gpio_write(leds[i].port, leds[i].pin, (GPIO_PinState)(!leds[i].state));
    }
}

void led_update() {
    for (uint8_t i = 0; i < LED_NUM; i++) {
        gpio_write(leds[i].port, leds[i].pin, (GPIO_PinState)(!leds[i].state));
    }
}