#include "device_spi.h"

#include "z_include.h"

#ifdef USE_DEVICE_SPI

// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_SPI4_00_SCLK_GPIO_PORT	SCLK_00_GPIO_Port
#define DEVICE_SPI4_00_SCLK_PIN				SCLK_00_Pin
#define DEVICE_SPI4_00_MOSI_GPIO_PORT	MOSI_00_GPIO_Port
#define DEVICE_SPI4_00_MOSI_PIN				MOSI_00_Pin
#define DEVICE_SPI4_00_MISO_GPIO_PORT	MISO_00_GPIO_Port
#define DEVICE_SPI4_00_MISO_PIN				MISO_00_Pin
#define DEVICE_SPI4_00_CS_GPIO_PORT		CS_00_GPIO_Port
#define DEVICE_SPI4_00_CS_PIN					CS_00_Pin
#endif
// ��ֲ ����


// �ڲ���ʱ����
static inline void spi_delay(uint16_t cycles)
{
    volatile uint32_t i = cycles;

    while (i--)
    {
        __nop();
    }
}

// ����SPI
DEVICE_4SPI::DEVICE_4SPI(Z_GPIO_PORT sclk_port,
												 Z_GPIO_PIN  sclk_pin,
												 Z_GPIO_PORT mosi_port,
												 Z_GPIO_PIN mosi_pin,
												 Z_GPIO_PORT miso_port,
												 Z_GPIO_PIN miso_pin,
												 Z_GPIO_PORT cs_port,
												 Z_GPIO_PIN cs_pin,
												 Z_GPIO_STATE cs_active_level = Z_LEVEL_LOW,
												 SPI4_MODE spi4_mode = SPI4_MODE0,
												 SPI4_BIT_ORDER spi4_bit_order = SPI4_MSB_FIRST)
						 :
							 m_sclk_port(sclk_port),
							 m_sclk_pin(sclk_pin),
							 m_mosi_port(mosi_port),
							 m_mosi_pin(mosi_pin),
							 m_miso_port(miso_port),
							 m_miso_pin(miso_pin),
							 m_cs_port(cs_port),
							 m_cs_pin(cs_pin),
							 m_cs_active_level(cs_active_level),
							 m_spi4_mode(spi4_mode),
							 m_spi4_bit_order(spi4_bit_order)
{
	switch(m_spi4_mode)
    {
    case SPI4_MODE0: // CPOL=0, CPHA=0
        cpol = 0;
        cpha = 0;
        break;
    case SPI4_MODE1: // CPOL=0, CPHA=1
        cpol = 0;
        cpha = 1;
        break;
    case SPI4_MODE2: // CPOL=1, CPHA=0
        cpol = 1;
        cpha = 0;
        break;
    case SPI4_MODE3: // CPOL=1, CPHA=1
        cpol = 1;
        cpha = 1;
        break;
    default:        // ��ЧģʽĬ��ΪMODE0
        cpol = 0;
        cpha = 0;
        break;
    }
};

// ����SPI
DEVICE_3SPI::DEVICE_3SPI(Z_GPIO_PORT sclk_port,
												 Z_GPIO_PIN  sclk_pin,
												 Z_GPIO_PORT mosi_port,
												 Z_GPIO_PIN mosi_pin,
												 Z_GPIO_PORT cs_port,
												 Z_GPIO_PIN cs_pin,
												 Z_GPIO_STATE cs_active_level = Z_LEVEL_LOW,
												 SPI3_MODE spi3_mode = SPI3_MODE0,
												 SPI3_BIT_ORDER spi3_bit_order = SPI3_MSB_FIRST)
						 :
							 m_sclk_port(sclk_port),
							 m_sclk_pin(sclk_pin),
							 m_mosi_port(mosi_port),
							 m_mosi_pin(mosi_pin),
							 m_cs_port(cs_port),
							 m_cs_pin(cs_pin),
							 m_cs_active_level(cs_active_level),
							 m_spi3_mode(spi3_mode),
							 m_spi3_bit_order(spi3_bit_order)
{
	switch(m_spi3_mode)
    {
    case SPI3_MODE0: // CPOL=0, CPHA=0
        cpol = 0;
        cpha = 0;
        break;
    case SPI3_MODE1: // CPOL=0, CPHA=1
        cpol = 0;
        cpha = 1;
        break;
    case SPI3_MODE2: // CPOL=1, CPHA=0
        cpol = 1;
        cpha = 0;
        break;
    case SPI3_MODE3: // CPOL=1, CPHA=1
        cpol = 1;
        cpha = 1;
        break;
    default:        // ��ЧģʽĬ��ΪMODE0
        cpol = 0;
        cpha = 0;
        break;
    }
};


