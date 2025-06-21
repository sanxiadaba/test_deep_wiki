#ifndef __Z_FILTER_H
#define __Z_FILTER_H

#include <stdint.h>

int16_t z_filter_get_midvalue_average_int16(int16_t arr[], int32_t arr_size);
int32_t z_filter_get_midvalue_average_int32(int32_t arr[], int32_t arr_size);
int16_t z_filter_get_average_int16(int16_t arr[], int32_t arr_size);
int32_t z_filter_get_average_int32(int32_t arr[], int32_t arr_size);

#endif
