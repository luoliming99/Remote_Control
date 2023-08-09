#include "ch32f20x.h"
#include "bsp_systick.h"
#include "bsp_uart.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "bsp_flash.h"

#include "led.h"
#include "key.h"
#include "nrf24l01.h"
#include "disp.h"
#include "tone.h"
#include "niming.h"
#include "string.h"     /* memcpy(), strcmp() */

extern uint32_t g_1ms_cnt;
extern uint8_t  g_5000ms_flag;

int main( void )
{
    int ret = 0;
    
    uint8_t nrf_tx_buf[PLOAD_WIDTH_MAX] = {0};
    uint8_t nrf_rx_buf[PLOAD_WIDTH_MAX] = {0};

    uint16_t batt_remote    = 0;    /* 飞控或平衡车电池电压，单位：10mV */
    
    int16_t  accelerator    = 0;    /* 油门 */
    int16_t  pitch          = 0;    /* 俯仰角 */
    int16_t  roll           = 0;    /* 横滚角 */
    int16_t  yaw            = 0;    /* 偏航角 */

    int16_t  speed          = 0;    /* 行走速度，单位：r/min */
    int16_t  turn           = 0;    /* 转向速度 */ 
    
    key_status_e key_val    = NO_KEY_PRESS;
    
    systick_init();
    uart_init();
    adc_init();
    disp_init();
    tim_init();

    printf("System clock frequency: %d Hz\r\n", SystemCoreClock);
    
    led_init();
    key_init();
    tone_init();

    ret = nrf24l01_init();
    printf("nrf24l01_init %d\r\n", ret);
    nrf24l01_tx_mode();

    play_tone(TONE_BUTTON);
    if (PRODUCT == FLY)
    {
        disp_set_string("FLY");
    }
    else if (PRODUCT == CAR)
    {
        disp_set_string("CAR");
    }
    delay_ms(2000);
    
    while (1)
    { 
        key_val = key_scan(KEY_MODE_CONTINUE, g_1ms_cnt);
        if (NO_KEY_PRESS != key_val)
        {
            play_tone(TONE_BUTTON);
        }
        
        adc_calibration();
        
        if (PRODUCT == FLY)
        {
            accelerator = g_adc_val[2] * 900.0 / ((1 << 12) - 1) + 0.5;                 /* 0~900 */
            pitch       = 10 - (int16_t)(g_adc_val[0] * 20.0  / ((1 << 12) - 1) + 0.5); /* 10~-10 */
            roll        = 10 - (int16_t)(g_adc_val[1] * 20.0  / ((1 << 12) - 1) + 0.5); /* 10~-10 */
            
            /* 消除摇杆电位器零点偏差(0.1) */
            pitch       = (pitch > -1 && pitch < 1) ? 0 : pitch;
            roll        = (roll > -1 && roll < 1) ? 0 : roll;
            
            memcpy(nrf_tx_buf, (const char *)"FLY", 4);
            *(uint16_t *)&nrf_tx_buf[4] = accelerator;
            *(int16_t *)&nrf_tx_buf[6]  = pitch;
            *(int16_t *)&nrf_tx_buf[8]  = roll;
            *(uint8_t *)&nrf_tx_buf[10]  = key_val;
            
            ret = nrf24l01_tx_packet(nrf_tx_buf, 12);
        }
        else if (PRODUCT == CAR)
        {
            speed = (int16_t)(g_adc_val[0] * 200.0  / ((1 << 12) - 1) + 0.5) - 100;     /* -100~100 */        
            turn  = 2000 - (int16_t)(g_adc_val[3] * 4000.0  / ((1 << 12) - 1) + 0.5);   /* 2000~-2000 */
            
            /* 消除摇杆电位器零点偏差(0.1) */
            speed  = (speed > -10 && speed < 10) ? 0 : speed;
            turn  = (turn > -200 && turn < 200) ? 0 : turn;
            
            memcpy(nrf_tx_buf, (const char *)"CAR", 4);
            *(int16_t *)&nrf_tx_buf[4]  = speed;
            *(int16_t *)&nrf_tx_buf[6]  = turn;
            
            ret = nrf24l01_tx_packet(nrf_tx_buf, 8);
        }
        
        if (ret == 0)
        {
            led_set(LED_L, TOGGLE);
//            printf("TX:%d %d %d %d %x\r\n", *(int16_t *)&nrf_tx_buf[4], *(int16_t *)&nrf_tx_buf[6], *(int16_t *)&nrf_tx_buf[8], *(int16_t *)&nrf_tx_buf[10], ret);
        }
        
        ret = nrf24l01_rx_packet(nrf_rx_buf);
        if (ret == 0)
        {
            led_set(LED_R, TOGGLE);
            
            /* 发送姿态数据到匿名上位机 */
            niming_report_imu(nrf_rx_buf);
            niming_report_data(nrf_rx_buf);
            
            if (PRODUCT == FLY && strcmp((const char *)nrf_rx_buf, "FLZ") == 0)
            {
                batt_remote = *(uint16_t *)&nrf_rx_buf[28];
                display_batt(batt_remote);
            }
            else if (PRODUCT == CAR && strcmp((const char *)nrf_rx_buf, "CAS") == 0)
            {
                batt_remote = *(uint16_t *)&nrf_rx_buf[28];
                display_batt(batt_remote);
            }
            else
            {
                disp_set_string("LOCK");
            }
        }
    }
}
