#ifndef __Z_MATH_H
#define __Z_MATH_H

#include <stdint.h>
#include <stdbool.h>

#include "z_define.h"


// ����10��n�η�
uint32_t z_math_10_n(int32_t n);

// ��ȡ��������������
int32_t z_math_get_float_zheng(float num);

// ��ȡ������nλС������
int32_t z_math_get_float_xiao(float num, int32_t n);

// �������һ�ȡ���ֵĵ�nλ
uint8_t z_math_get_digit_left_to_right(int32_t num, int32_t n);

// �������һ�ȡ���ֵĵ�nλ
uint8_t z_math_get_digit_right_to_left(int32_t num, int32_t n);

// ��Ķ�ά����
typedef struct z_math_point_2_x_y
{
    float x;
    float y;
} z_math_point_2_x_y;

// ֱ�ߵ�б�ʡ��ؾ�
typedef struct z_math_point_2_k_b
{
    float k;
    float b;
} z_math_point_2_k_b;

// ����б�ʡ��ؾ�
z_math_point_2_k_b z_math_calculate_k_b(z_math_point_2_x_y p1, z_math_point_2_x_y p2);

// ����������ж��������������n���������ڵ���judge_num ���1 ��֮Ϊ0
Z_STATE z_math_judge_0_1_num(uint8_t arr[], int32_t arr_size, int32_t n, int32_t judge_num);

// CRC16У��
uint16_t z_math_crc16(uint8_t *ptr, int32_t len);

// ���ؽϴ�С���ֵĺ�
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

// �����������min��max֮��(���Ǳ�����)
#define z_math_constrain_min_max(value, min, max) ({      \
typeof(value) _value = (value);       \
typeof(min) _min = (min);             \
typeof(max) _max = (max);             \
(void) (&_value == &_min);            \
(void) (&_min == &_max);\
_min > _max ? _min : (_value < _min ? _min : (_value > _max ? _max : _value));\
})

// �ж������Ƿ���min��max֮��(���Ǳ�����)
#define z_math_judge_constrain_min_max(value, min, max) ({ \
    typeof(value) _value = (value);       \
    typeof(min) _min = (min);             \
    typeof(max) _max = (max);             \
    (void) (&_value == &_min);            \
    (void) (&_min == &_max);              \
    _min > _max ? 0 : (_value >= _min && _value <= _max); \
})

// ��������ƽ����,�����
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

// �ж��Ƿ������� odd:��ֵ�
// ������ʹ�õ� "��λ��" ���е��ж�
// "��λ��" Ҳ���� & ���ŵĹ��������� bit λ��Ϊ 1 �����Ϊ 1
// ������洢�������Զ����Ƹ�ʽ�洢��
// һ������,�����ż��,�����������ĩβ��Ȼ�� 0
// �� 1 ���� "��λ��" ,��ʵ�������ж����һλ�� 1 ���� 0
// ���һλ�� 1 �Ļ����� Z_OK(����)
#define z_math_is_odd_num(x) ({                                                                            \
Z_STATE is_odd = Z_ERROR;                                                                        \
if((x & 1) == 1) {                                                                            \
is_odd = Z_OK;                                                                            \
}                                                                                            \
is_odd;                                                                                        \
})

#endif
