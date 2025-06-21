#ifndef __DEVICE_SPI_H
#define __DEVICE_SPI_H

#include "bsp_port.h"

#ifdef USE_DEVICE_SPI


// ����SPI
class DEVICE_4SPI
{
public:
		// SPIģʽ
		enum SPI4_MODE
		{
			SPI4_MODE0 = 0,
			SPI4_MODE1,
			SPI4_MODE2,
			SPI4_MODE3
		};

		// ��λ���ȡ���λ����
		enum SPI4_BIT_ORDER
		{
			SPI4_MSB_FIRST = 0,
			SPI4_LSB_FIRST
		};

		// ���߶˿ڡ����� Ƭѡ��Ч��ƽ SPIģʽ ��С���ֽ���
    DEVICE_4SPI (Z_GPIO_PORT scl_port,
								 Z_GPIO_PIN  scl_pin,
								 Z_GPIO_PORT mosi_port,
								 Z_GPIO_PIN mosi_pin,
								 Z_GPIO_PORT miso_port,
								 Z_GPIO_PIN miso_pin,
								 Z_GPIO_PORT cs_port,
								 Z_GPIO_PIN cs_pin,
								 Z_GPIO_STATE cs_active_level,
								 SPI4_MODE spi4_mode,
								 SPI4_BIT_ORDER spi4_bit_order);

		void init();
		void deinit();
		void active_cs(void);
		void release_cs(void);
		uint8_t transfer_core(uint8_t data);
		uint8_t transfer_8bit(uint8_t data);
		uint16_t transfer_16bit(uint16_t data);
		void transfer_8bit_bulk(const uint8_t *tx_data, uint8_t *rx_data, uint16_t len);
		void transfer_16bit_bulk(const uint16_t *tx_data, uint16_t *rx_data, uint16_t len);
		
		uint8_t	cpol;            // ʱ�Ӽ���
    uint8_t	cpha;            // ʱ����λ
		
		
		
private:
		Z_GPIO_PORT m_sclk_port;
		Z_GPIO_PIN  m_sclk_pin;
		Z_GPIO_PORT m_mosi_port;
		Z_GPIO_PIN m_mosi_pin;
		Z_GPIO_PORT m_miso_port;
		Z_GPIO_PIN m_miso_pin;
	  Z_GPIO_PORT m_cs_port;
		Z_GPIO_PIN m_cs_pin;
		Z_GPIO_STATE m_cs_active_level;
		SPI4_MODE m_spi4_mode;
		SPI4_BIT_ORDER m_spi4_bit_order;
		
};

// ����SPI
class DEVICE_3SPI
{
	public:
		
		// SPIģʽ
		enum SPI3_MODE
		{
			SPI3_MODE0 = 0,
			SPI3_MODE1,
			SPI3_MODE2,
			SPI3_MODE3
		};

		// ��λ���ȡ���λ����
		enum SPI3_BIT_ORDER
		{
			SPI3_MSB_FIRST = 0,
			SPI3_LSB_FIRST
		};
	
		// ���߶˿ڡ����� Ƭѡ��Ч��ƽ SPIģʽ ��С���ֽ���
    DEVICE_3SPI (Z_GPIO_PORT scl_port,
								 Z_GPIO_PIN  scl_pin,
								 Z_GPIO_PORT mosi_port,
								 Z_GPIO_PIN mosi_pin,
								 Z_GPIO_PORT cs_port,
								 Z_GPIO_PIN cs_pin,
								 Z_GPIO_STATE cs_active_level,
								 SPI3_MODE spi3_mode,
								 SPI3_BIT_ORDER spi3_bit_order);

		void init();
		void deinit();
		void active_cs(void);
		void release_cs(void);

		void transfer_core(uint8_t data);
		void transfer_8bit(uint8_t data);
		void transfer_16bit(uint16_t data);
		void transfer_8bit_bulk(const uint8_t *tx_data, uint16_t len);
		void transfer_16bit_bulk(const uint16_t *tx_data, uint16_t len);
		
		uint8_t	cpol;            // ʱ�Ӽ���
    uint8_t	cpha;            // ʱ����λ
		
		
		
private:
		Z_GPIO_PORT m_sclk_port;
		Z_GPIO_PIN  m_sclk_pin;
		Z_GPIO_PORT m_mosi_port;
		Z_GPIO_PIN m_mosi_pin;
	  Z_GPIO_PORT m_cs_port;
		Z_GPIO_PIN m_cs_pin;
		Z_GPIO_STATE m_cs_active_level;
		SPI3_MODE m_spi3_mode;
		SPI3_BIT_ORDER m_spi3_bit_order;
		
};

// ��ֲ ����
extern DEVICE_4SPI device_4spi_00;
extern DEVICE_3SPI device_3spi_00;
// ��ֲ ����

void device_spi_init(void);
void device_spi_deinit(void);


#endif

#endif
