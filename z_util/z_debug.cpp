#include "z_debug.h"
// z_debugº¯Êý

#include "z_include.h"

#ifdef Z_DEBUG

uint8_t z_debug_arr[__Z_DEBUG_SIZE] = {0};
void __z_debug_arr_add__func(uint8_t buf[], int32_t len)
{
    static int32_t index = 0;
    if (strcmp((char *)buf, "__z_debug_arr_add end") == 0)
    {
        __z_port_printf(z_debug_arr, index);
        index = 0;
        return;
    }
    strcpy((char *)z_debug_arr + index, (char *)buf);
    index += len;
}

#endif
