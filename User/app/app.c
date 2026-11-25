#include "bh1750.h"
#include "comm.h"
#include "dht11.h"
#include "fan.h"
#include "hc-sr501.h"
#include "key.h"
#include "led.h"
#include "mq-2.h"
#include "oled_i2c.h"
#include "timer.h"

void setup(void) {
    oled_i2c_soft_init();
    oled_i2c_clear();
    mq_2_init();
    sr501_init();
    dht11_init();
    bh1750_init();
    fan_init();
    led_init();
    timer_init();
    oled_i2c_printf(28, 24, 8, "Loading!");

    delay_ms(2000);
    oled_i2c_clear();
}
#define SMOKE_threshold_VAL 800

uint8_t temp, humi, people_state;
uint8_t fan_speed, fan_mode, fan_blink_flag; // 0-100 0 自动， 1 手动
float smoke_val, light_level;
uint32_t start_time;
uint8_t led_flag, led2_blink_flag, led1_mode;
uint8_t dht11_flag, ren_flag, mq2_flag, bh1750_flag;
void run(void) {
    // 按键检测
    if (key_check(KEY_2, KEY_SINGLE)) {
        fan_mode = !fan_mode;
        led1_mode = !led1_mode;
        leds[0].state = ON;
    }
    if (led1_mode) {
        if (key_check(KEY_5, KEY_SINGLE)) {
            leds[0].state = !leds[0].state;
        }
    }
    if (fan_mode) {
        if (key_check(KEY_3, KEY_SINGLE)) {
            fan_speed += 10;
            if (fan_speed >= 100) {
                fan_speed = 100;
            }
        }
        if (key_check(KEY_4, KEY_SINGLE)) {
            if (fan_speed <= 10) {
                fan_speed = 0;
            } else {
                fan_speed -= 10;
            }
        }
    }
    // 红外人体检测
    if (ren_flag) {
        ren_flag = 0;
        people_state = sr501_read();
        if (led1_mode == 0) {
            if (people_state) {
                start_time = get_ms_tick();
                leds[0].state = ON;
            } else {
                if ((get_ms_tick() - start_time) > 3000) {
                    leds[0].state = OFF;
                } else {
                    leds[0].state = ON;
                }
            }
        }
    }
    // 烟雾检测
    if (mq2_flag) {
        mq2_flag = 0;
        smoke_val = mq_2_read();
        if (smoke_val >= SMOKE_threshold_VAL) {
            led2_blink_flag = 1;
        }
    }

    // 温湿度检测
    if (dht11_flag) {
        dht11_flag = 0;
        dht11_read(&temp, &humi);
        if (fan_mode == 0) {
            fan_speed = (temp - 15) * 10;
        }
    }
    // 光强
    if (bh1750_flag) {
        light_level = bh1750_read_data() / 1.2f;
    }

    // led更新
    if (led_flag) {
        led_flag = 0;
        led_update();
    }
    // fan更新
    fan_set_speed(fan_speed);

    oled_i2c_printf(0, 0, 6, "mq=%5.2fppm", smoke_val);
    oled_i2c_printf(0, 8, 6, "Tp=%5d", people_state);

    oled_i2c_printf(0, 16, 6, "t =%5d c", temp);
    oled_i2c_printf(0, 24, 6, "h =%5d %%", humi);
    oled_i2c_printf(0, 32, 6, "LL=%5.2fLux", light_level);
    if (fan_mode == 0) {
        oled_i2c_printf(0, 48, 6, "auto   fan");
        oled_i2c_printf(0, 40, 6, "FS=%5d", fan_speed);

    } else {
        oled_i2c_printf(0, 48, 6, "handed fan");
        if (fan_blink_flag == 1) {
            oled_i2c_printf(0, 40, 6, "FS=     ");
        } else {
            oled_i2c_printf(0, 40, 6, "FS=%5d", fan_speed);
        }
    }
}

uint32_t time_count = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        time_count = (time_count + 1) % 10000; // 0-10s
        if (!(time_count % 200)) {
            mq2_flag = 1;
            bh1750_flag = 1;
            led_flag = 1;
            ren_flag = 1;
            leds[3].state = !leds[3].state;
        }
        if (!(time_count % 2000)) {
            dht11_flag = 1;
        }
        if (fan_mode) {
            if (!(time_count % 500)) {
                fan_blink_flag = !fan_blink_flag;
            }
        }
        if (led2_blink_flag) {
            if (time_count % 200 == 0) {
                leds[1].state = !leds[1].state;
            }
        } else {
            leds[1].state = OFF;
        }
        key_tick();
    }
}