#include "device_iic.h"
// LED封装

#include "z_include.h"

#ifdef USE_DEVICE_IIC

// 移植 下面
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_IIC_00_SCL_GPIO_PORT	SCL_00_GPIO_Port
#define DEVICE_IIC_00_SCL_PIN				SCL_00_Pin
#define DEVICE_IIC_00_SDA_GPIO_PORT	SDA_00_GPIO_Port
#define DEVICE_IIC_00_SDA_PIN				SDA_00_Pin
#endif
// 移植 上面


// 移植 下面
DEVICE_IIC device_iic_00(DEVICE_IIC_00_SCL_GPIO_PORT,DEVICE_IIC_00_SCL_PIN,DEVICE_IIC_00_SDA_GPIO_PORT,DEVICE_IIC_00_SDA_PIN);
// 移植 上面


void device_iic_init()
{
// 移植 下面
	device_iic_00.init();
// 移植 上面
}

void device_iic_deinit()
{
// 移植 下面

// 移植 上面
}


DEVICE_IIC::DEVICE_IIC(Z_GPIO_PORT scl_port,
                       Z_GPIO_PIN scl_pin,
                       Z_GPIO_PORT sda_port,
                       Z_GPIO_PIN sda_pin):
    m_scl_port(scl_port),
    m_scl_pin(scl_pin),
    m_sda_port(sda_port),
    m_sda_pin(sda_pin)
{

}

// IIC延迟
#define IIC_DELAY() bsp_delay_us(2)


// 读写地址转换宏
#define IIC_WRITE(slave)            (slave & 0xFE)
#define IIC_READ(slave)             (slave | 0x01)

// IO口设置，开漏输出
void DEVICE_IIC::init()
{
    scl_pin_dir_output();
    sda_pin_dir_output();
}

void DEVICE_IIC::deinit()
{
    z_gpio_input_set(m_scl_port,m_scl_pin);
    z_gpio_input_set(m_sda_port,m_sda_pin);
}

// 	IIC地址扫描
void DEVICE_IIC::scan()
{
	uint8_t data = 0;
	z_fori(0,255)
	{
			uint8_t state = iic_read_multi_byte(i, 0, &data, 1);
			if(Z_OK == state)z_printf("0x%02x ",i);
			else z_printf(".");
	}
	z_printf("\r\n\r\n");
}

void DEVICE_IIC::sda_pin_dir_input()
{
    z_gpio_input_set(m_sda_port,m_sda_pin);
}

void DEVICE_IIC::sda_pin_dir_output()
{
    z_gpio_output_od(m_sda_port,m_sda_pin);
}

void DEVICE_IIC::sda_pin_out_low()
{
    z_gpio_write(m_sda_port,m_sda_pin,Z_LEVEL_LOW);
}

void DEVICE_IIC::sda_pin_out_high()
{
    z_gpio_write(m_sda_port,m_sda_pin,Z_LEVEL_HIGH);
}

Z_GPIO_STATE DEVICE_IIC::sda_pin_read_level()
{
    return z_gpio_read_input(m_sda_port,m_sda_pin);
}

void DEVICE_IIC::scl_pin_out_low()
{
    z_gpio_write(m_scl_port,m_scl_pin,Z_LEVEL_LOW);
}

void DEVICE_IIC::scl_pin_out_high()
{
    z_gpio_write(m_scl_port,m_scl_pin,Z_LEVEL_HIGH);
}

void DEVICE_IIC::scl_pin_dir_output()
{
    z_gpio_output_od(m_scl_port,m_scl_pin);
}


// 回复
void DEVICE_IIC::iic_ack()
{
		scl_pin_out_low();
    sda_pin_dir_output();
    sda_pin_out_low();
    IIC_DELAY();
    scl_pin_out_high();
    IIC_DELAY();
    scl_pin_out_low();
}

// 不回复
void DEVICE_IIC::iic_nack()
{
    scl_pin_out_low();
    sda_pin_dir_output();
		sda_pin_out_low();
		IIC_DELAY();
    sda_pin_out_high();  // 提前设置SDA为高
    IIC_DELAY();
    scl_pin_out_high();  // SCL高电平期间SDA保持高（非应答）
    IIC_DELAY();
    scl_pin_out_low();   // 结束非应答
}

// 起始信号
void DEVICE_IIC::iic_start()
{
		scl_pin_out_high();
		sda_pin_out_high();
    IIC_DELAY();
    sda_pin_out_low();
    IIC_DELAY();
    scl_pin_out_low();
}

// 结束信号
void DEVICE_IIC::iic_stop()
{
		scl_pin_out_low();
    sda_pin_out_low();
    IIC_DELAY();
    scl_pin_out_high();
    IIC_DELAY();
    sda_pin_out_high();
    IIC_DELAY();
}

// 等待回应
Z_STATE DEVICE_IIC::iic_wait_ack()
{
    uint16_t wait_time = 255;

    scl_pin_out_low();
    sda_pin_dir_input();
    IIC_DELAY();
    scl_pin_out_high();
    while (sda_pin_read_level()==Z_LEVEL_HIGH)
    {
        if ((wait_time--) == 0)
        {
            sda_pin_out_high();
            sda_pin_dir_output();
            iic_stop();
            return Z_ERROR;
        }
    }
    IIC_DELAY();
    scl_pin_out_low();
    sda_pin_dir_output();
    sda_pin_out_high();
    return Z_OK;
}

