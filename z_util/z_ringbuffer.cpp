#include "z_ringbuffer.h"
// FIFO�Զ���ʵ��

#include "z_include.h"


// ע�⣬������ǽṹ��ָ���ָ��
// �ͷ�ring_buffer
void z_ring_buffer_free(z_ring_buffer_stu ** ring_buffer_ptr_ptr)
{
    z_ring_buffer_stu * ring_buffer = *ring_buffer_ptr_ptr;
    free(ring_buffer->buffer);
    ring_buffer->buffer = NULL;
    free(ring_buffer);
    *ring_buffer_ptr_ptr = NULL;
}


// ��ʼ��һ��buffer_size��С��uint8_t buffer
z_ring_buffer_stu *z_ring_buffer_init(int32_t buffer_size)
{
    z_ring_buffer_stu *ring_buffer_ptr = (z_ring_buffer_stu*)(malloc(sizeof(z_ring_buffer_stu)));
    ring_buffer_ptr->buffer = (uint8_t *)(calloc(buffer_size, 1));
    ring_buffer_ptr->r_location = 0;
    ring_buffer_ptr->w_location = 0;
    ring_buffer_ptr->capacity = buffer_size;
    ring_buffer_ptr->read_flag = 0;
    return ring_buffer_ptr;
}


// ��ȡbuffer��������
uint32_t z_ring_buffer_get_capacity(z_ring_buffer_stu * ring_buffer_ptr)
{
    return (ring_buffer_ptr->capacity);
}


//��ȡ����ȡ������
uint32_t z_ring_buffer_get_read_num(z_ring_buffer_stu * ring_buffer_ptr)
{
    uint32_t r_location = ring_buffer_ptr->r_location;
    uint32_t w_location = ring_buffer_ptr->w_location;
    uint32_t capacity = ring_buffer_ptr->capacity;
    uint8_t read_flag = ring_buffer_ptr->read_flag;
    uint32_t read_num = 0;
    if (read_flag == 0)
    {
        read_num = w_location - r_location;
    }
    else if (read_flag == 1)
    {
        read_num = capacity - r_location  + w_location;
    }
    return read_num;
}


// ����д��
void z_ring_buffer_write(z_ring_buffer_stu * ring_buffer_ptr, uint8_t *arr, int32_t arr_size)
{
    int32_t w_location = ring_buffer_ptr->w_location;
    int32_t r_location = ring_buffer_ptr->r_location;
    int32_t capacity = ring_buffer_ptr->capacity;
    uint8_t *buffer = ring_buffer_ptr->buffer;
    uint8_t read_flag = ring_buffer_ptr->read_flag;
    int32_t tmp = 0;
    if ((read_flag == 0) &&  (w_location >= r_location))
    {
        tmp = z_math_min_two_num(arr_size, capacity - w_location);
        memcpy(buffer + w_location, arr, tmp);
        w_location = (w_location + tmp) % capacity;
        arr += tmp;
        arr_size -= tmp;
    }
    if (read_flag == 0 && w_location == 0)
    {
        read_flag = 1;
    }
    if (read_flag == 1)
    {
        if (w_location < r_location)
        {
            tmp = z_math_min_two_num(arr_size, r_location - w_location);
            memcpy(buffer + w_location, arr, tmp);
            w_location = (w_location + tmp) % capacity;
            arr += tmp;
            arr_size -= tmp;
        }
        // ��ʱ�����Ѿ�����
        if (arr_size > 0)
        {
            tmp = z_math_min_two_num(arr_size, capacity - w_location);
            memcpy(buffer + w_location, arr, tmp);
            w_location = (w_location + tmp) % capacity;
            arr += tmp;
            arr_size -= tmp;
            // ��ͷ��ʼ�������
            if (w_location == 0 && arr_size > 0)
            {
                tmp = arr_size / capacity ;
                if (tmp == 0)
                {
                    memcpy(buffer, arr, arr_size);
                    w_location = (w_location + arr_size) % capacity;
                    arr += arr_size;
                    arr_size -= arr_size;
                }
                else
                {
                    tmp = (tmp - 1) * capacity;
                    arr += tmp;
                    memcpy(buffer, arr, capacity);
                    arr += capacity;
                    tmp = arr_size % capacity ;
                    memcpy(buffer, arr, tmp);
                    w_location = tmp;
                }
            }
            r_location = w_location;
        }
    }

    ring_buffer_ptr->read_flag = read_flag;
    ring_buffer_ptr->w_location = w_location;
    ring_buffer_ptr->r_location = r_location;
}


// �����ݷŵ��������
void z_ring_buffer_read_buffer(z_ring_buffer_stu * ring_buffer_ptr, uint8_t* arr, int32_t arr_size)
{
    int32_t r_location = ring_buffer_ptr->r_location;
    int32_t capacity = ring_buffer_ptr->capacity;
    uint8_t *buffer = ring_buffer_ptr->buffer;
    uint8_t read_flag = ring_buffer_ptr->read_flag;
    int32_t tmp = 0;
    int32_t all_num = z_ring_buffer_get_read_num(ring_buffer_ptr);
    int32_t read_num = z_math_min_two_num(all_num, arr_size);
    if (read_num == 0)
    {
        return;
    }
    if (read_num > (capacity - r_location))
    {
        tmp = read_num - (capacity - r_location);
        read_num = capacity - r_location;
    }
    memcpy(arr, buffer + r_location, read_num);
    r_location = (r_location + read_num) % capacity;
    if (r_location == 0)
    {
        read_flag = 0;
        memcpy(arr + read_num, buffer, tmp);
        r_location += tmp;
    }
    ring_buffer_ptr->r_location = r_location;
    ring_buffer_ptr->read_flag = read_flag;
}
