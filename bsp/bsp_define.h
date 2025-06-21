#ifndef __BSP_DEFINE_H
#define __BSP_DEFINE_H

#include "z_define.h"

// 使用 LED
#define USE_DEVICE_LED
// 使用 KEY
#define USE_DEVICE_KEY
// 使用 IIC
#define USE_DEVICE_IIC
// 使用AT24C02
#define USE_DEVICE_AT24C02
// 使用SPI
#define USE_DEVICE_SPI
// 使用W25Q64
#define USE_DEVICE_W25Q64


#ifndef USE_DEVICE_IIC
#ifdef USE_DEVICE_AT24C02
#undef USE_DEVICE_AT24C02
#endif
#endif

#ifndef USE_DEVICE_SPI
#ifdef USE_DEVICE_W25Q64
#undef USE_DEVICE_W25Q64
#endif
#endif

#endif
