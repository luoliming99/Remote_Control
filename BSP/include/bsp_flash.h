#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

#include "ch32f20x.h"

int flash_write_data(uint16_t halfWordBuf[], uint32_t numOfHalfWord);
void flash_read_data(uint16_t halfWordBuf[], uint32_t numOfHalfWord);

#endif
