#include "led.h"

/******************************************************************************/
void led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能相关时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
    /* GPIO配置 */
    GPIO_InitStructure.GPIO_Pin = LED_L_PIN | LED_R_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    led_set(LED_L, OFF);
    led_set(LED_R, OFF);
}

/******************************************************************************/
void led_set(which_led_e led, sw_status_e status)
{
    switch (led)
    {
        case LED_L:
            if (status == ON)
                GPIO_ResetBits(LED_PORT, LED_L_PIN);
            else if (status == OFF)
                GPIO_SetBits(LED_PORT, LED_L_PIN);
            else if (status == TOGGLE)
                GPIO_WriteBit(LED_PORT, LED_L_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_L_PIN)));
        break;
            
        case LED_R:
            if (status == ON)
                GPIO_ResetBits(LED_PORT, LED_R_PIN);
            else if (status == OFF)
                GPIO_SetBits(LED_PORT, LED_R_PIN);
            else if (status == TOGGLE)
                GPIO_WriteBit(LED_PORT, LED_R_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_PORT, LED_R_PIN)));
        break;
            
        default: break;
    }
}
