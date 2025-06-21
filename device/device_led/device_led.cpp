#include "device_led.h"

#include "z_include.h"

#ifdef USE_DEVICE_LED

// 移植 下面
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_LED_00_GPIO_PORT LED_GPIO_Port
#define DEVICE_LED_00_PIN LED_Pin
#endif
// 移植 上面

// PORT PIN 输出模式 默认电平 激活电平
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
		// 每个周期包含亮灭各一次
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

// port,pin,输出模式 (推挽输出上下拉),激活电平，默认电平
// 移植 下面
DEVICE_LED device_led_00(DEVICE_LED_00_GPIO_PORT, DEVICE_LED_00_PIN, DEVICE_LED::LED_OUTPUT_PP_UP, Z_LEVEL_LOW,Z_LEVEL_HIGH);
// 移植 上面

void device_led_init()
{
// 移植 下面
	 device_led_00.init();
// 移植 上面
}


void device_led_deinit()
{
// 移植 下面
	device_led_00.deinit();
// 移植 上面
}

void device_led_task()
{
// 移植 下面
	device_led_00.task();
// 移植 上面
}

#endif // USE_DEVICE_LED


