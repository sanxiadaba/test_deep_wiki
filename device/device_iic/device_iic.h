#ifndef __DEVICE_IIC_H
#define __DEVICE_IIC_H

#include "bsp_port.h"

#ifdef USE_DEVICE_IIC


void device_iic_init(void);

void device_iic_deinit(void);


class DEVICE_IIC
{
public:
    // PORT PIN ���ģʽ Ĭ�ϵ�ƽ �����ƽ
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


    // �ظ�
    void iic_ack();
    // ���ظ�
    void iic_nack();
    // ��ʼ�ź�
    void iic_start();
    // �����ź�
    void iic_stop();
    // �ȴ���Ӧ
    Z_STATE iic_wait_ack();
    // д�ֽ�
    Z_STATE iic_write_byte(uint8_t byte);
    // ���ֽ�
    uint8_t iic_read_byte(Z_STATE ack);
    // ��ָ����ַ ָ���Ĵ��� д����
    Z_STATE iic_write_multi_byte(uint8_t slave_addr, uint8_t reg_addr, void *buf, uint16_t length);
    // ��ָ����ַд����
    Z_STATE iic_write_multi_byte_no_reg(uint8_t slave_addr, void *buf, uint16_t length);
    // ��ָ����ַ ָ���Ĵ��� ��ȡ����
    Z_STATE iic_read_multi_byte(uint8_t slave_addr, uint8_t reg_addr, void *buf, uint16_t length);
    // ��ָ����ַ��ȡ����
    Z_STATE iic_read_multi_byte_no_reg(uint8_t slave_addr, void *buf, uint16_t length);
    // ������16λ�ĵ�ַ���Ĵ���
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

// ��ֲ ����
extern DEVICE_IIC device_iic_00;
// ��ֲ ����

#endif

#endif
