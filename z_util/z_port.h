#ifndef __Z_PORT_H
#define __Z_PORT_H

#include <stdint.h>

// ��ӡ
void __z_port_printf(uint8_t arr[], int32_t size);

// ʱ�ӳ�ʼ��
void z_clk_init(void);

// ι��
void z_wdt_dog_feed(void);

// �����ʼ��
void z_bsp_init(void);

// ���Ź���ʼ��
void z_wdt_dog_init(void);

// ��λ����
void z_mcu_reset(void);

#endif
