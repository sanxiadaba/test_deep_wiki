#include "z_math.h"
// 数学计算库

#include "z_include.h"


// 10的n次方(n>=0)
uint32_t z_math_10_n(int32_t n)
{
    uint32_t result = 1;
    z_fori(0,n)
    {
        result = result * 10;
    }
    return result;
}


// 获取浮点数整数部分
int32_t z_math_get_float_zheng(float num)
{
    return (int32_t)num;
}


// 获取浮点数n位小数部分
// 如，输入123.456 3 返回:456
int32_t z_math_get_float_xiao(float num, int32_t n)
{
    float decimal = num - z_math_get_float_zheng(num);
    return abs((int32_t)round(decimal * z_math_10_n(n)));
}


// 计算两个点之间线的斜率、截距
z_math_point_2_k_b z_math_calculate_k_b(z_math_point_2_x_y p1, z_math_point_2_x_y p2)
{		
		// 斜率、截距
    z_math_point_2_k_b  point_2_k_b = {0, 0};
    if (p2.x - p1.x == 0)
    {
        return point_2_k_b;
    }
		// 斜率
    point_2_k_b.k = (p2.y - p1.y) / (p2.x - p1.x);
		// 截距
    point_2_k_b.b = p2.y - point_2_k_b.k * p2.x;
    return point_2_k_b;
}


// 对数组进行判定，当数组里面的n的数量大于等于judge_num 输出Z_OK 反之为Z_ERROR
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


// crc16校验码
// uint8_t arr[4] = {1, 2, 3, 4};
// 其crc校验为 0x2ba1
// z_printf("%x", (uint8_t)(z_crc16(arr, 4) >> 8)); // 打印2b
// z_printf("%x", (uint8_t)z_crc16(arr, 4));				// 打印a1
// 高位在前
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


// 获取数字有几位整数位数
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


// 从左向右获取第几位
uint8_t z_math_get_digit_left_to_right(int32_t num, int32_t n)
{
    if (n < 1) return 0;     // n 需 ≥1
    if (num == 0) return 0;	 // 0只有1位

    num = abs(num);  // 处理负数
		// 计算总位数
    int32_t digits = z_get_digit_nums(num);  

    // 若 n 超过总位数，返回 0
    if (n > digits) return 0;

    // 计算 10 的 (总位数 - n) 次方
    uint32_t divisor = z_math_10_n(digits-n);

    return (num / divisor) % 10;
}

// 从右向左获取第几位
uint8_t z_math_get_digit_right_to_left(int32_t num, int32_t n)
{
		if (n < 1) return 0;     // n 需 ≥1
    if (num == 0) return 0;	 // 0只有1位
	
    // 处理负数，取其绝对值
    num = abs(num);
	
		 // 计算总位数
		int32_t digits = z_get_digit_nums(num); 

    // 若 n 超过总位数，返回 0
    if (n > digits) return 0;

    // 从右向左依次取出每一位
    z_fori(0,n-1)
    {
        num /= 10;
    }

    // 返回第n位数字
    return num % 10;
}