void DEVICE_4SPI::init()
{
		// ����GPIOģʽ
    z_gpio_output_pp_up(m_mosi_port, m_mosi_pin);
		z_gpio_output_pp_up(m_miso_port, m_miso_pin);
    z_gpio_output_pp_up(m_sclk_port,m_sclk_pin);
    z_gpio_output_pp_up(m_cs_port,m_cs_pin);

    // ��ʼ����ƽ
    z_gpio_write(m_sclk_port, m_sclk_pin,
                 cpol ? Z_LEVEL_HIGH : Z_LEVEL_LOW);
		// ��ʼ�ͷ�CS
    z_gpio_write(m_cs_port, m_cs_pin,
                 (m_cs_active_level == Z_LEVEL_LOW) ?
                 Z_LEVEL_HIGH : Z_LEVEL_LOW);
}


void DEVICE_3SPI::init()
{
		// ����GPIOģʽ
    z_gpio_output_pp_up(m_mosi_port, m_mosi_pin);
    z_gpio_output_pp_up(m_sclk_port,m_sclk_pin);
    z_gpio_output_pp_up(m_cs_port,m_cs_pin);

    // ��ʼ����ƽ
    z_gpio_write(m_sclk_port, m_sclk_pin,
                 cpol ? Z_LEVEL_HIGH : Z_LEVEL_LOW);
		// ��ʼ�ͷ�CS
    z_gpio_write(m_cs_port, m_cs_pin,
                 (m_cs_active_level == Z_LEVEL_LOW) ?
                 Z_LEVEL_HIGH : Z_LEVEL_LOW);
}


void DEVICE_4SPI::deinit()
{
	z_gpio_input_set(m_mosi_port, m_mosi_pin);
	z_gpio_input_set(m_miso_port, m_miso_pin);
	z_gpio_input_set(m_sclk_port,m_sclk_pin);
	z_gpio_input_set(m_cs_port,m_cs_pin);
}

void DEVICE_3SPI::deinit()
{
	z_gpio_input_set(m_mosi_port, m_mosi_pin);
	z_gpio_input_set(m_sclk_port,m_sclk_pin);
	z_gpio_input_set(m_cs_port,m_cs_pin);
}


void DEVICE_4SPI::active_cs(void)
{
	z_gpio_write(m_cs_port, m_cs_pin, m_cs_active_level);
}

void DEVICE_3SPI::active_cs(void)
{
	z_gpio_write(m_cs_port, m_cs_pin, m_cs_active_level);
}

void DEVICE_4SPI::release_cs(void)
{
	z_gpio_write(m_cs_port, m_cs_pin, (Z_GPIO_STATE)(!m_cs_active_level));
}

void DEVICE_3SPI::release_cs(void)
{
	z_gpio_write(m_cs_port, m_cs_pin, (Z_GPIO_STATE)(!m_cs_active_level));
}


