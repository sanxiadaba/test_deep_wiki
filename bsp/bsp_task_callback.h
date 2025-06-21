#ifndef __BSP_TASK_CALLBACK_H
#define __BSP_TASK_CALLBACK_H

#include <stdint.h>

#include "MultiTimer.h"

#include "bsp_port.h"

void timer_test_callback(MultiTimer* timer, void* userData);


#ifdef USE_DEVICE_LED
void timer_led_callback(MultiTimer* timer, void* userData);
#endif

#ifdef USE_DEVICE_AT24C02
void timer_at24c02_callback(MultiTimer* timer, void* userData);
#endif

#endif
