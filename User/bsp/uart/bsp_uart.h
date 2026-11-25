#pragma once
#include "bsp_sys.h"

typedef struct uart_t 
{
    UART_HandleTypeDef  *uart;
    IRQn_Type irqn;
    gpio_t *tx_g;
    gpio_t *rx_g;
    uint8_t *tx_buf;
    uint8_t *rx_buf;
    uint16_t tx_len;
    uint16_t rx_len;
} uart_t; 

void uart_init(uart_t *uart);
void uart_send_byte(uart_t *uart, uint8_t byte);
void uart_send_string(uart_t *uart, uint8_t *str);
