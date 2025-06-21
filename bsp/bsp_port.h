#ifndef __BSP_PORT_H
#define __BSP_PORT_H

#include <stdint.h>

#include "bsp_define.h"

void __z_port_printf(uint8_t arr[], int32_t size);

void clk_init(void);

void feed_dog(void);

void wdt_dog_init(void);

// 基础头文件、GPIO、时钟
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
#include "main.h"
#include "iwdg.h"
#include "gpio.h"
// GPIO 定义
#define Z_GPIO_PORT GPIO_TypeDef*
#define Z_GPIO_PIN uint16_t
#define Z_GPIO_STATE GPIO_PinState
// 高低电平
#define Z_LEVEL_HIGH	GPIO_PIN_SET
#define Z_LEVEL_LOW	GPIO_PIN_RESET
// GPIO结构体
#define Z_GPIO_STU GPIO_InitTypeDef
// SYSTICK 频率
#define Z_SYSTICK_FREQUENCY_MHZ 72
#endif
// 移植 上面


#endif
