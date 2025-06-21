#include "bsp_task_callback.h"
// 定时任务回调

#include "z_include.h"

void timer_test_callback(MultiTimer* timer, void* userData)
{
		uint8_t data[128] = {0};
		static uint8_t flag = 0;
    uint64_t start_time = z_systick_get();
//		bsp_flash_test();
    z_debug("hello world\r\n");
#ifdef USE_DEVICE_LED
		device_led_00.blink(5);
#endif

#ifdef USE_DEVICE_IIC
//		device_iic_00.scan();
#endif
	
#ifdef USE_DEVICE_AT24C02
//		device_at24c02_00.scan();
//		if(flag)
//		{
//			z_fori(0,128)
//			{
//				data[i] = i;
//			}
//			device_at24c02_00.write(0,data,128);
//		}
//		else
//		{
//			memset(data,0,128);
//			Z_STATE state = device_at24c02_00.read(0,data,128);
//			if(state == Z_OK)
//			{
//				z_printf("read ok\r\n");
//				z_printf_uint8_arr(data,128);
//			}
//		}
		
#endif

#ifdef USE_DEVICE_SPI
//	uint8_t test_spi[8] = {1,2,3,4,5,6,7,8};
//	device_4spi_00.active_cs();
//	device_4spi_00.transfer_8bit_bulk(test_spi,NULL,8);
//	device_4spi_00.release_cs();

//	uint8_t test_spi[8] = {1,2,3,4,5,6,7,8};
//	device_3spi_00.active_cs();
//	device_3spi_00.transfer_8bit_bulk(test_spi,8);
//	device_3spi_00.release_cs();
#endif

#ifdef USE_DEVICE_W25Q64
		device_w25q64_00.test();
#endif
		
		flag = !flag;
//		z_test_all();
    // restart
    multiTimerStart(timer, timer_test_time, timer_test_callback, NULL);
    timer_test_run_time = z_systick_get() - start_time;
}


#ifdef USE_DEVICE_LED
void timer_led_callback(MultiTimer* timer, void* userData)
{
    uint64_t start_time = z_systick_get();

		device_led_task();

    // restart
    multiTimerStart(timer, timer_led_time, timer_led_callback, NULL);
    timer_led_run_time = z_systick_get() - start_time;
}
#endif

#ifdef USE_DEVICE_AT24C02
void timer_at24c02_callback(MultiTimer* timer, void* userData)
{
    uint64_t start_time = z_systick_get();

		device_at24c02_task();

    // restart
    multiTimerStart(timer, timer_at24c02_time, timer_at24c02_callback, NULL);
    timer_at24c02_run_time = z_systick_get() - start_time;
}
#endif

