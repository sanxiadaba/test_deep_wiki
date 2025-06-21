#include "bsp_port.h"
// BSP��ֲ

#include "z_include.h"


// debug���
void __z_port_printf(uint8_t arr[], int32_t size)
{
//		bsp_delay_ms(1);
		bsp_delay_us(200);
#ifdef USE_SEGGER_RTT
    SEGGER_RTT_Write(0,arr,size);
#endif
}

// z_ms_callback����
// systick�жϺ���
// ��1ms��systick�жϵ���z_ms_callback
// ��ֲ ����
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
// ��ֲ ����


// ʱ�ӳ�ʼ��
void z_clk_init(void)
{
// ��ֲ ����

// ��ֲ ����
}



// ���Ź���ʼ��
void z_wdt_dog_init()
{
// ��ֲ ����

// ��ֲ ����
}


// ��λ����
void z_mcu_reset()
{
// ��ֲ ����
    NVIC_SystemReset();
// ��ֲ ����
}

// ���Ź�
// Ĭ����1s����һ��
void z_wdt_dog_feed()
{
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    HAL_IWDG_Refresh(&hiwdg);
#endif
// ��ֲ ����
}



// bsp��ʼ��
void z_bsp_init()
{
    bsp_init();
}


