#include "bsp_delay.h"
// ���롢΢�뼶�����ӳ�

#include "z_include.h"


// ͨ�����systickʵ���ӳ�
// �ӳ�us������0.2s���ӳٲ�Ҫ��
// nus���ֵ 233015
void bsp_delay_us(uint32_t nus)
{
    nus = z_math_max_two_num((uint32_t)0,nus-1);
    if(nus==0)
    {
        return;
    }
    uint32_t ticks = 0;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;    //LOAD��ֵ
    ticks = nus * Z_SYSTICK_FREQUENCY_MHZ;   //��Ҫ�Ľ�����
    told = SysTick->VAL;    //�ս���ʱ�ļ�������ֵ
    while (1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told) tcnt += told - tnow;    //����ע��һ��SYSTICK��һ���ݼ�������
            else tcnt += reload - tnow + told;  //�������������
            told = tnow;
            if(tcnt >= ticks) break;    //ʱ�䳬��/����Ҫ�ӳٵ�ʱ�䣬���˳�
        }
    }
}


void bsp_delay_ms(uint32_t nms)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;          // ��ȡ��װ��ֵ
    ticks = nms * (Z_SYSTICK_FREQUENCY_MHZ * 1000); // �����ܽ�����
    told = SysTick->VAL;                      // ��¼��ʼ������ֵ

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            // �������ζ�ȡ֮��Ľ��Ĳ�
            if (tnow < told)
            {
                tcnt += told - tnow;               // �ݼ�δ���
            }
            else
            {
                tcnt += reload - tnow + told;      // ������������
            }
            told = tnow;

            // �ۼƽ���������Ŀ��ֵʱ�˳�
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}
