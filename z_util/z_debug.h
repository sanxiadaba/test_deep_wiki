#ifndef Z_DEBUG_H
#define Z_DEBUG_H

#include <stdint.h>
#include <inttypes.h>

#include "z_util.h"
#include "z_define.h"

#ifdef Z_DEBUG

#define __Z_DEBUG_SIZE 128

extern uint8_t z_debug_arr[__Z_DEBUG_SIZE];
void __z_debug_arr_add__func(uint8_t buf[], int32_t len);
#define __z_debug_arr_add(x...) __Z_XXX(z_var_to_str(__z_debug_arr_add__func, x);)
// z_debug µœ÷
#define z_debug(x...) __Z_XXX(\
__z_debug_arr_add("Tick\t\t: %" PRIu64 "\r\n",z_systick_get());\
__z_debug_arr_add("File\t\t: %s\r\n",__FILE__);\
__z_debug_arr_add("Func\t\t: %s\r\n",__func__);\
__z_debug_arr_add("Line\t\t: %d\r\n",__LINE__);\
__z_debug_arr_add("Info\t\t:");\
__z_debug_arr_add(x);\
__z_debug_arr_add("\r\n");\
__z_debug_arr_add("__z_debug_arr_add end");\
\
)

#else

#define z_debug(x...)

#endif

#endif
