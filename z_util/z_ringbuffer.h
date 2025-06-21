#ifndef __Z_RINGBUFFER__H
#define __Z_RINGBUFFER__H

#include <stdint.h>

// ring_buffer结构体
typedef struct z_ring_buffer_stu
{
    // 数据应该读取的位置
    uint32_t r_location;
    // 数据应该写入的位置
    uint32_t w_location;
    // 存储buffer
    uint8_t *buffer;
    // buffer容量
    uint32_t capacity;
    // read读取顺序，读到尾再读到w_location
    // 1:读到尾再读到w_location
    // 0:读到w_location
    uint8_t read_flag;
} z_ring_buffer_stu;

// 释放内存
void z_ring_buffer_free(z_ring_buffer_stu ** ring_buffer_ptr_ptr);

z_ring_buffer_stu *z_ring_buffer_init(int32_t buffer_size);

uint32_t z_ring_buffer_get_capacity(z_ring_buffer_stu * ring_buffer_ptr);

uint32_t z_ring_buffer_get_read_num(z_ring_buffer_stu * ring_buffer_ptr);

void z_ring_buffer_write(z_ring_buffer_stu * ring_buffer_ptr, uint8_t *arr, int32_t arr_size);

void z_ring_buffer_read_buffer(z_ring_buffer_stu * ring_buffer_ptr, uint8_t* arr, int32_t arr_size);

#endif
