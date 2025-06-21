#include "z_filter.h"
// �˲��㷨

#include "z_include.h"

// �޳�ǰ��1/8��
#define Z_GET_MIDVALUE_AVERAGE_GET_AWAY 8


// ��ֵƽ���˲�
// ȥ����ֵ�㣬Ȼ����ƽ��
int16_t z_filter_get_midvalue_average_int16(int16_t arr[], int32_t arr_size)
{
    z_sort_quick_int16_t(arr, 0, arr_size - 1);
    int32_t all = 0;
    int32_t index_begin = arr_size / Z_GET_MIDVALUE_AVERAGE_GET_AWAY;
    z_fori (index_begin,arr_size - index_begin)
    {
        all += arr[i];
    }
    return (all / (arr_size - 2 * index_begin));
}

int32_t z_filter_get_midvalue_average_int32(int32_t arr[], int32_t arr_size)
{
    z_sort_quick_int32_t(arr, 0, arr_size - 1);
    int64_t all = 0;
    int32_t index_begin = arr_size / Z_GET_MIDVALUE_AVERAGE_GET_AWAY;
    z_fori (index_begin,arr_size - index_begin)
    {
        all += arr[i];
    }
    return (all / (arr_size - 2 * index_begin));
}


// ���������ƽ��ֵ
int16_t z_filter_get_average_int16(int16_t arr[], int32_t arr_size)
{
    int32_t all = 0;
    z_fori (0,arr_size)
    {
        all += arr[i];
    }
    return (all / arr_size);
}

int32_t z_filter_get_average_int32(int32_t arr[], int32_t arr_size)
{
    int64_t all = 0;
    z_fori (0,arr_size)
    {
        all += arr[i];
    }
    return (all / arr_size);
}
