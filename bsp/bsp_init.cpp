#include "bsp_init.h"
// bsp初始化

#include "z_include.h"


void bsp_device_init()
{
#ifdef USE_DEVICE_LED
		device_led_init();
#endif
#ifdef USE_DEVICE_KEY
		device_key_init();
#endif
#ifdef USE_DEVICE_IIC
		device_iic_init();
#endif
#ifdef USE_DEVICE_AT24C02
		device_at24c02_init();
#endif
#ifdef USE_DEVICE_SPI
		device_spi_init();
#endif
#ifdef USE_DEVICE_W25Q64
		device_w25q64_init();
#endif
}


// 外设 IO初始化
void bsp_init()
{
    bsp_task_init();
    bsp_flash_init();
		bsp_device_init();
}

