#ifndef __DEVICE_KEY_H
#define __DEVICE_KEY_H

#include "bsp_port.h"

uint64_t z_systick_get(void);

#ifdef USE_DEVICE_KEY

class DEVICE_KEY
{
public:
    enum class State
    {
        IDLE,
        DEBOUNCE_DOWN,
        PRESSED,
        PRESSED_SHORT,  // 新增：短按状态
        DEBOUNCE_UP
    };

    // 读取电平的函数
    typedef Z_GPIO_STATE (*read)(void);
    // 长按 短按 松开回调函数
    typedef void (*call)(void);

    DEVICE_KEY(read read_func,
               // 默认电平
               Z_GPIO_STATE active_level,
               // 激活状态下的电平
               Z_GPIO_STATE default_level,
               // 消抖
               uint16_t debounce_ms,
               // 短按时间
               uint16_t short_press_ms,
               // 长按时间
               uint16_t long_press_ms);

    void scan();

    void set_callbacks(call short_cb,call long_cb,call release_cb);

private:
    read m_read_func;
    Z_GPIO_STATE m_default_level;
    Z_GPIO_STATE m_active_level;
    uint16_t m_debounce_ms;
    uint16_t m_short_press_ms;
    uint16_t m_long_press_ms;

    State m_state = State::IDLE;
    uint64_t m_debounce_down_start = 0;
    uint64_t m_press_start = 0;
    uint64_t m_debounce_up_start = 0;
    Z_STATE m_long_pressed = Z_ERROR;

    call m_short_press_cb = NULL;
    call m_long_press_cb = NULL;
    call m_release_cb = NULL;  // 松开回调函数指针
};

void device_key_init(void);
void device_key_deinit(void);
void device_key_task(void);

#endif

#endif