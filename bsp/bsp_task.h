#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "MultiTimer.h"

#include "bsp_port.h"

#include <stdint.h>

void bsp_task_init(void);

// ≤‚ ‘
extern MultiTimer timer_test;
extern uint32_t timer_test_time;
extern uint32_t timer_test_run_time;

#ifdef USE_DEVICE_LED
// LED
extern MultiTimer timer_led;
extern uint32_t timer_led_time;
extern uint32_t timer_led_run_time;
#endif

#ifdef USE_DEVICE_AT24C02
// AT24C02
extern MultiTimer timer_at24c02;
extern uint32_t timer_at24c02_time;
extern uint32_t timer_at24c02_run_time;
#endif

void bsp_task_info_time_show(void);


#endif
