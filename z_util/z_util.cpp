#include "z_util.h"
// 一些基础功能的函数

#include "z_include.h"

#ifdef Z_DEBUG


static uint8_t z_var_to_str_buf[__Z_DEBUG_SIZE] =  {0};

// 里面数组的大小 处理完后的回调函数 变量
void z_var_to_str(z_t_process_var_to_str z_process_var_to_str, const char * sFormat, ...)
{
    int32_t str_len = 0;
    va_list ParamList;
    va_start(ParamList, sFormat);
    str_len = vsnprintf((char *)z_var_to_str_buf, __Z_DEBUG_SIZE, sFormat, ParamList);
    z_process_var_to_str(z_var_to_str_buf, str_len);
    va_end(ParamList);
    return;
}

#endif


static uint8_t lookup[16] =
{
    0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
    0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,
};


// bit反转
uint8_t z_reverse_8bit(uint8_t n)
{
    return (lookup[n & 0b1111] << 4) | lookup[n >> 4];
}


uint16_t z_reverse_16bit(uint16_t n)
{
    uint16_t output = 0;
    return  ((output | z_reverse_8bit(n)) << 8) | ((output | z_reverse_8bit(n >> 8)));
}


uint32_t z_reverse_32bit(uint32_t n)
{
    uint32_t output = 0;
    return  ((output | z_reverse_16bit(n)) << 16) | ((output | z_reverse_16bit(n >> 16)));
}


uint64_t z_reverse_64bit(uint64_t n)
{
    uint64_t output = 0;
    return  ((output | z_reverse_32bit(n)) << 32) | ((output | z_reverse_32bit(n >> 32)));
}
