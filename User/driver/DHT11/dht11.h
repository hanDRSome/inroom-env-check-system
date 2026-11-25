#pragma once

#include "bsp_sys.h"

#define DHT11_Port dht11_onewire_GPIO_Port
#define DHT11_Pin  dht11_onewire_Pin

uint8_t dht11_init();
uint8_t dht11_read(uint8_t *temp, uint8_t *humi);