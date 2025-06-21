#ifndef __Z_UTIL_H
#define __Z_UTIL_H

#include <stdint.h>

// �궨�����
// ��������һЩ����ִ�����ʱʹ��
#define __Z_XXX(x...) ({do{x}while(0);})

// ��������������
//int ab = 12;
//z_printf("%d", z_concat(a, b));
#define z_concat(a, b) (a ## b)

// ������������ٽ��к�������
// #define aa 01
// z_concatenate(test, aa)();
// z_concatenate(test, 02)();
// ��ֱ���� test01() test02() test02()
#define z_concatenate(a, b) z_concat(a, b)

// forѭ��
// ����ҿ�����
#define z_fori(a,b) for(int32_t i=(a); i<(b); i++)
#define z_forj(a,b) for(int32_t j=(a); j<(b); j++)
#define z_fork(a,b) for(int32_t k=(a); k<(b); k++)

// ����ת�ַ���
// z_printf(z_str1(sasas));
// ��ӡ "sasas"
#define z_str1(R)  #R

// �궨������ת�ַ���
// #define SAMPLE_RATE 16000
// z_printf(z_str2(SAMPLE_RATE));
// �����ӡ"16000"
#define z_str2(R)  z_str1(R)

// һ�������ֽ�ƴ��short
// z_printf("%x", z_merage2(0x2b, 0xa1));
// ��ӡ��� "2ba1"
#define z_merage2(h,l) ((((uint16_t)h) << 8) | l)

typedef void (*z_t_process_var_to_str)(uint8_t[], int32_t);
// ��ʽ���ַ���
void z_var_to_str(z_t_process_var_to_str z_process_var_to_str, const char * sFormat, ...);

// ��������������ͬ��ʽ�ı�����ע���������ָ�룩
//int32_t c = 555;
//int32_t d = 666;
//z_swap(&c, &d);
#define z_swap(a,b)  ({\
(void)(&(*a) == &(*b));	\
typeof(*a) __tmp = *a;\
*a = *b; \
*b = __tmp;})

// ��ת����
#define z_reverse_arr(arr, arr_size)    __Z_XXX(                                                            \
z_fori(0,arr_size/2){                                                                            \
z_swap(&arr[i],&arr[arr_size-1-i]);                                                        \
}                                                                                            \
)

// ��ת�����ָ������ ǰ���Ǳ�����
#define z_reverse_arr_begin_and_end(arr, start, end)    __Z_XXX(                                                \
int left = start;                                                                            \
int right = end;                                                                            \
do{                                                                                            \
z_swap(&(arr[left]),&(arr[right]));                                                        \
left++;                                                                                    \
right--;                                                                                \
}while(left < right);                                                                        \
)

// Byte��ת
#define z_bswap_8(x) ((x) & 0xff)
#define z_bswap_16(x) ((z_bswap_8(x) << 8) | z_bswap_8((x) >> 8))
#define z_bswap_32(x) ((z_bswap_16(x) << 16) | z_bswap_16((x) >> 16))
#define z_bswap_64(x) ((z_bswap_32(x) << 32) | z_bswap_32((x) >> 32))

uint8_t z_reverse_8bit(uint8_t n);

uint16_t z_reverse_16bit(uint16_t n);

uint32_t z_reverse_32bit(uint32_t n);

uint64_t z_reverse_64bit(uint64_t n);


uint64_t z_systick_get(void);
// ������������ʱ��
#define Z_MEASURE_FUN_TIME(func) __Z_XXX(\
uint64_t _start = z_systick_get(); \
func(); \
uint64_t _end = z_systick_get(); \
z_printf("%s took %" PRIu64 " ms\n", #func, _end - _start); \
)

#endif
