#include "device_led.h"

#include "z_include.h"

#ifdef USE_DEVICE_LED

// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_LED_00_GPIO_PORT LED_GPIO_Port
#define DEVICE_LED_00_PIN LED_Pin
#endif
// ��ֲ ����

// PORT PIN ���ģʽ Ĭ�ϵ�ƽ �����ƽ
DEVICE_LED::DEVICE_LED(Z_GPIO_PORT port,
                 Z_GPIO_PIN pin,
                 OUTPUT_MODE mode,
                 Z_GPIO_STATE active_level,
                 Z_GPIO_STATE default_level) :
    m_port(port),
    m_pin(pin),
    m_mode(mode),
    m_active_level(active_level),
		m_default_level(default_level),
    m_remaining_cycles(0)
{
		init();
}

void DEVICE_LED::init()
{
    if(m_mode == LED_OUTPUT_PP_UP)
    {
        z_gpio_output_pp_up(m_port, m_pin);
    }
    else if(m_mode == LED_OUTPUT_PP_DOWN)
    {
        z_gpio_output_pp_down(m_port, m_pin);
    }
		set_high_low_level(m_default_level);
		if(m_active_level != m_default_level)
		{
			m_open_close_state = LED_STATE_CLOSED;
		}
		else
		{
			m_open_close_state = LED_STATE_OPEN;
		}
}

void DEVICE_LED::deinit()
{
    if(m_active_level == Z_LEVEL_HIGH)
    {
        z_gpio_input_set_down(m_port, m_pin);
    }
    else
    {
        z_gpio_input_set_up(m_port, m_pin);
    }
}

void DEVICE_LED::open()
{
    set_high_low_level(m_active_level);
		m_open_close_state = LED_STATE_OPEN;
}

void DEVICE_LED::close()
{
    set_high_low_level((Z_GPIO_STATE)(!m_active_level));
		m_open_close_state = LED_STATE_CLOSED;
}

void DEVICE_LED::toggle()
{
    if(m_open_close_state == LED_STATE_OPEN)
    {
        close();
    }
    else
    {
        open();
    }
}

void DEVICE_LED::blink(uint32_t times)
{
		// ÿ�����ڰ��������һ��
    m_remaining_cycles = times * 2;
}

void DEVICE_LED::task()
{
    if(m_remaining_cycles > 0)
    {
        toggle();
				m_remaining_cycles--;
		}
}


DEVICE_LED::STATE_OPEN_CLOSE DEVICE_LED::get_open_close_state()
{
    return m_open_close_state;
}


void DEVICE_LED::set_high_low_level(Z_GPIO_STATE level)
{
    z_gpio_write(m_port, m_pin, level);
}

// port,pin,���ģʽ (�������������),�����ƽ��Ĭ�ϵ�ƽ
// ��ֲ ����
DEVICE_LED device_led_00(DEVICE_LED_00_GPIO_PORT, DEVICE_LED_00_PIN, DEVICE_LED::LED_OUTPUT_PP_UP, Z_LEVEL_LOW,Z_LEVEL_HIGH);
// ��ֲ ����

void device_led_init()
{
// ��ֲ ����
	 device_led_00.init();
// ��ֲ ����
}


void device_led_deinit()
{
// ��ֲ ����
	device_led_00.deinit();
// ��ֲ ����
}

void device_led_task()
{
// ��ֲ ����
	device_led_00.task();
// ��ֲ ����
}

#endif // USE_DEVICE_LED


