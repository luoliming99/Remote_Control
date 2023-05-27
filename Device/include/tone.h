#ifndef __TONE_H
#define __TONE_H

#include "ch32f20x.h"
#include "common.h"

#define TONE_PORT   GPIOC   
#define TONE_PIN    GPIO_Pin_13

typedef enum
{
    TONE_STAT_INIT = 0,
    TONE_STAT_ON,
    TONE_STAT_OFF,
} tone_status_e;

typedef enum
{
    TONE_STOP = 0,
    TONE_BUTTON,        /* °´¼üÒô */
    TONE_EMG            /* ±¨¾¯ */
} tone_type_e;

void tone_init(void);

void tone_set_status(tone_status_e status);
tone_status_e tone_get_status(void);
void tone_set_cycle(tone_status_e status, uint16_t cycle);
uint16_t tone_get_cycle(tone_status_e status);
void play_tone(tone_type_e type);

#endif
