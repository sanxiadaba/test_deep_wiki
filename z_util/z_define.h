#ifndef __Z_DEFINE_H
#define __Z_DEFINE_H

#define PI 			3.14159265
#define INF_32	0x3f3f3f3f
#define INF_64	0x3f3f3f3f3f3f3f3f
#define Z_1K 		1024

// 开启调试打印
#define Z_DEBUG

#ifdef Z_DEBUG
#define USE_SEGGER_RTT
#endif

// 成功、失败
typedef enum
{
    Z_OK = 0,
    Z_ERROR = 1
} Z_STATE;

#define __Z_STM32F103C8T6_H
//#define __Z_PY32F002Ax5_H
//#define __Z_STM32F103ZET6_H
//#define __Z_HC32L130J8T6_H
//#define __Z_HC32L130F8UA_H

//#define Z_DEBUG_TEST

#ifndef Z_DEBUG
#undef Z_DEBUG_TEST
#endif


#endif
