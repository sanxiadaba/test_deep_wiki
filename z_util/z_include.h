#ifndef __Z_INCLUDE_H
#define __Z_INCLUDE_H

// ��׼��
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include <stdio.h>

// �������ܿ�
#include "z_define.h"
#include "z_filter.h"
#include "z_math.h"
#include "z_printf.h"
#include "z_ringbuffer.h"
#include "z_sort.h"
#include "z_util.h"
#include "z_test.h"
#include "z_callback.h"
#include "z_init.h"
#include "z_while.h"
#include "z_debug.h"
#include "z_port.h"

// �õ��ĵ�������
#ifdef USE_SEGGER_RTT
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#endif
#include "MultiTimer.h"

// bsp��
#include "bsp_include.h"

#endif