// 写字节
Z_STATE DEVICE_IIC::iic_write_byte(uint8_t byte)
{
		z_fori(0,8)
    {
        scl_pin_out_low();
        IIC_DELAY();
        if (byte & 0x80)
            sda_pin_out_high();
        else
            sda_pin_out_low();
        scl_pin_out_high();
        IIC_DELAY();
        byte <<= 1;
    }
    return iic_wait_ack();
}

// 读字节
uint8_t DEVICE_IIC::iic_read_byte(Z_STATE ack)
{
    uint8_t byte = 0;

    sda_pin_dir_input();
    z_fori(0,8)
    {
        byte <<= 1;
        scl_pin_out_low();
        IIC_DELAY();
        scl_pin_out_high();
        if (sda_pin_read_level()==Z_LEVEL_HIGH)
            byte |= 0x01;
        IIC_DELAY();
    }
    if (ack==Z_OK)
        iic_ack();
    else
        iic_nack();
    return byte;
}

// 向指定地址 指定寄存器 写数据
Z_STATE DEVICE_IIC::iic_write_multi_byte(uint8_t slave_addr, uint8_t reg_addr, void *buf, uint16_t length)
{
    Z_STATE state = Z_OK;
    uint8_t *p = (uint8_t*)buf;
		// 起始信号
    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
    {
        iic_stop();
        return state;
    }
    state = iic_write_byte(reg_addr);
    if(state==Z_ERROR)
    {
        iic_stop();
        return state;
    }
    z_fori(0,length)
    {
        state = iic_write_byte(p[i]);
        if(state==Z_ERROR)
        {
            iic_stop();
            return state;
        }
    }
    iic_stop();
    return state;
}

// 向指定地址写数据
Z_STATE DEVICE_IIC::iic_write_multi_byte_no_reg(uint8_t slave_addr, void *buf, uint16_t length)
{
    Z_STATE state = Z_OK;
    uint8_t *p = (uint8_t*)buf;

    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        state = iic_write_byte(p[i]);
				if(state==Z_ERROR)
        {
            iic_stop();
            return state;
        }
    }
    iic_stop();
    return state;
}

// 从指定地址 指定寄存器 读取数据
Z_STATE DEVICE_IIC::iic_read_multi_byte(uint8_t slave_addr, uint8_t reg_addr, void *buf, uint16_t length)
{
    uint8_t *p = (uint8_t*)buf;
    Z_STATE state = Z_OK;
    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    state = iic_write_byte(reg_addr);
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    iic_start();
    state = iic_write_byte(IIC_READ(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        if (i != (length - 1))
        {
            p[i] = iic_read_byte(Z_OK);
        }
        else
        {
            p[i] = iic_read_byte(Z_ERROR);
        }
    }
    iic_stop();
    return state;
}

// 从指定地址读取数据
Z_STATE DEVICE_IIC::iic_read_multi_byte_no_reg(uint8_t slave_addr, void *buf, uint16_t length)
{
    uint8_t *p = (uint8_t*)buf;
    Z_STATE state = Z_OK;
    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    iic_start();
    state = iic_write_byte(IIC_READ(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        if (i != (length - 1))
        {
            p[i] = iic_read_byte(Z_OK);
        }
        else
        {
            p[i] = iic_read_byte(Z_ERROR);
        }
    }
    iic_stop();
    return state;
}

// 下面是16位的寄存器地址
Z_STATE DEVICE_IIC::iic_write_multi_byte_16bit(uint8_t slave_addr, uint16_t reg_addr, void *buf, uint16_t length)
{
		Z_STATE state = Z_OK;
    uint8_t *p = (uint8_t*)buf;

    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    state =iic_write_byte((uint8_t)(reg_addr >> 8));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    state =iic_write_byte((uint8_t)reg_addr);
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        state = iic_write_byte(p[i]);
        if(state==Z_ERROR)
				{
						iic_stop();
						return state;
				}
    }
    iic_stop();

    return state;
}

Z_STATE DEVICE_IIC::iic_read_multi_byte_16bit(uint8_t slave_addr, uint16_t reg_addr, void *buf, uint16_t length)
{
    uint8_t *p = (uint8_t*)buf;
    Z_STATE state = Z_OK;
    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    state = iic_write_byte((uint8_t)(reg_addr >> 8));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    state = iic_write_byte((uint8_t)reg_addr);
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    iic_start();
    state = iic_write_byte(IIC_READ(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        if (i != (length - 1))
        {
            p[i] = iic_read_byte(Z_OK);
        }
        else
        {
            p[i] = iic_read_byte(Z_ERROR);
        }
    }
    iic_stop();
    return state;
}

Z_STATE DEVICE_IIC::iic_write_multi_byte_16bit_no_reg(uint8_t slave_addr, void *buf, uint16_t length)
{
    Z_STATE state = Z_OK;
    uint8_t *p = (uint8_t*)buf;

    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        state = iic_write_byte(p[i]);
        if(state==Z_ERROR)
				{
						iic_stop();
						return state;
				}
    }
    iic_stop();

    return state;
}

Z_STATE DEVICE_IIC::iic_read_multi_byte_16bit_no_reg(uint8_t slave_addr, void *buf, uint16_t length)
{
    uint8_t *p = (uint8_t*)buf;
    Z_STATE state = Z_OK;
    iic_start();
    state = iic_write_byte(IIC_WRITE(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    iic_start();
    state = iic_write_byte(IIC_READ(slave_addr));
    if(state==Z_ERROR)
		{
				iic_stop();
				return state;
		}
    z_fori(0,length)
    {
        if (i != (length - 1))
        {
            p[i] = iic_read_byte(Z_OK);
        }
        else
        {
            p[i] = iic_read_byte(Z_ERROR);
        }
    }
    iic_stop();
    return state;
}


#endif
