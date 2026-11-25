#include "dht11.h"

#define BUS_SEND_TIME 20       // ms
#define SLAVE_RESPONSE_TIME 90 // us

static void data_write(uint8_t bit) {
    gpio_write(DHT11_Port, DHT11_Pin, bit ? 1 : 0);
}
static uint8_t data_read() { return gpio_read(DHT11_Port, DHT11_Pin); }

static uint8_t Read_byte(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        while (data_read() == 0)
            ;         // 等低电平结束
        delay_us(40); // 高电平开始后延时 40µs
        byte <<= 1;
        if (data_read() == 1) { // 采样
            byte |= 1;
        }
        uint32_t timeout = 1000;

        while (data_read() == 1) {
            if (--timeout == 0)
                return 0; // 超时，返回错误
        }; // 等高电平结束
    }
    return byte;
}
static void RST() {
    // 主机起始信号
    data_write(0);
    delay_ms(18);
    data_write(1);
    delay_us(20);
}
static uint8_t Check() {
    // 从机响应信号
    uint32_t start_tick = get_us_tick();
    while (data_read() == 1) {
        uint32_t now_tick = get_us_tick();
        if ((now_tick - start_tick) > SLAVE_RESPONSE_TIME) {
            return 1;
        }
    }
    start_tick = get_us_tick();
    while (data_read() == 0) {
        uint32_t now_tick = get_us_tick();
        if ((now_tick - start_tick) > SLAVE_RESPONSE_TIME) {
            return 1;
        }
    }
    start_tick = get_us_tick();
    while (data_read() == 1) {
        uint32_t now_tick = get_us_tick();
        if ((now_tick - start_tick) > SLAVE_RESPONSE_TIME) {
            return 1;
        }
    }
    return 0;
}
/**
 * @brief DHT11 初始化
 *
 * @return uint8_t 1 失败，0 成功
 */
uint8_t dht11_init() {
    RST();
    return Check();
}
/**
 * @brief 读取温湿度
 *
 * @param temp
 * @param humi
 * @return uint8_t
 */
uint8_t dht11_read(uint8_t *temp, uint8_t *humi) {
    uint8_t buf[5] = {0};
    RST();
    if (Check())
        return 1;
    // 读取数据
    for (uint8_t i = 0; i < 5; i++) {
        buf[i] = Read_byte();
    }
    data_write(1);

    // 校验
    if (buf[4] != (uint8_t)(buf[0] + buf[1] + buf[2] + buf[3]))
        return 1;
    *humi = buf[0];
    *temp = buf[2];
    return 0;
}