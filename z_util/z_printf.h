#ifndef __Z_PRINTF_H
#define __Z_PRINTF_H

#include <stdint.h>

#include "z_define.h"
#include "z_util.h"


void __z_printf(uint8_t arr[], int32_t size);

#ifdef Z_DEBUG

// �Լ�ʵ�ֵ�printf
#define z_printf(x...) z_var_to_str(__z_printf,x)
#else
#define z_printf(x...)

#endif

void z_print_bin(int64_t n, int32_t len);

// ʮ�����ƴ�ӡ����
void z_printf_uint8_arr(uint8_t arr[],int32_t size);

#endif
