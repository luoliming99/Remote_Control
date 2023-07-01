#include "tone.h"

static tone_status_e   _tone_status    = TONE_STAT_INIT;
static uint16_t        _tone_cycle[]   = {0, 0};


/******************************************************************************/
void tone_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 

    /* 使能相关时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    /* GPIO配置 */
    GPIO_InitStructure.GPIO_Pin = TONE_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(TONE_PORT, &GPIO_InitStructure);
    
    play_tone(TONE_STOP);
}

void tone_set_status(tone_status_e status)
{
    if (TONE_STAT_ON == status)
    {
        GPIO_SetBits(TONE_PORT, TONE_PIN);
    }
    else if (TONE_STAT_OFF == status)
    {
        GPIO_ResetBits(TONE_PORT, TONE_PIN);
    }
    _tone_status = status;
}

tone_status_e tone_get_status(void)
{
    return _tone_status;
}

void tone_set_cycle(tone_status_e status, uint16_t cycle)
{
    if (TONE_STAT_ON == status)
    {
        _tone_cycle[0] = cycle;
    }
    else if (TONE_STAT_OFF == status)
    {
        _tone_cycle[1] = cycle;
    }
}

uint16_t tone_get_cycle(tone_status_e status)
{
    if (TONE_STAT_ON == status)
    {
        return _tone_cycle[0];
    }
    else if (TONE_STAT_OFF == status)
    {
        return _tone_cycle[1];
    }
    return 0;
}

void play_tone(tone_type_e type)
{
    switch (type)
    {
        case TONE_STOP:
            GPIO_ResetBits(TONE_PORT, TONE_PIN); _tone_status = TONE_STAT_INIT; _tone_cycle[0] = 0; _tone_cycle[1] = 0;
        break;
        
        case TONE_BUTTON:
            if (TONE_STAT_INIT == _tone_status)
            {
                _tone_status = TONE_STAT_ON; _tone_cycle[0] = 40; _tone_cycle[1] = 200;
            }
        break;
        
        case TONE_EMG:
            if (TONE_STAT_INIT == _tone_status)
            {
                _tone_status = TONE_STAT_ON; _tone_cycle[0] = 100; _tone_cycle[1] = 2000;
            }
        break;
        
        default: break;
    }
}
