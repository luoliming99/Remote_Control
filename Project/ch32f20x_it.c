/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32f20x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/08/08
* Description        : Main Interrupt Service Routines.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "ch32f20x_it.h"
#include "bsp_uart.h"
#include "bsp_systick.h"
#include "bsp_tim.h"
#include "disp.h"
#include "tone.h"
#include "common.h"

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      MemManage_Handler
 *
 * @brief   This function handles Memory Manage exception.
 *
 * @return  none
 */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      BusFault_Handler
 *
 * @brief   This function handles Bus Fault exception.
 *
 * @return  none
 */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      UsageFault_Handler
 *
 * @brief   This function handles Usage Fault exception.
 *
 * @return  none
 */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      SVC_Handler
 *
 * @brief   This function handles SVCall exception.
 *
 * @return  none
 */
void SVC_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      DebugMon_Handler
 *
 * @brief   This function handles Debug Monitor exception.
 *
 * @return  none
 */
void DebugMon_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      PendSV_Handler
 *
 * @brief   This function handles PendSVC exception.
 *
 * @return  none
 */
void PendSV_Handler(void)
{
  while (1)
  {
  }
}


void SysTick_Handler(void)
{
    g_systick_cnt++;
    if (g_systick_cnt == 1000000)
    {
        g_systick_cnt = 0;
    }
}

void DEBUG_USART_IRQHandler(void) 
{
    uint8_t ch;
    
    if (USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET)
    {        
        ch = USART_ReceiveData(DEBUG_USART);
        USART_SendData(DEBUG_USART, ch);    
    }
}

uint32_t g_1ms_cnt      = 0;
uint8_t  g_5000ms_flag  = 0;

void  TIM_IRQHandler(void)
{
    uint8_t i = 0;
    
    if (TIM_GetITStatus(TIM_x, TIM_IT_Update) != RESET) 
    {
        if ((g_1ms_cnt % 5) == 0)
        {
            disp_code();
        }

        for (i = 0; i < SMG_NUM; i++)
        {
            if (disp_get_flick_cycle(i) != 0 && (g_1ms_cnt % disp_get_flick_cycle(i)) == 0)
            {
                disp_set_flick_status(i, !disp_get_flick_status(i));
            }
        }

        if (TONE_STAT_ON == tone_get_status())
        {
            if (0 != tone_get_cycle(TONE_STAT_ON))
            {
                tone_set_status(TONE_STAT_ON);
                tone_set_cycle(TONE_STAT_ON, tone_get_cycle(TONE_STAT_ON) - 1);
            }
            else
            {
                tone_set_status(TONE_STAT_OFF);
            }
        } 
        else if (TONE_STAT_OFF == tone_get_status())
        {
            if (0 != tone_get_cycle(TONE_STAT_OFF))
            {
                tone_set_cycle(TONE_STAT_OFF, tone_get_cycle(TONE_STAT_OFF) - 1);
            }
            else
            {
                tone_set_status(TONE_STAT_INIT);
            }
        }
        
        if ((g_1ms_cnt % 5000) == 0)
        {
            g_5000ms_flag = 1;
        }
        
        g_1ms_cnt++;
        if (g_1ms_cnt == 1000000)
        {
            g_1ms_cnt = 0;
        }

        TIM_ClearITPendingBit(TIM_x , TIM_FLAG_Update);
    }
}