uint8_t DEVICE_4SPI::transfer_core(uint8_t data)
{
	uint8_t received = 0;

	for (uint8_t bit = 0; bit < 8; bit++)
	{
			uint8_t bit_pos = (m_spi4_bit_order == SPI4_MSB_FIRST) ? (7 - bit) : bit;
			Z_GPIO_STATE bit_state = (data & (1 << bit_pos)) ? Z_LEVEL_HIGH : Z_LEVEL_LOW;

			if (cpha == 0)
			{
					// MODE0/MODE2: �����ڵ�һ�����ز���
					z_gpio_write(m_mosi_port, m_mosi_pin, bit_state);
					spi_delay(2);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // ����1��������
					spi_delay(2);
					uint8_t input_bit = z_gpio_read_input(m_miso_port, m_miso_pin);
					received = (received << 1) | (input_bit & 0x01);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // �ָ�ʱ��
			}
			else
			{
					// MODE1/MODE3: �����ڵڶ������ز���
					z_gpio_write(m_mosi_port, m_mosi_pin, bit_state);
					spi_delay(1);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // ����1����������
					spi_delay(2);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // ����2��������
					uint8_t input_bit = z_gpio_read_input(m_miso_port, m_miso_pin);
					received = (received << 1) | (input_bit & 0x01);
					spi_delay(1);
			}
	}
	return received;
}

void DEVICE_3SPI::transfer_core(uint8_t data)
{
	uint8_t received = 0;

	for (uint8_t bit = 0; bit < 8; bit++)
	{
			uint8_t bit_pos = (m_spi3_bit_order == SPI3_MSB_FIRST) ? (7 - bit) : bit;
			Z_GPIO_STATE bit_state = (data & (1 << bit_pos)) ? Z_LEVEL_HIGH : Z_LEVEL_LOW;

			if (cpha == 0)
			{
					// MODE0/MODE2: �����ڵ�һ�����ز���
					z_gpio_write(m_mosi_port, m_mosi_pin, bit_state);
					spi_delay(2);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // ����1��������
					spi_delay(2);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // �ָ�ʱ��
			}
			else
			{
					// MODE1/MODE3: �����ڵڶ������ز���
					z_gpio_write(m_mosi_port, m_mosi_pin, bit_state);
					spi_delay(1);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // ����1����������
					spi_delay(2);
					z_gpio_toggle(m_sclk_port, m_sclk_pin); // ����2��������
					spi_delay(1);
			}
	}
}

uint8_t DEVICE_4SPI::transfer_8bit(uint8_t data)
{
	return transfer_core(data);
}


void DEVICE_3SPI::transfer_8bit(uint8_t data)
{
	transfer_core(data);
}


uint16_t DEVICE_4SPI::transfer_16bit(uint16_t data)
{
	union
    {
        uint16_t val;
        uint8_t bytes[2];
    } tx, rx;

    tx.val = data;

    if(m_spi4_bit_order == SPI4_MSB_FIRST)
    {
        rx.bytes[1] = transfer_core(tx.bytes[1]); // �ȴ����ֽ�
        rx.bytes[0] = transfer_core(tx.bytes[0]);
    }
    else
    {
        rx.bytes[0] = transfer_core(tx.bytes[0]); // �ȴ����ֽ�
        rx.bytes[1] = transfer_core(tx.bytes[1]);
    }
		
    return rx.val;
}


void DEVICE_3SPI::transfer_16bit(uint16_t data)
{
	union
    {
        uint16_t val;
        uint8_t bytes[2];
    } tx;

    tx.val = data;

    if(m_spi3_bit_order == SPI3_MSB_FIRST)
    {
        transfer_core(tx.bytes[1]); // �ȴ����ֽ�
        transfer_core(tx.bytes[0]);
    }
    else
    {
        transfer_core(tx.bytes[0]); // �ȴ����ֽ�
        transfer_core(tx.bytes[1]);
    }
}


void DEVICE_4SPI::transfer_8bit_bulk(const uint8_t *tx_data, uint8_t *rx_data, uint16_t len)
{
	z_fori(0,len)
	{
			uint8_t tx_byte = tx_data ? tx_data[i] : 0xFF;   // ��tx_dataΪNULL������0xFF
			uint8_t rx_byte = transfer_core(tx_byte); // ���䲢��������
			if (rx_data)
			{
					rx_data[i] = rx_byte; // ����������ݣ���rx_data��NULL
			}
	}
}

