#ifndef __DEVICE_IIC_H
#define __DEVICE_IIC_H

#include "bsp_port.h"

#ifdef USE_DEVICE_IIC


void device_iic_init(void);

void device_iic_deinit(void);


class DEVICE_IIC
{
public:
    // PORT PIN 输出模式 默认电平 激活电平
    DEVICE_IIC(Z_GPIO_PORT scl_port,
               Z_GPIO_PIN scl_pin,
               Z_GPIO_PORT sda_port,
               Z_GPIO_PIN sda_pin);

    void init();
    void deinit();
    void scan();
    void sda_pin_dir_input();
    void sda_pin_dir_output();
    void sda_pin_out_low();
    void sda_pin_out_high();
    Z_GPIO_STATE sda_pin_read_level();
    void scl_pin_out_low();
    void scl_pin_out_high();
    void scl_pin_dir_output();


    // 回复
    void iic_ack();
    // 不回复
    void iic_nack();
    // 起始信号
    void iic_start();
    // 结束信号
    void iic_stop();
    // 等待回应
    Z_STATE iic_wait_ack();
    // 写字节
    Z_STATE iic_write_byte(uint8_t byte);
    // 读字节
    uint8_t iic_read_byte(Z_STATE ack);
    // 向指定地址 指定寄存器 写数据
    Z_STATE iic_write_multi_byte(uint8_t slave_addr, uint8_t reg_addr, void *buf, uint16_t length);
    // 向指定地址写数据
    Z_STATE iic_write_multi_byte_no_reg(uint8_t slave_addr, void *buf, uint16_t length);
    // 从指定地址 指定寄存器 读取数据
    Z_STATE iic_read_multi_byte(uint8_t slave_addr, uint8_t reg_addr, void *buf, uint16_t length);
    // 从指定地址读取数据
    Z_STATE iic_read_multi_byte_no_reg(uint8_t slave_addr, void *buf, uint16_t length);
    // 下面是16位的地址、寄存器
    Z_STATE iic_write_multi_byte_16bit(uint8_t slave_addr, uint16_t reg_addr, void *buf, uint16_t length);
    Z_STATE iic_read_multi_byte_16bit(uint8_t slave_addr, uint16_t reg_addr, void *buf, uint16_t length);
    Z_STATE iic_write_multi_byte_16bit_no_reg(uint8_t slave_addr, void *buf, uint16_t length);
    Z_STATE iic_read_multi_byte_16bit_no_reg(uint8_t slave_addr, void *buf, uint16_t length);

private:
    Z_GPIO_PORT m_scl_port;
    Z_GPIO_PIN m_scl_pin;
    Z_GPIO_PORT m_sda_port;
    Z_GPIO_PIN m_sda_pin;
};

// 移植 下面
extern DEVICE_IIC device_iic_00;
// 移植 上面

#endif

#endif
