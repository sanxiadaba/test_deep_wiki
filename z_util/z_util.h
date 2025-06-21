#ifndef __Z_UTIL_H
#define __Z_UTIL_H

#include <stdint.h>

// 宏定义包裹
// 当里面是一些复杂执行语句时使用
#define __Z_XXX(x...) ({do{x}while(0);})

// 将两个参数连接
//int ab = 12;
//z_printf("%d", z_concat(a, b));
#define z_concat(a, b) (a ## b)

// 将两参数组合再进行函数调用
// #define aa 01
// z_concatenate(test, aa)();
// z_concatenate(test, 02)();
// 会分别调用 test01() test02() test02()
#define z_concatenate(a, b) z_concat(a, b)

// for循环
// 左闭右开区间
#define z_fori(a,b) for(int32_t i=(a); i<(b); i++)
#define z_forj(a,b) for(int32_t j=(a); j<(b); j++)
#define z_fork(a,b) for(int32_t k=(a); k<(b); k++)

// 变量转字符串
// z_printf(z_str1(sasas));
// 打印 "sasas"
#define z_str1(R)  #R

// 宏定义数字转字符串
// #define SAMPLE_RATE 16000
// z_printf(z_str2(SAMPLE_RATE));
// 输出打印"16000"
#define z_str2(R)  z_str1(R)

// 一个两个字节拼接short
// z_printf("%x", z_merage2(0x2b, 0xa1));
// 打印输出 "2ba1"
#define z_merage2(h,l) ((((uint16_t)h) << 8) | l)

typedef void (*z_t_process_var_to_str)(uint8_t[], int32_t);
// 格式化字符串
void z_var_to_str(z_t_process_var_to_str z_process_var_to_str, const char * sFormat, ...);

// 交换任意两个相同格式的变量（注：传入的是指针）
//int32_t c = 555;
//int32_t d = 666;
//z_swap(&c, &d);
#define z_swap(a,b)  ({\
(void)(&(*a) == &(*b));	\
typeof(*a) __tmp = *a;\
*a = *b; \
*b = __tmp;})

// 翻转数组
#define z_reverse_arr(arr, arr_size)    __Z_XXX(                                                            \
z_fori(0,arr_size/2){                                                                            \
z_swap(&arr[i],&arr[arr_size-1-i]);                                                        \
}                                                                                            \
)

// 翻转数组的指定区间 前后都是闭区间
#define z_reverse_arr_begin_and_end(arr, start, end)    __Z_XXX(                                                \
int left = start;                                                                            \
int right = end;                                                                            \
do{                                                                                            \
z_swap(&(arr[left]),&(arr[right]));                                                        \
left++;                                                                                    \
right--;                                                                                \
}while(left < right);                                                                        \
)

// Byte翻转
#define z_bswap_8(x) ((x) & 0xff)
#define z_bswap_16(x) ((z_bswap_8(x) << 8) | z_bswap_8((x) >> 8))
#define z_bswap_32(x) ((z_bswap_16(x) << 16) | z_bswap_16((x) >> 16))
#define z_bswap_64(x) ((z_bswap_32(x) << 32) | z_bswap_32((x) >> 32))

uint8_t z_reverse_8bit(uint8_t n);

uint16_t z_reverse_16bit(uint16_t n);

uint32_t z_reverse_32bit(uint32_t n);

uint64_t z_reverse_64bit(uint64_t n);


uint64_t z_systick_get(void);
// 测量函数运行时间
#define Z_MEASURE_FUN_TIME(func) __Z_XXX(\
uint64_t _start = z_systick_get(); \
func(); \
uint64_t _end = z_systick_get(); \
z_printf("%s took %" PRIu64 " ms\n", #func, _end - _start); \
)

#endif
