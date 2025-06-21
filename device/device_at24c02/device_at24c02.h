#ifndef __DEVICE_AT24C02_H
#define __DEVICE_AT24C02_H

#include "bsp_port.h"

#ifdef USE_DEVICE_AT24C02

#include "device_iic.h"

class DEVICE_AT24C02
{
public:
		// PORT PIN 输出模式 默认电平 激活电平
    DEVICE_AT24C02(uint8_t address_write,
									 uint8_t address_read,
									 Z_GPIO_PORT scl_port,
									 Z_GPIO_PIN  scl_pin,
									 Z_GPIO_PORT sda_port,
									 Z_GPIO_PIN sda_pin);

		typedef void (*call)(void);

		void init(void);
		void deinit(void);
		void scan(void);
		void task(void);

		Z_STATE read(uint8_t start_address,uint8_t buf[],uint8_t length);
		void write(uint8_t start_address,uint8_t buf[],uint8_t length);

		void set_address(uint8_t address_write,uint8_t address_read);
		void set_callback(call write_ok_callback,call write_err_callback);

		DEVICE_IIC device_iic;
		
		Z_STATE write_flag = Z_ERROR;
		
		uint8_t count = 0;
		
private:
	
		Z_GPIO_PORT m_scl_port;
    Z_GPIO_PIN m_scl_pin;
    Z_GPIO_PORT m_sda_port;
    Z_GPIO_PIN m_sda_pin;

		uint8_t m_address_write = 0;
		uint8_t m_address_read = 0;

		uint8_t m_start_address = 0;
		uint8_t *m_buf = NULL;
		uint8_t m_length = 0;
		

		call m_write_ok_callback = NULL;
		call m_write_err_callback = NULL;
};

// 移植 下面
extern DEVICE_AT24C02 device_at24c02_00;
// 移植 上面

void device_at24c02_init(void);
void device_at24c02_deinit(void);
void device_at24c02_task(void);

#endif

#endif
