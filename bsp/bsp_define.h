#ifndef __BSP_DEFINE_H
#define __BSP_DEFINE_H

#include "z_define.h"

// ʹ�� LED
#define USE_DEVICE_LED
// ʹ�� KEY
#define USE_DEVICE_KEY
// ʹ�� IIC
#define USE_DEVICE_IIC
// ʹ��AT24C02
#define USE_DEVICE_AT24C02
// ʹ��SPI
#define USE_DEVICE_SPI
// ʹ��W25Q64
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
