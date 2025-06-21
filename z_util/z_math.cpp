#include "z_math.h"
// ��ѧ�����

#include "z_include.h"


// 10��n�η�(n>=0)
uint32_t z_math_10_n(int32_t n)
{
    uint32_t result = 1;
    z_fori(0,n)
    {
        result = result * 10;
    }
    return result;
}


// ��ȡ��������������
int32_t z_math_get_float_zheng(float num)
{
    return (int32_t)num;
}


// ��ȡ������nλС������
// �磬����123.456 3 ����:456
int32_t z_math_get_float_xiao(float num, int32_t n)
{
    float decimal = num - z_math_get_float_zheng(num);
    return abs((int32_t)round(decimal * z_math_10_n(n)));
}


// ����������֮���ߵ�б�ʡ��ؾ�
z_math_point_2_k_b z_math_calculate_k_b(z_math_point_2_x_y p1, z_math_point_2_x_y p2)
{		
		// б�ʡ��ؾ�
    z_math_point_2_k_b  point_2_k_b = {0, 0};
    if (p2.x - p1.x == 0)
    {
        return point_2_k_b;
    }
		// б��
    point_2_k_b.k = (p2.y - p1.y) / (p2.x - p1.x);
		// �ؾ�
    point_2_k_b.b = p2.y - point_2_k_b.k * p2.x;
    return point_2_k_b;
}


// ����������ж��������������n���������ڵ���judge_num ���Z_OK ��֮ΪZ_ERROR
Z_STATE z_math_judge_0_1_num(uint8_t arr[], int32_t arr_size, int32_t n, int32_t judge_num)
{
    int32_t count = 0;
    z_fori(0,arr_size)
    {
        if (arr[i] == n)
        {
            count++;
            if (count >= judge_num)
            {
                return Z_OK;
            }
        }
    }
    return Z_ERROR;
}


// crc16У����
// uint8_t arr[4] = {1, 2, 3, 4};
// ��crcУ��Ϊ 0x2ba1
// z_printf("%x", (uint8_t)(z_crc16(arr, 4) >> 8)); // ��ӡ2b
// z_printf("%x", (uint8_t)z_crc16(arr, 4));				// ��ӡa1
// ��λ��ǰ
uint16_t z_math_crc16(uint8_t *ptr, int32_t len)
{
    uint16_t crc = 0xFFFF;

    while (len--)
    {
        crc ^= *ptr++;
        z_fori (0,8)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}


// ��ȡ�����м�λ����λ��
// 123 -> 3
// 0 -> 0
// 34567 -> 5
uint8_t z_get_digit_nums(int32_t num)
{
    uint8_t n = 0;
    num = abs(num);
    while(num > 0)
    {
        n++;
        num /= 10;
    }
    return n;
}


// �������һ�ȡ�ڼ�λ
uint8_t z_math_get_digit_left_to_right(int32_t num, int32_t n)
{
    if (n < 1) return 0;     // n �� ��1
    if (num == 0) return 0;	 // 0ֻ��1λ

    num = abs(num);  // ������
		// ������λ��
    int32_t digits = z_get_digit_nums(num);  

    // �� n ������λ�������� 0
    if (n > digits) return 0;

    // ���� 10 �� (��λ�� - n) �η�
    uint32_t divisor = z_math_10_n(digits-n);

    return (num / divisor) % 10;
}

// ���������ȡ�ڼ�λ
uint8_t z_math_get_digit_right_to_left(int32_t num, int32_t n)
{
		if (n < 1) return 0;     // n �� ��1
    if (num == 0) return 0;	 // 0ֻ��1λ
	
    // ��������ȡ�����ֵ
    num = abs(num);
	
		 // ������λ��
		int32_t digits = z_get_digit_nums(num); 

    // �� n ������λ�������� 0
    if (n > digits) return 0;

    // ������������ȡ��ÿһλ
    z_fori(0,n-1)
    {
        num /= 10;
    }

    // ���ص�nλ����
    return num % 10;
}



