#include "z_init.h"
// ��ʼ��

#include "z_include.h"

void z_init()
{
    // ʱ�ӳ�ʼ��
    z_clk_init();
    // ���Ź���ʼ��
    z_wdt_dog_init();
		// DEBUG��ʼ��
#ifdef USE_SEGGER_RTT
		SEGGER_RTT_Init();
#endif
		// ��ʱ�����ʼ��
		 multiTimerInstall(z_systick_get);
    // bsp��ʼ��
    z_bsp_init();
}
