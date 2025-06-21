#include "z_callback.h"
// 中断回调

#include "z_include.h"


static uint32_t tim_1ms_count = 0;
static uint32_t tim_3ms_count = 0;
static uint32_t tim_5ms_count = 0;
static uint32_t tim_1s_count = 0;


// 系统systick计数
static uint64_t z_systick_time = 0;


// systick指定时间中断回调
// 快进快出
static void tim_1ms_callback(void)
{

}

static void tim_3ms_callback(void)
{

}

static void tim_5ms_callback(void)
{
#ifdef USE_DEVICE_KEY
	device_key_task();
#endif
}


static void tim_1s_callback(void)
{
// 看门狗
    z_wdt_dog_feed();
}


// 1ms 一次调用
static void tim_callback(void)
{
    tim_1ms_count ++;
    if(tim_1ms_count>=1)
    {
        tim_1ms_callback();
        tim_1ms_count = 0;
    }

    tim_3ms_count ++;
    if(tim_3ms_count>=3)
    {
        tim_3ms_callback();
        tim_3ms_count = 0;
    }

    tim_5ms_count++;
    if(tim_5ms_count>=5)
    {
        tim_5ms_callback();
        tim_5ms_count = 0;
    }

    tim_1s_count++;
    if(tim_1s_count>=1000)
    {
        tim_1s_callback();
        tim_1s_count = 0;
    }
}


// 1ms中断进入
void z_ms_callback(void)
{
    z_systick_time++;
    tim_callback();
}


// systick计数
uint64_t z_systick_get()
{
    return z_systick_time;
}

