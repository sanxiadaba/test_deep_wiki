#include "z_init.h"
// 初始化

#include "z_include.h"

void z_init()
{
    // 时钟初始化
    z_clk_init();
    // 看门狗初始化
    z_wdt_dog_init();
		// DEBUG初始化
#ifdef USE_SEGGER_RTT
		SEGGER_RTT_Init();
#endif
		// 定时任务初始化
		 multiTimerInstall(z_systick_get);
    // bsp初始化
    z_bsp_init();
}
