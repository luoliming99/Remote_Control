#ifndef __KEY_H
#define __KEY_H

#include "ch32f20x.h"

#define KEY_PORT    GPIOA   
#define KEY_L       GPIO_Pin_11
#define KEY_R       GPIO_Pin_12

#define KEY_L_VAL   GPIO_ReadInputDataBit(KEY_PORT, KEY_L)
#define KEY_R_VAL   GPIO_ReadInputDataBit(KEY_PORT, KEY_R)

typedef enum
{
    NO_KEY_PRESS = 0,
    KEY_L_PRESS,
    KEY_R_PRESS,
} key_status_e;

typedef enum
{
    KEY_MODE_NORMAL = 0,
    KEY_MODE_CONTINUE,
} key_mode_e;

void key_init(void);
key_status_e key_scan(key_mode_e mode, uint32_t tick_ms);

#endif
