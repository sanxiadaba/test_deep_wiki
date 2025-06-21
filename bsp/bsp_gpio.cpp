#include "bsp_gpio.h"
// GPIO��һЩ���á�ʹ��

#include "bsp_port.h"

// GPIO����Ϊ��ͬģʽ
// �������������
void z_gpio_output_pp_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_OUTPUT_PP;
    z_gpio_stu.Pull = GPIO_PULLUP;
    z_gpio_stu.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// ��ֲ ����
}

// �������������
void z_gpio_output_pp_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_OUTPUT_PP;
    z_gpio_stu.Pull = GPIO_PULLDOWN;
    z_gpio_stu.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// ��ֲ ����
}

// ��©���
void z_gpio_output_od(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_OUTPUT_OD;
    z_gpio_stu.Pull = GPIO_PULLUP;
    z_gpio_stu.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// ��ֲ ����
}

// ����Ϊ��������(��������)
void z_gpio_input_set(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_INPUT;
    z_gpio_stu.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif

// ��ֲ ����
}

// ����Ϊ��������(����)
void z_gpio_input_set_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_INPUT;
    z_gpio_stu.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// ��ֲ ����
}

// ����Ϊ��������(����)
void z_gpio_input_set_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_INPUT;
    z_gpio_stu.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// ��ֲ ����
}

// ��ȡ����IO�ĵ�ƽ
Z_GPIO_STATE z_gpio_read_input(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STATE level = Z_LEVEL_LOW;
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    level = HAL_GPIO_ReadPin(gpio_port,gpio_pin);
#endif
// ��ֲ ����
    return level;
}

// ��ȡ���IO�ĵ�ƽ
Z_GPIO_STATE  z_gpio_read_output(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STATE level = Z_LEVEL_LOW;
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    level = HAL_GPIO_ReadPin(gpio_port,gpio_pin);
#endif
// ��ֲ ����
    return level;
}


// �ı�ĳ�����ŵ�ƽ
void z_gpio_write(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin,Z_GPIO_STATE level)
{
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    HAL_GPIO_WritePin(gpio_port,gpio_pin,level);
#endif
// ��ֲ ����
}

// ĳ�����ţ���ƽ��ת
void z_gpio_toggle(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
// ��ֲ ����
// ��ƽ��ת
#ifdef __Z_STM32F103C8T6_H
    HAL_GPIO_TogglePin(gpio_port,gpio_pin);
#endif
// ��ֲ ����
}


// һЩGPIO���ó�ʼ��
void bsp_gpio_init(void)
{

}

void bsp_gpio_deinit()
{

}
