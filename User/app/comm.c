#include "comm.h"
#include "usart.h"

uint8_t pc_uart_tx_buf[256];
uint8_t pc_uart_rx_buf[256];
uart_t pc_uart = {
    .uart = &huart1,
    .irqn = USART1_IRQn,
    .tx_buf = pc_uart_tx_buf,
    .tx_len = sizeof(pc_uart_tx_buf),
    .rx_buf = pc_uart_rx_buf,
    .rx_len = sizeof(pc_uart_rx_buf),
};

void comm_init(void)
{
    uart_init(&pc_uart);
    HAL_UARTEx_ReceiveToIdle_IT(pc_uart.uart, pc_uart.rx_buf, 256);
    printf("Hello, world!%.2f\r\n",1.22);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    pc_uart.rx_len = Size;
    HAL_UART_Transmit_IT(huart, pc_uart.rx_buf, pc_uart.rx_len);
    HAL_UARTEx_ReceiveToIdle_IT(pc_uart.uart, pc_uart.rx_buf, 256);
}

/* GCC/newlib 下 printf 的底层出口*/
int _write(int file, char *ptr, int len)
{
    (void)file;
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
    return len;
}
