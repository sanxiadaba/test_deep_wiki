#ifndef __Z_MATH_H
#define __Z_MATH_H

#include <stdint.h>
#include <stdbool.h>

#include "z_define.h"


// 返回10的n次方
uint32_t z_math_10_n(int32_t n);

// 获取浮点数整数部分
int32_t z_math_get_float_zheng(float num);

// 获取浮点数n位小数部分
int32_t z_math_get_float_xiao(float num, int32_t n);

// 从左向右获取数字的第n位
uint8_t z_math_get_digit_left_to_right(int32_t num, int32_t n);

// 从右向右获取数字的第n位
uint8_t z_math_get_digit_right_to_left(int32_t num, int32_t n);

// 点的二维坐标
typedef struct z_math_point_2_x_y
{
    float x;
    float y;
} z_math_point_2_x_y;

// 直线的斜率、截距
typedef struct z_math_point_2_k_b
{
    float k;
    float b;
} z_math_point_2_k_b;

// 计算斜率、截距
z_math_point_2_k_b z_math_calculate_k_b(z_math_point_2_x_y p1, z_math_point_2_x_y p2);

// 对数组进行判定，当数组里面的n的数量大于等于judge_num 输出1 反之为0
Z_STATE z_math_judge_0_1_num(uint8_t arr[], int32_t arr_size, int32_t n, int32_t judge_num);

// CRC16校验
uint16_t z_math_crc16(uint8_t *ptr, int32_t len);

// 返回较大、小数字的宏
#define z_math_min_two_num(x, y)  ({    \
typeof(x) _x = (x);\
typeof(y) _y = (y);\
(void) (&_x == &_y);\
_x < _y ? _x : _y;})

#define z_math_max_two_num(x, y)({    \
typeof(x) _x = (x);\
typeof(y) _y = (y);\
(void) (&_x == &_y);\
_x < _y ? _y : _x;\
})

// 将结果限制在min与max之间(都是闭区间)
#define z_math_constrain_min_max(value, min, max) ({      \
typeof(value) _value = (value);       \
typeof(min) _min = (min);             \
typeof(max) _max = (max);             \
(void) (&_value == &_min);            \
(void) (&_min == &_max);\
_min > _max ? _min : (_value < _min ? _min : (_value > _max ? _max : _value));\
})

// 判断数字是否在min与max之间(都是闭区间)
#define z_math_judge_constrain_min_max(value, min, max) ({ \
    typeof(value) _value = (value);       \
    typeof(min) _min = (min);             \
    typeof(max) _max = (max);             \
    (void) (&_value == &_min);            \
    (void) (&_min == &_max);              \
    _min > _max ? 0 : (_value >= _min && _value <= _max); \
})

// 两个数的平均数,防溢出
#define z_math_average_two_num(x, y)({    \
typeof(x) _x = (x);\
typeof(y) _y = (y);                \
typeof(y) _z;        \
(void) (&_x == &_y);               \
if ((_x >0 && _y>0) || (_x<0 && _y<0)){    \
_z = _x + (_y-_x)/2;    \
} else {    \
_z = (_x+_y)/2;    \
}                                  \
_z;                                   \
})

// 判断是否是奇数 odd:奇怪的
// 这里是使用的 "按位与" 进行的判断
// "按位与" 也就是 & 符号的规则是两个 bit 位都为 1 结果才为 1
// 计算机存储数据是以二进制格式存储的
// 一个数字,如果是偶数,其二进制数字末尾必然是 0
// 与 1 进行 "按位与" ,其实本质是判断最后一位是 1 还是 0
// 最后一位是 1 的话返回 Z_OK(奇数)
#define z_math_is_odd_num(x) ({                                                                            \
Z_STATE is_odd = Z_ERROR;                                                                        \
if((x & 1) == 1) {                                                                            \
is_odd = Z_OK;                                                                            \
}                                                                                            \
is_odd;                                                                                        \
})

#endif
