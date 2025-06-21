#include "device_key.h"

#include "z_include.h"


#ifdef USE_DEVICE_KEY


// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_KEY_00_GPIO_PORT KEY0_GPIO_Port
#define DEVICE_KEY_00_PIN 			KEY0_Pin
#endif
// ��ֲ ����

// ��ȡ��ƽ�ĺ���
typedef Z_GPIO_STATE (*read)(void);
// ���� �̰� �ɿ��ص�����
typedef void (*call)(void);

DEVICE_KEY::DEVICE_KEY(read read_func,
                       // Ĭ�ϵ�ƽ
                       Z_GPIO_STATE active_level,
                       // ����״̬�µĵ�ƽ
                       Z_GPIO_STATE default_level,
                       // ����
                       uint16_t debounce_ms = 15,
                       // �̰�ʱ��
                       uint16_t short_press_ms = 50,
                       // ����ʱ��
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

    // ״̬������
    switch (m_state)
    {
    // ����״̬������δ�����£�
    case State::IDLE:
        if (current_state == m_active_level)
        {
            // ��ʼ��������
            m_state = State::DEBOUNCE_DOWN;
            m_debounce_down_start = now;
        }
        break;

    // ��ʼ��������
    case State::DEBOUNCE_DOWN:
        if (current_state != m_active_level)
        {
            m_state = State::IDLE; // ���������ؿ���״̬
        }
        // ����ʱ�����
        else if (now - m_debounce_down_start >= m_debounce_ms)
        {
            // ������� ȷ�ϰ���
            m_state = State::PRESSED;
            m_press_start = now;
            m_long_pressed = Z_ERROR;
        }
        break;

    // ����״̬
    case State::PRESSED:
        if (current_state != m_active_level)
        {
            // �����ͷţ���ʼ�ɿ�����
            m_state = State::DEBOUNCE_UP;
            m_debounce_up_start = now;

            // ����Ƿ�ﵽ�̰�ʱ��
            uint32_t press_duration = now - m_press_start;
            if (m_long_pressed == Z_ERROR && press_duration >= m_short_press_ms)
            {
                // ���Ϊ�̰�
                m_state = State::PRESSED_SHORT;
            }
        }
        else
        {
            // ����Ƿ�ﵽ����ʱ��
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

    // �̰�״̬��������
    case State::PRESSED_SHORT:
        if (current_state != m_active_level)
        {
            if(m_short_press_cb != NULL)
            {
                m_short_press_cb();
            }
            // ��ʼ�ɿ�����
            m_state = State::DEBOUNCE_UP;
            m_debounce_up_start = now;
        }
        break;

    // �ɿ�����
    case State::DEBOUNCE_UP:
        if (current_state == m_active_level)
        {
            // �����ٴΰ��£����ذ�������״̬
            m_state = State::DEBOUNCE_DOWN;
            m_debounce_down_start = now;
        }
        // �ɿ��������
        else if (now - m_debounce_up_start >= m_debounce_ms)
        {
            // �����ɿ��ص�
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


// ��ֲ ����
Z_GPIO_STATE read_key_00()
{
    return z_gpio_read_input(DEVICE_KEY_00_GPIO_PORT,DEVICE_KEY_00_PIN);
}

// ��ȡ��ƽ�����������ƽ��Ĭ�ϵ�ƽ
DEVICE_KEY device_key_00(read_key_00, Z_LEVEL_LOW, Z_LEVEL_HIGH);


// ����
void device_key_00_click()
{
    z_debug("click");
}

// �ɿ�
void device_key_00_release()
{
    z_debug("release");
}

// ��������
void device_key_00_long_press()
{
    z_debug("long press");
}

// ��ֲ ����



void device_key_init(void)
{
// ��ֲ ����
    device_key_00.set_callbacks(device_key_00_click, device_key_00_long_press, device_key_00_release);
// ��ֲ ����
}


void device_key_deinit()
{
// ��ֲ ����

// ��ֲ ����
}

void device_key_task()
{
// ��ֲ ����
    device_key_00.scan();
// ��ֲ ����
}


#endif // USE_DEVICE_KEY


