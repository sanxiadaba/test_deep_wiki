#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include <stdint.h>

#include "bsp_port.h"


// GPIO设置为不同模式
// 推挽输出、上拉
void z_gpio_output_pp_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 推挽输出、下拉
void z_gpio_output_pp_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 开漏输出
void z_gpio_output_od(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 设置为输入引脚
void z_gpio_input_set(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 输入上拉
void z_gpio_input_set_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 输入下拉
void z_gpio_input_set_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 读取输入IO的电平
Z_GPIO_STATE z_gpio_read_input(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);


// 读取输出IO的电平
Z_GPIO_STATE z_gpio_read_output(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

// 改变某各引脚电平
void z_gpio_write(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin,Z_GPIO_STATE level);

// 某个引脚，电平翻转
void z_gpio_toggle(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin);

void bsp_gpio_init(void);

void bsp_gpio_deinit(void);


#endif
