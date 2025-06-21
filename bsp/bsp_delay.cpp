#include "bsp_delay.h"
// 毫秒、微秒级阻塞延迟

#include "z_include.h"


// 通过检查systick实现延迟
// 延迟us，大于0.2s的延迟不要用
// nus最大值 233015
void bsp_delay_us(uint32_t nus)
{
    nus = z_math_max_two_num((uint32_t)0,nus-1);
    if(nus==0)
    {
        return;
    }
    uint32_t ticks = 0;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;    //LOAD的值
    ticks = nus * Z_SYSTICK_FREQUENCY_MHZ;   //需要的节拍数
    told = SysTick->VAL;    //刚进入时的计数器的值
    while (1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told) tcnt += told - tnow;    //这里注意一下SYSTICK是一个递减计数器
            else tcnt += reload - tnow + told;  //计数器向下溢出
            told = tnow;
            if(tcnt >= ticks) break;    //时间超过/等于要延迟的时间，则退出
        }
    }
}


void bsp_delay_ms(uint32_t nms)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;          // 获取重装载值
    ticks = nms * (Z_SYSTICK_FREQUENCY_MHZ * 1000); // 计算总节拍数
    told = SysTick->VAL;                      // 记录初始计数器值

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            // 计算两次读取之间的节拍差
            if (tnow < told)
            {
                tcnt += told - tnow;               // 递减未溢出
            }
            else
            {
                tcnt += reload - tnow + told;      // 处理计数器溢出
            }
            told = tnow;

            // 累计节拍数超过目标值时退出
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}
