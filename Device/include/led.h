#ifndef __LED_H
#define __LED_H

#include "ch32f20x.h"
#include "common.h"

#define LED_PORT    GPIOB   
#define LED_L_PIN   GPIO_Pin_0
#define LED_R_PIN   GPIO_Pin_1

typedef enum
{
    LED_L,  /* ��LED */
    LED_R   /* �ҷ�LED */
} which_led_e;


void led_init(void);
void led_set(which_led_e led, sw_status_e status);

#endif
