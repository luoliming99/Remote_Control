#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "ch32f20x.h"


/**
 * \brief 时钟配置
 * \note ADC1 - DMA1通道1
 *       ADC3 - DMA2通道5
 */
#define ADC_CLK_CMD         RCC_APB2PeriphClockCmd
#define ADC_CLK             RCC_APB2Periph_ADC1

#define ADC_GPIO_CLK_CMD    RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK        RCC_APB2Periph_GPIOA

#define ADC_DMA_CLK_CMD     RCC_AHBPeriphClockCmd
#define ADC_DMA_CLK         RCC_AHBPeriph_DMA1

/* GPIO配置 */
#define ADC_PORT    GPIOA

#define ADC_PIN_LY  GPIO_Pin_0
#define ADC_CH_LY   ADC_Channel_0

#define ADC_PIN_LX  GPIO_Pin_1
#define ADC_CH_LX   ADC_Channel_1

#define ADC_PIN_RY  GPIO_Pin_2
#define ADC_CH_RY   ADC_Channel_2

#define ADC_PIN_RX  GPIO_Pin_3
#define ADC_CH_RX   ADC_Channel_3

#define ADC_PIN_BAT GPIO_Pin_4
#define ADC_CH_BAT  ADC_Channel_4

/* DMA配置 */
#define ADC_DMA_CH  DMA1_Channel1
#define ADC_x       ADC1    /* ADC控制器1 */
#define ADC_CH_NUM  5       /* ADC转换通道个数 */

extern uint16_t g_adc_val[];

void adc_init(void);
void adc_calibration(void);
    
#endif
