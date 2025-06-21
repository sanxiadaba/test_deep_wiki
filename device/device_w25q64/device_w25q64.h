#ifndef __DEVICE_W25Q64_H
#define __DEVICE_W25Q64_H

#include "bsp_port.h"

#ifdef USE_DEVICE_W25Q64

#include "device_spi.h"

class DEVICE_W25Q64
{
public:
    DEVICE_W25Q64(Z_GPIO_PORT sclk_port,
								  Z_GPIO_PIN  sclk_pin,
								  Z_GPIO_PORT mosi_port,
								  Z_GPIO_PIN mosi_pin,
								  Z_GPIO_PORT miso_port,
								  Z_GPIO_PIN miso_pin,
								  Z_GPIO_PORT cs_port,
								  Z_GPIO_PIN cs_pin);

		typedef enum
		{
				OP_READ,
				OP_PAGE_WRITE,
				OP_SECTOR_ERASE,
				OP_BLOCK_ERASE,
				OP_CHIP_ERASE
		} W25Q64_OperationType;

		void init(void);
		void deinit(void);
		
		// 数据读取
		Z_STATE read_data(uint32_t address, uint8_t *data, uint32_t data_size);
		// 数据写入
		Z_STATE write_data(uint32_t address, uint8_t *data, uint32_t data_size);
		// 测试
		void test(void);
		
		DEVICE_4SPI device_4spi;
		// 下面是辅助函数
		
		Z_STATE write_enable(void);
		
		Z_STATE verify_erase(uint32_t address, uint32_t size);
		
		Z_STATE write_disable(void);
		
		Z_STATE wait_for_busy_state(W25Q64_OperationType op_type);
		
		uint8_t read_state_reg1(void);
		
		Z_STATE sector_erase(uint32_t Sector_Address);
		
		Z_STATE block_erase(uint32_t Block_Address);
		
		Z_STATE chip_erase(void);
		
		Z_STATE page_write(uint32_t address, uint8_t *data, uint16_t data_size);
		
		void read_id(uint32_t *ID);
		
		Z_STATE auto_erase(uint32_t address, uint32_t size);
		
		void fill_1k_blocks(void);
		
		void verify_1k_blocks(void);
		
		
private:
	
		Z_GPIO_PORT m_sclk_port;
		Z_GPIO_PIN  m_sclk_pin;
		Z_GPIO_PORT m_mosi_port;
		Z_GPIO_PIN m_mosi_pin;
		Z_GPIO_PORT m_miso_port;
		Z_GPIO_PIN m_miso_pin;
	  Z_GPIO_PORT m_cs_port;
		Z_GPIO_PIN m_cs_pin;
};

// 移植 下面
extern DEVICE_W25Q64 device_w25q64_00;
// 移植 上面

void device_w25q64_init(void);

void device_w25q64_deinit(void);

#endif

#endif
