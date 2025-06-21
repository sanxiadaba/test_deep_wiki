#include "z_test.h"
// 函数功能测试

#include "z_include.h"


#ifdef Z_DEBUG_TEST

#define __z_test_info() z_printf("\r\nFile :%s,Func :%s,Line: %d:\r\n",__FILE__,__func__,__LINE__)


void z_test_01(void)
{
    z_printf("this is test_01\r\n");
}


void z_test_02(void)
{
    z_printf("this is test_02\r\n");
}


void z_test_z_printf(void)
{
    __z_test_info();
    z_printf("this is %f\r\n", 3.14);
}


void z_test_concat(void)
{
    __z_test_info();
    int32_t ab = 12;
    z_printf("%" PRIi32 "\r\n", z_concat(a, b));
}


void z_test_CONCATENATE(void)
{
    __z_test_info();
#define AA 01
    z_concatenate(z_test_, AA)();
    z_concatenate(z_test_, 02)();
}


void z_test_fori(void)
{
    __z_test_info();
    z_fori(0, 3)
    {
        z_printf("%" PRIi32 " ", i);
    }
    z_printf("\r\n");
}


void z_test_forj(void)
{
    __z_test_info();
    z_forj(0, 3)
    {
        z_printf("%" PRIi32 " ", j);
    }
    z_printf("\r\n");
}


void z_test_fork(void)
{
    __z_test_info();
    z_fork(0, 3)
    {
        z_printf("%" PRIi32 " ", k);
    }
    z_printf("\r\n");
}


void z_test_debug(void)
{
    __z_test_info();
    z_debug("hello\r\n");
    z_printf("\r\n");
}


void z_test_str1(void)
{
    __z_test_info();
    z_printf(z_str1(sasssas));
    z_printf("\r\n");
}


void z_test_str2(void)
{
    __z_test_info();
#define SAMPLE_RATE 16000
    z_printf(z_str2(SAMPLE_RATE));
    z_printf("\r\n");
}


void z_test_swap(void)
{
    __z_test_info();
    int32_t c = 555;
    int32_t d = 666;
    z_swap(&c, &d);
    z_printf("c is %" PRIi32 ",d is %" PRIi32 "\r\n", c, d);
}


void z_test_min_max(void)
{
    __z_test_info();
    int32_t c = 555;
    int32_t d = 666;
    z_printf("max is %" PRIi32 ",min is %" PRIi32 "\r\n", z_math_max_two_num(c, d), z_math_min_two_num(c, d));
}


void z_test_merge2(void)
{
    __z_test_info();
    uint16_t a = z_merage2(0x2b, 0xa1);
    z_printf("0x%x,%" PRIi32 "\r\n", a, a );
}


void z_test_average(void)
{
    __z_test_info();
    z_printf("%" PRIi32 "\r\n", z_math_average_two_num(555, 666) );
}


void z_test_odd(void)
{
    __z_test_info();
    int32_t a = 111;
    if (z_math_is_odd_num(a)==Z_OK)
    {
        z_printf("a is odd\r\n");
    }
}


void z_test_reverse_arr(void)
{
    __z_test_info();
    int32_t arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    z_fori (0,10)
    {
        z_printf("%" PRIi32 " ", arr[i]);
    }
    z_printf("\r\n");
    z_reverse_arr(arr, 10);
    z_fori (0,10)
    {
        z_printf("%" PRIi32 " ", arr[i]);
    }
    z_printf("\r\n");
}


void z_test_reverse_arr_begin_and_end(void)
{
    __z_test_info();
    int32_t arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    z_fori (0,10)
    {
        z_printf("%" PRIi32 " ", arr[i]);
    }
    z_printf("\r\n");
    z_reverse_arr_begin_and_end(arr, 2, 7);
    z_fori (0,10)
    {
        z_printf("%" PRIi32 " ", arr[i]);
    }
    z_printf("\r\n");
}


