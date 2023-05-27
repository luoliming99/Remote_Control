#ifndef __LED_H
#define __LED_H

#include "ch32f20x.h"
#include "common.h"

#define LED_PORT    GPIOB   
#define LED_L_PIN   GPIO_Pin_0
#define LED_R_PIN   GPIO_Pin_1

typedef enum
{
    LED_L,  /* ×ó·½LED */
    LED_R   /* ÓÒ·½LED */
} which_led_e;


void led_init(void);
void led_set(which_led_e led, sw_status_e status);

#endif
