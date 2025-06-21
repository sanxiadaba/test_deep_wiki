#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

#include <stdint.h>

void bsp_flash_init(void);

uint32_t bsp_flash_write(uint8_t buf[],uint32_t size);

void bsp_flash_read(uint8_t buf[],uint32_t size);

void bsp_flash_test(void);

#endif
