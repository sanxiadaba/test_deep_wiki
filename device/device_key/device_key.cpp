#include "device_key.h"

#include "z_include.h"


#ifdef USE_DEVICE_KEY


// 移植 下面
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_KEY_00_GPIO_PORT KEY0_GPIO_Port
#define DEVICE_KEY_00_PIN 			KEY0_Pin
#endif
// 移植 上面

// 读取电平的函数
typedef Z_GPIO_STATE (*read)(void);
// 长按 短按 松开回调函数
typedef void (*call)(void);

DEVICE_KEY::DEVICE_KEY(read read_func,
                       // 默认电平
                       Z_GPIO_STATE active_level,
                       // 激活状态下的电平
                       Z_GPIO_STATE default_level,
                       // 消抖
                       uint16_t debounce_ms = 15,
                       // 短按时间
                       uint16_t short_press_ms = 50,
                       // 长按时间
                       uint16_t long_press_ms = 1000)
    : m_read_func(read_func),
			m_active_level(active_level),
      m_default_level(default_level),
      m_debounce_ms(debounce_ms),
      m_short_press_ms(short_press_ms),
      m_long_press_ms(long_press_ms)
{

}

void DEVICE_KEY::scan()
{
    Z_GPIO_STATE current_state = m_read_func();
    uint64_t now = z_systick_get();

    // 状态机处理
    switch (m_state)
    {
    // 空闲状态（按键未被按下）
    case State::IDLE:
        if (current_state == m_active_level)
        {
            // 开始按下消抖
            m_state = State::DEBOUNCE_DOWN;
            m_debounce_down_start = now;
        }
        break;

    // 开始按下消抖
    case State::DEBOUNCE_DOWN:
        if (current_state != m_active_level)
        {
            m_state = State::IDLE; // 抖动，返回空闲状态
        }
        // 消抖时间计算
        else if (now - m_debounce_down_start >= m_debounce_ms)
        {
            // 消抖完成 确认按下
            m_state = State::PRESSED;
            m_press_start = now;
            m_long_pressed = Z_ERROR;
        }
        break;

    // 按下状态
    case State::PRESSED:
        if (current_state != m_active_level)
        {
            // 按键释放，开始松开消抖
            m_state = State::DEBOUNCE_UP;
            m_debounce_up_start = now;

            // 检查是否达到短按时间
            uint32_t press_duration = now - m_press_start;
            if (m_long_pressed == Z_ERROR && press_duration >= m_short_press_ms)
            {
                // 标记为短按
                m_state = State::PRESSED_SHORT;
            }
        }
        else
        {
            // 检查是否达到长按时间
            uint32_t press_duration = now - m_press_start;
            if (m_long_pressed == Z_ERROR && press_duration >= m_long_press_ms)
            {
                if (m_long_press_cb  != NULL)
                {
                    m_long_press_cb();
                }
                m_long_pressed = Z_OK;
            }
        }
        break;

    // 短按状态（新增）
    case State::PRESSED_SHORT:
        if (current_state != m_active_level)
        {
            if(m_short_press_cb != NULL)
            {
                m_short_press_cb();
            }
            // 开始松开消抖
            m_state = State::DEBOUNCE_UP;
            m_debounce_up_start = now;
        }
        break;

    // 松开消抖
    case State::DEBOUNCE_UP:
        if (current_state == m_active_level)
        {
            // 按键再次按下，返回按下消抖状态
            m_state = State::DEBOUNCE_DOWN;
            m_debounce_down_start = now;
        }
        // 松开消抖完成
        else if (now - m_debounce_up_start >= m_debounce_ms)
        {
            // 触发松开回调
            if (m_release_cb != NULL)
            {
                m_release_cb();
            }

            m_state = State::IDLE;
        }
        break;
    }
}

void DEVICE_KEY::set_callbacks(call short_cb = NULL,
                               call long_cb = NULL,
                               call release_cb = NULL)
{
    m_short_press_cb = short_cb;
    m_long_press_cb = long_cb;
    m_release_cb = release_cb;
}


// 移植 下面
Z_GPIO_STATE read_key_00()
{
    return z_gpio_read_input(DEVICE_KEY_00_GPIO_PORT,DEVICE_KEY_00_PIN);
}

// 读取电平函数，激活电平，默认电平
DEVICE_KEY device_key_00(read_key_00, Z_LEVEL_LOW, Z_LEVEL_HIGH);


// 按下
void device_key_00_click()
{
    z_debug("click");
}

// 松开
void device_key_00_release()
{
    z_debug("release");
}

// 长按触发
void device_key_00_long_press()
{
    z_debug("long press");
}

// 移植 上面



void device_key_init(void)
{
// 移植 下面
    device_key_00.set_callbacks(device_key_00_click, device_key_00_long_press, device_key_00_release);
// 移植 上面
}


void device_key_deinit()
{
// 移植 下面

// 移植 上面
}

void device_key_task()
{
// 移植 下面
    device_key_00.scan();
// 移植 上面
}


#endif // USE_DEVICE_KEY


