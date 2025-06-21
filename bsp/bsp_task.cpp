#include "bsp_task.h"
// 定时任务调度

#include "z_include.h"

// 测试
MultiTimer timer_test;
uint32_t timer_test_time =  1000;
uint32_t timer_test_run_time = 0;

#ifdef USE_DEVICE_LED
// LED
MultiTimer timer_led;
uint32_t timer_led_time =  20;
uint32_t timer_led_run_time = 0;
#endif

#ifdef USE_DEVICE_AT24C02
// AT24C02
MultiTimer timer_at24c02;
uint32_t timer_at24c02_time =  5;
uint32_t timer_at24c02_run_time = 0;
#endif

void bsp_task_init()
{
    // 测试
    multiTimerStart(&timer_test, timer_test_time, timer_test_callback, NULL);
#ifdef USE_DEVICE_LED
		 // LED
    multiTimerStart(&timer_led, timer_led_time, timer_led_callback, NULL);
#endif
#ifdef USE_DEVICE_AT24C02
		 // LED
    multiTimerStart(&timer_at24c02, timer_at24c02_time, timer_at24c02_callback, NULL);
#endif
}


void bsp_task_info_time_show()
{
    z_printf("timer_test_run_time is : %" PRIu32 "ms\r\n",timer_test_run_time);
#ifdef USE_DEVICE_LED
		z_printf("timer_led_run_time is : %" PRIu32 "ms\r\n",timer_led_run_time);
#endif
#ifdef USE_DEVICE_AT24C02
		z_printf("timer_at24c02_run_time is : %" PRIu32 "ms\r\n",timer_at24c02_run_time);
#endif
    z_printf("\r\n");
}


