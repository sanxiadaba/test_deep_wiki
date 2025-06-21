#ifndef __Z_RINGBUFFER__H
#define __Z_RINGBUFFER__H

#include <stdint.h>

// ring_buffer�ṹ��
typedef struct z_ring_buffer_stu
{
    // ����Ӧ�ö�ȡ��λ��
    uint32_t r_location;
    // ����Ӧ��д���λ��
    uint32_t w_location;
    // �洢buffer
    uint8_t *buffer;
    // buffer����
    uint32_t capacity;
    // read��ȡ˳�򣬶���β�ٶ���w_location
    // 1:����β�ٶ���w_location
    // 0:����w_location
    uint8_t read_flag;
} z_ring_buffer_stu;

// �ͷ��ڴ�
void z_ring_buffer_free(z_ring_buffer_stu ** ring_buffer_ptr_ptr);

z_ring_buffer_stu *z_ring_buffer_init(int32_t buffer_size);

uint32_t z_ring_buffer_get_capacity(z_ring_buffer_stu * ring_buffer_ptr);

uint32_t z_ring_buffer_get_read_num(z_ring_buffer_stu * ring_buffer_ptr);

void z_ring_buffer_write(z_ring_buffer_stu * ring_buffer_ptr, uint8_t *arr, int32_t arr_size);

void z_ring_buffer_read_buffer(z_ring_buffer_stu * ring_buffer_ptr, uint8_t* arr, int32_t arr_size);

#endif
