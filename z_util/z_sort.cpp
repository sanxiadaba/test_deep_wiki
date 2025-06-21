#include "z_sort.h"
// �����㷨ʵ��

// ��������
void z_sort_quick_int16_t(int16_t *arr, int32_t low, int32_t high)
{
    if (low < high)
    {
        int32_t i = low;
        int32_t j = high;
        int16_t k = arr[low];
        while (i < j)
        {
            while (i < j && arr[j] >= k)    // ���������ҵ�һ��С��k����
            {
                j--;
            }

            if (i < j)
            {
                arr[i++] = arr[j];
            }

            while (i < j && arr[i] < k)     // ���������ҵ�һ�����ڵ���k����
            {
                i++;
            }

            if (i < j)
            {
                arr[j--] = arr[i];
            }
        }

        arr[i] = k;

        // �ݹ����
        z_sort_quick_int16_t(arr, low, i - 1);     // ����k���
        z_sort_quick_int16_t(arr, i + 1, high);    // ����k�ұ�
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
            while (i < j && arr[j] >= k)    // ���������ҵ�һ��С��k����
            {
                j--;
            }

            if (i < j)
            {
                arr[i++] = arr[j];
            }

            while (i < j && arr[i] < k)     // ���������ҵ�һ�����ڵ���k����
            {
                i++;
            }

            if (i < j)
            {
                arr[j--] = arr[i];
            }
        }

        arr[i] = k;

        // �ݹ����
        z_sort_quick_int32_t(arr, low, i - 1);     // ����k���
        z_sort_quick_int32_t(arr, i + 1, high);    // ����k�ұ�
    }
}
