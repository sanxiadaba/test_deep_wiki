#ifndef __DEVICE_LED_H
#define __DEVICE_LED_H

#include "bsp_port.h"


#ifdef USE_DEVICE_LED

class DEVICE_LED
{
public:
		// ��������������
    enum OUTPUT_MODE
    {
        LED_OUTPUT_PP_UP,
        LED_OUTPUT_PP_DOWN
    };
		
		// LED�Ƿ��
		enum STATE_OPEN_CLOSE
    {
        LED_STATE_CLOSED,
        LED_STATE_OPEN
    };
		
		// PORT PIN ���ģʽ Ĭ�ϵ�ƽ �����ƽ
    DEVICE_LED(Z_GPIO_PORT port,
            Z_GPIO_PIN pin,
            OUTPUT_MODE mode,
            Z_GPIO_STATE active_level,
            Z_GPIO_STATE default_level);
	
		void init();
    void deinit();
		void open();
    void close();
    void toggle();
    void blink(uint32_t times);
    void task();
		// LED��open����close
    STATE_OPEN_CLOSE get_open_close_state();
		
private:
		// ����LED�ĵ�ƽ
    void set_high_low_level(Z_GPIO_STATE high_low_level);
    Z_GPIO_PORT m_port;
    Z_GPIO_PIN m_pin;
    OUTPUT_MODE m_mode;
    Z_GPIO_STATE m_default_level;
    Z_GPIO_STATE m_active_level;
    STATE_OPEN_CLOSE m_open_close_state;
    uint32_t m_remaining_cycles;
};


void device_led_init(void);

void device_led_deinit(void);

void device_led_task(void);

// ��ֲ ����
extern DEVICE_LED device_led_00;
// ��ֲ ����

#endif

#endif