void DEVICE_3SPI::transfer_8bit_bulk(const uint8_t *tx_data, uint16_t len)
{
	z_fori(0,len)
	{
			uint8_t tx_byte = tx_data ? tx_data[i] : 0xFF;   // ��tx_dataΪNULL������0xFF
			transfer_core(tx_byte); // ���䲢��������
	}
}



void DEVICE_4SPI::transfer_16bit_bulk(const uint16_t *tx_data, uint16_t *rx_data, uint16_t len)
{
	z_fori(0,len)
	{
			union
			{
					uint16_t val;
					uint8_t bytes[2];
			} tx, rx;

			// ��tx_dataΪNULL������ȫ1��0xFFFF��
			tx.val = tx_data ? tx_data[i] : 0xFFFF;

			// �����ֽ���������
			if (m_spi4_bit_order == SPI4_MSB_FIRST)
			{
					rx.bytes[1] = transfer_core(tx.bytes[1]); // �ȴ����ֽ�
					rx.bytes[0] = transfer_core(tx.bytes[0]);
			}
			else
			{
					rx.bytes[0] = transfer_core(tx.bytes[0]); // �ȴ����ֽ�
					rx.bytes[1] = transfer_core(tx.bytes[1]);
			}

			// ����������ݣ���rx_data��NULL
			if (rx_data)
			{
					rx_data[i] = rx.val;
			}
	}
}

void DEVICE_3SPI::transfer_16bit_bulk(const uint16_t *tx_data, uint16_t len)
{
	z_fori(0,len)
	{
			union
			{
					uint16_t val;
					uint8_t bytes[2];
			} tx;

			// ��tx_dataΪNULL������ȫ1��0xFFFF��
			tx.val = tx_data ? tx_data[i] : 0xFFFF;

			// �����ֽ���������
			if (m_spi3_bit_order == SPI3_MSB_FIRST)
			{
					transfer_core(tx.bytes[1]); // �ȴ����ֽ�
					transfer_core(tx.bytes[0]);
			}
			else
			{
					transfer_core(tx.bytes[0]); // �ȴ����ֽ�
					transfer_core(tx.bytes[1]);
			}
	}
}



// ��ֲ ����
DEVICE_4SPI device_4spi_00(DEVICE_SPI4_00_SCLK_GPIO_PORT,DEVICE_SPI4_00_SCLK_PIN,\
													 DEVICE_SPI4_00_MOSI_GPIO_PORT,DEVICE_SPI4_00_MOSI_PIN,\
													 DEVICE_SPI4_00_MISO_GPIO_PORT,DEVICE_SPI4_00_MISO_PIN,\
													 DEVICE_SPI4_00_CS_GPIO_PORT,DEVICE_SPI4_00_CS_PIN\
);

DEVICE_3SPI device_3spi_00(DEVICE_SPI4_00_SCLK_GPIO_PORT,DEVICE_SPI4_00_SCLK_PIN,\
													 DEVICE_SPI4_00_MOSI_GPIO_PORT,DEVICE_SPI4_00_MOSI_PIN,\
													 DEVICE_SPI4_00_CS_GPIO_PORT,DEVICE_SPI4_00_CS_PIN\
);
// ��ֲ ����


void device_spi4_init(void)
{
// ��ֲ ����
	device_4spi_00.init();
// ��ֲ ����
}

void device_spi4_deinit(void)
{
// ��ֲ ����
	device_4spi_00.deinit();
// ��ֲ ����
}


void device_spi3_init(void)
{
// ��ֲ ����
	device_3spi_00.init();
// ��ֲ ����
}

void device_spi3_deinit(void)
{
// ��ֲ ����
	device_3spi_00.deinit();
// ��ֲ ����
}

void device_spi_init(void)
{
	device_spi4_init();
	device_spi3_init();
}


void device_spi_deinit(void)
{
	device_spi4_deinit();
	device_spi3_deinit();
}


#endif


