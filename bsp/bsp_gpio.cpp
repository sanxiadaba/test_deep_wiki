#include "bsp_gpio.h"
// GPIO的一些配置、使用

#include "bsp_port.h"

// GPIO设置为不同模式
// 推挽输出、上拉
void z_gpio_output_pp_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_OUTPUT_PP;
    z_gpio_stu.Pull = GPIO_PULLUP;
    z_gpio_stu.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// 移植 上面
}

// 推挽输出、下拉
void z_gpio_output_pp_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_OUTPUT_PP;
    z_gpio_stu.Pull = GPIO_PULLDOWN;
    z_gpio_stu.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// 移植 上面
}

// 开漏输出
void z_gpio_output_od(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_OUTPUT_OD;
    z_gpio_stu.Pull = GPIO_PULLUP;
    z_gpio_stu.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// 移植 上面
}

// 设置为输入引脚(无上下拉)
void z_gpio_input_set(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_INPUT;
    z_gpio_stu.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif

// 移植 上面
}

// 设置为输入引脚(上拉)
void z_gpio_input_set_up(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_INPUT;
    z_gpio_stu.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// 移植 上面
}

// 设置为输入引脚(下拉)
void z_gpio_input_set_down(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STU z_gpio_stu = {0};
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    z_gpio_stu.Pin = gpio_pin;
    z_gpio_stu.Mode = GPIO_MODE_INPUT;
    z_gpio_stu.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(gpio_port, &z_gpio_stu);
#endif
// 移植 上面
}

// 读取输入IO的电平
Z_GPIO_STATE z_gpio_read_input(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STATE level = Z_LEVEL_LOW;
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    level = HAL_GPIO_ReadPin(gpio_port,gpio_pin);
#endif
// 移植 上面
    return level;
}

// 读取输出IO的电平
Z_GPIO_STATE  z_gpio_read_output(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
    Z_GPIO_STATE level = Z_LEVEL_LOW;
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    level = HAL_GPIO_ReadPin(gpio_port,gpio_pin);
#endif
// 移植 上面
    return level;
}


// 改变某各引脚电平
void z_gpio_write(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin,Z_GPIO_STATE level)
{
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    HAL_GPIO_WritePin(gpio_port,gpio_pin,level);
#endif
// 移植 上面
}

// 某个引脚，电平翻转
void z_gpio_toggle(Z_GPIO_PORT gpio_port,Z_GPIO_PIN gpio_pin)
{
// 移植 下面
// 电平翻转
#ifdef __Z_STM32F103C8T6_H
    HAL_GPIO_TogglePin(gpio_port,gpio_pin);
#endif
// 移植 上面
}


// 一些GPIO配置初始化
void bsp_gpio_init(void)
{

}

void bsp_gpio_deinit()
{

}