void z_test_bswap(void)
{
    __z_test_info();
    uint8_t a = 0x01;
    uint16_t b = 0x0102;
    uint32_t c = 0x01020304;
    uint64_t d = 0x0102030405060708;
    z_printf("a is 0x%x,b is 0x%x,c is 0x%x,d is 0x%x\r\n", a, b, c, d);
    a = z_bswap_8(a);
    b = z_bswap_16(b);
    c = z_bswap_32(c);
    d = z_bswap_64(d);
    z_printf("a is 0x%x,b is 0x%x,c is 0x%x,d is 0x%x\r\n", a, b, c, d);
}


void z_test_get_float(void)
{
    __z_test_info();
    float a = -3.1422;
    int32_t b = z_math_get_float_zheng(a);
    int32_t c = z_math_get_float_xiao(a, 3);
		int32_t d = z_math_get_float_xiao(a, 4);
    z_printf("b is %" PRIi32 ",c is %" PRIi32 ",d is %" PRIi32 "\r\n", b, c ,d);
}


void z_test_quick_sort_int16_t(void)
{
    __z_test_info();
    int16_t arr[10] = {4, 6, 3, 9, 5, 2, 8, 7, 10, 1};
    z_fori (0,10)
    {
        z_printf("%" PRIi16 " ", arr[i]);
    }
    z_printf("\r\n");
    z_sort_quick_int16_t(arr, 0, 9);
    z_fori (0,10)
    {
        z_printf("%" PRIi16 " ", arr[i]);
    }
    z_printf("\r\n");
}


void z_test_quick_sort_int32_t(void)
{
    __z_test_info();
    int32_t arr[10] = {4, 6, 3, 9, 5, 2, 8, 7, 10, 1};
    z_fori (0,10)
    {
        z_printf("%" PRIi32 " ", arr[i]);
    }
    z_printf("\r\n");
    z_sort_quick_int32_t(arr, 0, 9);
    z_fori (0,10)
    {
        z_printf("%" PRIu32 " ", arr[i]);
    }
    z_printf("\r\n");
}


void test_i16_filter(void)
{
    __z_test_info();
    int16_t arr[10] = {4, 6, 3, 9, 5, 2, 8, 7, 10, 1};
    z_printf("%" PRIi16 "\r\n", z_filter_get_average_int16(arr, 10));
    z_printf("%" PRIi16 "\r\n", z_filter_get_midvalue_average_int16(arr, 10));
}


void test_i32_filter(void)
{
    __z_test_info();
    int32_t arr[10] = {4, 6, 3, 9, 5, 2, 8, 7, 10, 1};
    z_printf("%" PRIi32 "\r\n", z_filter_get_average_int32(arr, 10));
    z_printf("%" PRIi32 "\r\n", z_filter_get_midvalue_average_int32(arr, 10));
}


void z_test_print_bin(void)
{
    __z_test_info();
    uint8_t a = 0x01;
    uint16_t b = 0x0102;
    uint32_t c = 0x01020304;
    uint64_t d = 0x0102030405060708;
    z_print_bin(a, 8);
    z_print_bin(b, 16);
    z_print_bin(c, 32);
    z_print_bin(d, 64);
    z_printf("\r\n");
}


void z_test_revere_bit(void)
{
    __z_test_info();
    uint8_t a = 0x01;
    uint16_t b = 0x0102;
    uint32_t c = 0x01020304;
    uint64_t d = 0x0102030405060708;
    z_print_bin(a, 8);
    z_print_bin(b, 16);
    z_print_bin(c, 32);
    z_print_bin(d, 64);
    a = z_reverse_8bit(a);
    b = z_reverse_16bit(b);
    c = z_reverse_32bit(c);
    d = z_reverse_64bit(d);
    z_print_bin(a, 8);
    z_print_bin(b, 16);
    z_print_bin(c, 32);
    z_print_bin(d, 64);
}


void z_test_crc16(void)
{
    __z_test_info();
    uint8_t arr[4] = {1, 2, 3, 4};
// 其crc校验为 0x2ba1
    z_printf("%x", (uint8_t)(z_math_crc16(arr, 4) >> 8)); // 打印2b
    z_printf("%x", (uint8_t)z_math_crc16(arr, 4));				// 打印a1
    z_printf("\r\n");
}


void z_test_point_2_x_y(void)
{
    __z_test_info();
    z_math_point_2_x_y p1 = {1, 3};
    z_math_point_2_x_y p2 = {2, 4};
    z_math_point_2_k_b k_b = z_math_calculate_k_b(p1, p2);
    z_printf("k is %f,b is %f\r\n", k_b.k, k_b.b);
}


