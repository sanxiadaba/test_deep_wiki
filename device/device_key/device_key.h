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
        PRESSED_SHORT,  // �������̰�״̬
        DEBOUNCE_UP
    };

    // ��ȡ��ƽ�ĺ���
    typedef Z_GPIO_STATE (*read)(void);
    // ���� �̰� �ɿ��ص�����
    typedef void (*call)(void);

    DEVICE_KEY(read read_func,
               // Ĭ�ϵ�ƽ
               Z_GPIO_STATE active_level,
               // ����״̬�µĵ�ƽ
               Z_GPIO_STATE default_level,
               // ����
               uint16_t debounce_ms,
               // �̰�ʱ��
               uint16_t short_press_ms,
               // ����ʱ��
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
    call m_release_cb = NULL;  // �ɿ��ص�����ָ��
};

void device_key_init(void);
void device_key_deinit(void);
void device_key_task(void);

#endif

#endif