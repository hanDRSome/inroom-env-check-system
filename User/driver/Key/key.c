#include "key.h"

#define TIME_LONG_TIME 2000
#define TIME_DOUBLE_TIME 200
#define TIME_REPEAT_TIME 100

uint8_t key_flag[KEY_NUM] = {0};

key_t keys[KEY_NUM] = {
    {
        .port = GPIOB,
        .pin = GPIO_PIN_4,
    },
    {
        .port = GPIOB,
        .pin = GPIO_PIN_5,
    },
    {
        .port = GPIOA,
        .pin = GPIO_PIN_15,
    },
    {
        .port = GPIOB,
        .pin = GPIO_PIN_3,
    },
};

/**
 * @brief read key
 *
 * @param id
 * @return uint8_t
 */
static uint8_t key_read(uint8_t id) {
    return gpio_read(keys[id].port, keys[id].pin);
}

static KEY_STATE key_get_state(uint8_t id) {
    if (key_read(id) == 0) {
        return PRESSED;
    } else {
        return UNPRESSED;
    }
}
/**
 * @brief 1 tick key loop scan
 *
 */
void key_tick() {
    static uint8_t count = 0;
    static KEY_STATE pre_state[KEY_NUM];
    static KEY_STATE cur_state[KEY_NUM];
    static uint8_t s[KEY_NUM];
    static uint16_t time[KEY_NUM];
    for (uint8_t i = 0; i < KEY_NUM; i++) {
        if (time[i] > 0) {
            time[i]--;
        }
    }
    count++;
    if (count == 20) {
        count = 0;
        for (uint8_t i = 0; i < KEY_NUM; i++) {

            pre_state[i] = cur_state[i];
            cur_state[i] = key_get_state(i);

            if (cur_state[i] == PRESSED) {
                key_flag[i] |= KEY_HOLD;
            } else {
                key_flag[i] &= ~KEY_HOLD;
            }
            if (cur_state[i] == PRESSED && pre_state[i] == UNPRESSED) {
                key_flag[i] |= KEY_DOWN;
            } else if (cur_state[i] == UNPRESSED && pre_state[i] == PRESSED) {
                key_flag[i] |= KEY_UP;
            }
            switch (s[i]) {
            case 0:
                if (cur_state[i] == PRESSED) {
                    s[i] = 1;
                    time[i] = TIME_LONG_TIME;
                }
                break;
            case 1:
                if (cur_state[i] == UNPRESSED) {
                    s[i] = 2;
                    time[i] = TIME_DOUBLE_TIME;
                } else if (time[i] == 0) {
                    // 长按
                    key_flag[i] |= KEY_LONG;
                    s[i] = 4;
                    time[i] = TIME_REPEAT_TIME;
                }
                break;
            case 2:
                if (time[i] == 0) {
                    // 单击
                    key_flag[i] |= KEY_SINGLE;
                    s[i] = 0;
                } else if (cur_state[i] == PRESSED) {
                    // 双击
                    key_flag[i] |= KEY_DOUBLE;
                    s[i] = 3;
                }
                break;
            case 3:
                if (cur_state[i] == UNPRESSED) {
                    s[i] = 0;
                }
                break;
            case 4:
                if (time[i] == 0) {
                    s[i] = 0;
                } else {
                    // repeat long
                    key_flag[i] |= KEY_REPEAT;
                    time[i] = TIME_REPEAT_TIME;
                }
                break;
            }
        }
    }
}
/**
 * @brief  key check
 * @param id
 * @param flag
 * @return uint8_t
 */
uint8_t key_check(uint8_t id, uint8_t flag) {
    if (key_flag[id] & flag) {
        if (flag != KEY_HOLD) {
            key_flag[id] &= ~flag;
        }
        return 1;
    }
    return 0;
}