void z_test_judge_0_1_num(void)
{
    __z_test_info();
    uint8_t arr[8] = {0, 1, 0, 1, 0, 1, 0, 1};
    z_printf("%" PRIu8 "\r\n", z_math_judge_0_1_num(arr, 8, 1, 4));
    z_printf("%" PRIu8 "\r\n", z_math_judge_0_1_num(arr, 8, 1, 5));
}


void z_test_ringbuffer(void)
{
    __z_test_info();
    uint8_t arr[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    // 8个字节大小的循环队列
    z_ring_buffer_stu* test_ptr = z_ring_buffer_init(8 * sizeof(uint8_t));
    //查看容量
    z_printf("capacity is %" PRIu32 "\r\n", z_ring_buffer_get_capacity(test_ptr));
    // 循环队列写进去两个字节
    z_ring_buffer_write(test_ptr, arr, 2);
    // 再次查看容量、可用num
    z_printf("capacity is %" PRIu32 "\r\n", z_ring_buffer_get_capacity(test_ptr));
    z_printf("read num is %" PRIu8 "\r\n", z_ring_buffer_get_read_num(test_ptr));
    // 读出一个数据
    uint8_t get_arr[1] = {0};
    z_ring_buffer_read_buffer(test_ptr, get_arr, 1);
    z_printf("read is %" PRIu8 "\r\n", get_arr[0]);
    z_printf("read num is %" PRIu32 "\r\n", z_ring_buffer_get_read_num(test_ptr));
    // 再读一个数据
    z_ring_buffer_read_buffer(test_ptr, get_arr, 1);
    z_printf("read is %" PRIu8 "\r\n", get_arr[0]);
    z_printf("read num is %" PRIu32 "\r\n", z_ring_buffer_get_read_num(test_ptr));
    // 写进去数据、读出来
    uint8_t get_all_arr[8] = {0};
    z_ring_buffer_write(test_ptr, arr, 12);
    z_ring_buffer_read_buffer(test_ptr, get_all_arr, z_ring_buffer_get_read_num(test_ptr));
    z_fori (0,8)
    {
        z_printf("%" PRIu8 " ", get_all_arr[i]);
    }
    z_printf("\r\n");
    z_printf("read num is %" PRIu8 "\r\n", z_ring_buffer_get_read_num(test_ptr));
    z_printf("\r\n");
    z_ring_buffer_free(&test_ptr);
}


void z_test_get_digit_left_to_right()
{
    __z_test_info();
    // 测试用例
    int32_t test_cases[10][3] =
    {
        {123, 1, 1},    // 123 的第 1 位是 1
        {123, 2, 2},    // 123 的第 2 位是 2
        {123, 3, 3},    // 123 的第 3 位是 3
        {-456, 2, 5},   // -456 的第 2 位是 5
        {789, 4, 0},    // 789 的第 4 位超出范围，返回 0
        {0, 1, 0},      // 0 的第 1 位是 0
        {12345, 5, 5},  // 12345 的第 5 位是 5
        {12345, 6, 0},  // 12345 的第 6 位超出范围，返回 0
        {-12345, 3, 3}, // -12345 的第 3 位是 3
        {987654321, 9, 1} // 987654321 的第 9 位是 1
    };
 
    // 运行测试
    z_fori(0, 10)
    {
        int32_t num = test_cases[i][0];
        int32_t n = test_cases[i][1];
        int32_t expected = test_cases[i][2];
        int32_t result = z_math_get_digit_left_to_right(num, n);
        // 打印结果
				
        z_printf("Test case %d: num = %d, n = %d, expected = %d, result = %d -> %s\n",
                 i + 1, num, n, expected, result,
                 (result == expected) ? "PASS" : "FAIL");
    }
    return;
}

void z_test_get_digit_right_to_left()
{
    // 测试用例（格式：{数字, 位数n, 期望结果}）
    int32_t test_cases[10][3] =
    {
        {123, 1, 3},    // 123 的右起第1位（个位）是 3
        {123, 2, 2},    // 123 的右起第2位（十位）是 2
        {123, 3, 1},    // 123 的右起第3位（百位）是 1
        {-456, 2, 5},   // -456 的右起第2位是 5
        {789, 4, 0},    // 789 的右起第4位超出范围，返回 0
        {0, 1, 0},      // 0 的右起第1位是 0
        {12345, 5, 1},  // 12345 的右起第5位是 1
        {12345, 6, 0},  // 12345 的右起第6位超出范围，返回 0
        {-12345, 3, 3}, // -12345 的右起第3位是 3
        {987654321, 9, 9} // 987654321 的右起第9位是 9
    };

    // 运行测试
    z_fori(0, 10)
    {
        int32_t num = test_cases[i][0];
        int32_t n = test_cases[i][1];
        int32_t expected = test_cases[i][2];
        int32_t result = z_math_get_digit_right_to_left(num, n);
        // 打印结果
        z_printf("Test case %d: num = %d, n = %d, expected = %d, result = %d -> %s\n",
                 i + 1, num, n, expected, result,
                 (result == expected) ? "PASS" : "FAIL");
				bsp_delay_ms(10);
    }
    return;
}


void z_test_constrain_min_max()
{
    __z_test_info();
    // 测试用例数组
    struct
    {
        int value;
        int min;
        int max;
        int expected;
    } test_cases[] =
    {
        // 正常情况
        {5, 0, 10, 5},       // value 在范围内
        {-5, 0, 10, 0},      // value 小于 min
        {15, 0, 10, 10},     // value 大于 max
        {0, 0, 10, 0},       // value 等于 min
        {10, 0, 10, 10},     // value 等于 max
        {3, 3, 3, 3},        // min == max == value
        {2, 3, 3, 3},        // value 小于 min == max
        {4, 3, 3, 3},        // value 大于 min == max
        // 边界情况
        {0, 0, 0, 0},        // value == min == max == 0
        {-1, -1, 1, -1},     // value == min
        {1, -1, 1, 1},       // value == max
        {-2, -1, 1, -1},     // value 小于 min（负数）
        {2, -1, 1, 1},       // value 大于 max（正数）
        // 大数测试
        {1000000, 0, 1000000, 1000000},  // value == max（大数）
        {1000001, 0, 1000000, 1000000},  // value 大于 max（大数）
        {-1000000, -1000000, 0, -1000000}, // value == min（大负数）
        {-1000001, -1000000, 0, -1000000}, // value 小于 min（大负数）
        // 异常情况（min > max）
        {5, 10, 0, 10},      // min > max，返回 min
        {-5, 10, 0, 10},     // min > max，value 小于 min
        {15, 10, 0, 10},     // min > max，value 大于 min
    };
    // 运行测试
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    z_fori(0,num_tests)
    {
        int value = test_cases[i].value;
        int min = test_cases[i].min;
        int max = test_cases[i].max;
        int expected = test_cases[i].expected;
        int result = z_math_constrain_min_max(value, min, max);
        // 打印结果
        z_printf("Test case %d: value = %d, min = %d, max = %d, expected = %d, result = %d -> %s\n",
                 i + 1, value, min, max, expected, result,
                 (result == expected) ? "PASS" : "FAIL");
    }
}

void z_test_all()
{
    z_test_z_printf();
    z_test_concat();
    z_test_CONCATENATE();
    z_test_fori();
    z_test_forj();
    z_test_fork();
    z_test_debug();
    z_test_str1();
    z_test_str2();
    z_test_swap();
    z_test_min_max();
    z_test_merge2();
    z_test_average();
    z_test_odd();
    z_test_reverse_arr();
    z_test_reverse_arr_begin_and_end();
    z_test_bswap();
    z_test_get_float();
    z_test_quick_sort_int16_t();
    z_test_quick_sort_int32_t();
    test_i16_filter();
    test_i32_filter();
    z_test_print_bin();
    z_test_revere_bit();
    z_test_crc16();
    z_test_point_2_x_y();
    z_test_judge_0_1_num();
    z_test_ringbuffer();
    z_debug("hello world");
    z_test_get_digit_left_to_right();
    z_test_get_digit_right_to_left();
    z_test_constrain_min_max();
}

#else

void z_test_all()
{

}

#endif
