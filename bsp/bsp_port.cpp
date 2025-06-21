#include "bsp_port.h"
// BSP移植

#include "z_include.h"


// debug输出
void __z_port_printf(uint8_t arr[], int32_t size)
{
//		bsp_delay_ms(1);
		bsp_delay_us(200);
#ifdef USE_SEGGER_RTT
    SEGGER_RTT_Write(0,arr,size);
#endif
}

// z_ms_callback运行
// systick中断函数
// 在1ms的systick中断调用z_ms_callback
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
void HAL_IncTick(void)
{
    uwTick += uwTickFreq;
    HAL_SYSTICK_IRQHandler();
}

void HAL_SYSTICK_Callback(void)
{
    z_ms_callback();
}

#endif
// 移植 上面


// 时钟初始化
void z_clk_init(void)
{
// 移植 下面

// 移植 上面
}



// 看门狗初始化
void z_wdt_dog_init()
{
// 移植 下面

// 移植 上面
}


// 复位函数
void z_mcu_reset()
{
// 移植 下面
    NVIC_SystemReset();
// 移植 上面
}

// 看门狗
// 默认是1s调用一次
void z_wdt_dog_feed()
{
// 移植 下面
#ifdef __Z_STM32F103C8T6_H
    HAL_IWDG_Refresh(&hiwdg);
#endif
// 移植 上面
}



// bsp初始化
void z_bsp_init()
{
    bsp_init();
}


