#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include <stdint.h>

#include "bsp_port.h"


// GPIO����Ϊ��ͬģʽ
// �������������
void z_gpio_output_pp_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// �������������
void z_gpio_output_pp_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// ��©���
void z_gpio_output_od(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// ����Ϊ��������
void z_gpio_input_set(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// ��������
void z_gpio_input_set_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// ��������
void z_gpio_input_set_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// ��ȡ����IO�ĵ�ƽ
Z_GPIO_STATE z_gpio_read_input(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);


// ��ȡ���IO�ĵ�ƽ
Z_GPIO_STATE z_gpio_read_output(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// �ı�ĳ�����ŵ�ƽ
void z_gpio_write(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin,Z_GPIO_STATE level);

// ĳ�����ţ���ƽ��ת
void z_gpio_toggle(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

void bsp_gpio_init(void);

void bsp_gpio_deinit(void);


#endif
