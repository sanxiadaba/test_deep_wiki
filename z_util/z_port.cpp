#include "z_port.h"
// 基础框架需要移植的

#include <stdio.h>

// 打印
__attribute__((weak))  void __z_port_printf(uint8_t arr[], int32_t size)
{
//	printf("%.*s", size, arr);
}


// 时钟初始化
__attribute__((weak)) void z_clk_init(void)
{

}


// 看门狗初始化
__attribute__((weak)) void z_wdt_dog_init(void)
{

}

// 复位函数
__attribute__((weak)) void z_mcu_reset(void)
{

}

// 喂狗
__attribute__((weak)) void z_wdt_dog_feed(void)
{

}

// BSP初始化
__attribute__((weak)) void z_bsp_init(void)
{

}

