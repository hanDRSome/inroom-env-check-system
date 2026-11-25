#include "bsp_uart.h"

void uart_init(uart_t *uart)
{
}

void uart_send_byte(uart_t *uart, uint8_t byte)
{
    HAL_UART_Transmit(uart->uart, &byte, 1,HAL_MAX_DELAY);
}

void uart_send_string(uart_t *uart, uint8_t *str)
{
    uart->tx_buf = str;
    uart->tx_len = 0;
    while (*(str + uart->tx_len) != '\0')
    {
        uart->tx_len++;
    }
    HAL_UART_Transmit(uart->uart, uart->tx_buf, uart->tx_len,HAL_MAX_DELAY);
}
