#include "device_at24c02.h"

#include "z_include.h"

#ifdef USE_DEVICE_AT24C02

// 移植 下面
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_AT24C02_00_SCL_GPIO_PORT	SCL_00_GPIO_Port
#define DEVICE_AT24C02_00_SCL_PIN				SCL_00_Pin
#define DEVICE_AT24C02_00_SDA_GPIO_PORT	SDA_00_GPIO_Port
#define DEVICE_AT24C02_00_SDA_PIN				SDA_00_Pin
#endif
// 移植 上面

typedef void (*call)(void);

DEVICE_AT24C02::DEVICE_AT24C02(uint8_t address_write,
															 uint8_t address_read,
															 Z_GPIO_PORT scl_port,
															 Z_GPIO_PIN  scl_pin,
															 Z_GPIO_PORT sda_port,
															 Z_GPIO_PIN sda_pin)
    : m_address_write(address_write),
      m_address_read(address_read),
      m_scl_port(scl_port),
      m_scl_pin(scl_pin),
      m_sda_port(sda_port),
      m_sda_pin(sda_pin),
			device_iic(scl_port,scl_pin,sda_port,sda_pin)
{

}

void DEVICE_AT24C02::init(void)
{
	device_iic.init();
}

void DEVICE_AT24C02::deinit(void)
{
	device_iic.deinit();
}

void DEVICE_AT24C02::scan(void)
{
	device_iic.scan();
}


void DEVICE_AT24C02::task(void)
{
	// 每个字节的写入
	if(write_flag==Z_OK)
	{
			Z_STATE state = device_iic.iic_write_multi_byte(m_address_write, count, m_buf+count-m_start_address, 1);
			// 写入成功
			if(state==Z_OK)
			{
					if(count==m_start_address+m_length-1)
					{
							if(m_write_ok_callback)
							{
								m_write_ok_callback();
							}
							count = 0;
							return;
					}
					count++;
			}
			else
			{
					if(m_write_err_callback)
					{
						m_write_err_callback();
					}
			}
	}
}


Z_STATE DEVICE_AT24C02::read(uint8_t start_address,uint8_t buf[],uint8_t length)
{
	return device_iic.iic_read_multi_byte(m_address_read,start_address,buf,length);
}


void DEVICE_AT24C02::write(uint8_t start_address,uint8_t buf[],uint8_t length)
{
	write_flag = Z_OK;
	count = start_address;
	m_start_address = start_address;
	m_buf = buf;
	m_length = length;
}

void DEVICE_AT24C02::set_address(uint8_t address_write,uint8_t address_read)
{
	m_address_write = address_write;
	m_address_read = address_read;
}

void DEVICE_AT24C02::set_callback(call write_ok_callback,call write_err_callback)
{
	m_write_ok_callback = write_ok_callback;
	m_write_err_callback = write_err_callback;
}



// 写回调函数
// 移植 下面
static void device_at24c02_00_write_ok_callback()
{
	z_printf("write ok\r\n");
	device_at24c02_00.write_flag = Z_ERROR;
}

static void device_at24c02_00_write_err_callback()
{
	z_printf("write err\r\n");
	device_at24c02_00.write_flag = Z_ERROR;
}
// 移植 上面

// 实例化类
// 移植 下面
DEVICE_AT24C02 device_at24c02_00(0xA0,0xA1,DEVICE_AT24C02_00_SCL_GPIO_PORT,DEVICE_AT24C02_00_SCL_PIN,DEVICE_AT24C02_00_SDA_GPIO_PORT,DEVICE_AT24C02_00_SDA_PIN);
// 移植 上面


// 初始化
void device_at24c02_init()
{
// 移植 下面
		device_at24c02_00.init();
		device_at24c02_00.set_callback(device_at24c02_00_write_ok_callback,device_at24c02_00_write_err_callback);
// 移植 上面
}

void device_at24c02_deinit()
{
// 移植 下面
		device_at24c02_00.deinit();
// 移植 上面
}

void device_at24c02_task()
{
// 移植 下面
		device_at24c02_00.task();
// 移植 上面
}

#endif


