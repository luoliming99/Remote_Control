#include "key.h"
#include "bsp_systick.h"
#include "common.h"
#include "bsp_flash.h"

/******************************************************************************/
void key_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 

	/* ʹ�����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    /* GPIO���� */
    GPIO_InitStructure.GPIO_Pin = KEY_L | KEY_R;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
    
    /* �ϵ�ʱ�������Ϊ������ģʽ���Ҽ�����Ϊƽ�⳵ģʽ�������Զ����� */
    delay_ms(10);
    if (KEY_L_VAL == 0 && KEY_R_VAL == 1)
    {
        PRODUCT = FLY;
        flash_write_data(&PRODUCT, 1);
    }
    else if (KEY_L_VAL == 1 && KEY_R_VAL == 0)
    {
        PRODUCT = CAR;
        flash_write_data(&PRODUCT, 1);
    }
    else
    {
        flash_read_data(&PRODUCT, 1);
        if (PRODUCT != FLY && PRODUCT != CAR)   /* δ���ù�ģʽ��Ĭ��ΪFLY */
        {
            PRODUCT = FLY;
            flash_write_data(&PRODUCT, 1);
        }
    }
}

/******************************************************************************/
key_status_e key_scan(key_mode_e mode, uint32_t tick_ms)
{
    static uint8_t status = 0;
    static uint8_t tick0  = 0;
    
	if (status == 0 && (KEY_L_VAL == 0 || KEY_R_VAL == 0))  /* �а������� */
	{
		tick0 = tick_ms;
        status = 1;
	}
    if (status == 1 && KEY_L_VAL == 1 && KEY_R_VAL == 1)    /* �������ɿ��� */
    {
        status = 0;
    }
    else if (status == 1 && tick_ms - tick0 > 10)
    {
        if (mode == KEY_MODE_CONTINUE)
        {
            status = 0;
        }
        else
        {
            status = 2;
        }
        if (KEY_L_VAL == 0) 
            return KEY_L_PRESS;
        else if (KEY_R_VAL == 0)
            return KEY_R_PRESS;
    }
    
    if (status == 2 && KEY_L_VAL == 1 && KEY_R_VAL == 1)  /* �����Ѿ��ɿ� */
    {
        status = 0;
    }
    
    return NO_KEY_PRESS;
}
