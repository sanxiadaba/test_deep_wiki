#ifndef __BSP_INCLUDE_H
#define __BSP_INCLUDE_H

#include "bsp_define.h"

#include "bsp_init.h"
#include "bsp_port.h"
#include "bsp_task.h"
#include "bsp_task_callback.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "bsp_flash.h"


// 下面是一些device
#ifdef USE_DEVICE_LED
#include "device_led.h"
#endif
#ifdef USE_DEVICE_KEY
#include "device_key.h"
#endif
#ifdef USE_DEVICE_IIC
#include "device_iic.h"
#endif
#ifdef USE_DEVICE_AT24C02
#include "device_at24c02.h"
#endif
#ifdef USE_DEVICE_SPI
#include "device_spi.h"
#endif
#ifdef USE_DEVICE_W25Q64
#include "device_w25q64.h"
#endif

#endif
