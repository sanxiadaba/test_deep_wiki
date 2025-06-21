#ifndef __Z_PORT_H
#define __Z_PORT_H

#include <stdint.h>

// 打印
void __z_port_printf(uint8_t arr[], int32_t size);

// 时钟初始化
void z_clk_init(void);

// 喂狗
void z_wdt_dog_feed(void);

// 任务初始化
void z_bsp_init(void);

// 看门狗初始化
void z_wdt_dog_init(void);

// 复位函数
void z_mcu_reset(void);

#endif
