#include "z_port.h"
// ���������Ҫ��ֲ��

#include <stdio.h>

// ��ӡ
__attribute__((weak))  void __z_port_printf(uint8_t arr[], int32_t size)
{
//	printf("%.*s", size, arr);
}


// ʱ�ӳ�ʼ��
__attribute__((weak)) void z_clk_init(void)
{

}


// ���Ź���ʼ��
__attribute__((weak)) void z_wdt_dog_init(void)
{

}

// ��λ����
__attribute__((weak)) void z_mcu_reset(void)
{

}

// ι��
__attribute__((weak)) void z_wdt_dog_feed(void)
{

}

// BSP��ʼ��
__attribute__((weak)) void z_bsp_init(void)
{

}

