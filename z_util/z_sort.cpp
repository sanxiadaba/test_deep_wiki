#include "z_sort.h"
// 排序算法实现

// 快速排序
void z_sort_quick_int16_t(int16_t *arr, int32_t low, int32_t high)
{
    if (low < high)
    {
        int32_t i = low;
        int32_t j = high;
        int16_t k = arr[low];
        while (i < j)
        {
            while (i < j && arr[j] >= k)    // 从右向左找第一个小于k的数
            {
                j--;
            }

            if (i < j)
            {
                arr[i++] = arr[j];
            }

            while (i < j && arr[i] < k)     // 从左向右找第一个大于等于k的数
            {
                i++;
            }

            if (i < j)
            {
                arr[j--] = arr[i];
            }
        }

        arr[i] = k;

        // 递归调用
        z_sort_quick_int16_t(arr, low, i - 1);     // 排序k左边
        z_sort_quick_int16_t(arr, i + 1, high);    // 排序k右边
    }
}


void z_sort_quick_int32_t(int32_t *arr, int16_t low, int16_t high)
{
    if (low < high)
    {
        int16_t i = low;
        int16_t j = high;
        int32_t k = arr[low];
        while (i < j)
        {
            while (i < j && arr[j] >= k)    // 从右向左找第一个小于k的数
            {
                j--;
            }

            if (i < j)
            {
                arr[i++] = arr[j];
            }

            while (i < j && arr[i] < k)     // 从左向右找第一个大于等于k的数
            {
                i++;
            }

            if (i < j)
            {
                arr[j--] = arr[i];
            }
        }

        arr[i] = k;

        // 递归调用
        z_sort_quick_int32_t(arr, low, i - 1);     // 排序k左边
        z_sort_quick_int32_t(arr, i + 1, high);    // 排序k右边
    }
}
