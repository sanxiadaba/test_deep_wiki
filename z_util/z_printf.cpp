#include "z_printf.h"
// 打印函数

#include "z_include.h"

void __z_printf(uint8_t arr[], int32_t size)
{
    __z_port_printf(arr, size);
}


// 2进制打印
void z_print_bin(int64_t n, int32_t len)
{
    uint8_t __attribute__((unused)) buffer[len + 1];
    buffer[len] = '\0';
    for (int32_t i = len - 1; i >= 0; i--)
    {
        if (n & ((uint64_t)1 << i))
        {
            buffer[len - i - 1] = '1';
        }
        else
        {
            buffer[len - i - 1] = '0';
        }
    }
    z_fori (0,len / 8)
    {
        z_forj (0,8)
        {
            __z_port_printf(&(buffer[i * 8 + j]), 1);
        }
        if (i != len / 8 - 1)
        {
            __z_port_printf((uint8_t *)" ", 1);
        }
    }
    __z_port_printf((uint8_t *)"\r\n", 2);
}


// 十六进制打印数组
void z_printf_uint8_arr(uint8_t arr[],int32_t size)
{
    z_fori(0,size)
    {
        z_printf("0x%02x ",arr[i]);
    }
    z_printf("\r\n");
		
		HAL_Delay(1);
}

