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

extern uint32_t g_1ms_cnt;
extern uint8_t  g_5000ms_flag;

int main( void )
{
    int ret = 0;
    
    uint8_t nrf_tx_buf[PLOAD_WIDTH_MAX] = {0};
    uint8_t nrf_rx_buf[PLOAD_WIDTH_MAX] = {0};
    
    uint16_t batt_local     = 0;    /* 遥控器电池电压，单位：10mV */
    uint16_t batt_remote    = 0;    /* 飞控或平衡车电池电压，单位：10mV */
    
    int16_t  accelerator    = 0;    /* 油门 */
    int16_t  pitch          = 0;    /* 俯仰角 */
    int16_t  roll           = 0;    /* 横滚角 */
    int16_t  yaw            = 0;    /* 偏航角 */

    int16_t  speed          = 0;    /* 行走速度，单位：r/min */
    int16_t  turn           = 0;    /* 转向速度 */ 
    
    key_status_e key_val    = NO_KEY_PRESS;
    
    char    str_num[SMG_NUM + 1]    = {0};
    char    str_volt[10]            = {"L0.00"};
    uint8_t disp_self               = 0;
    
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
            
            *(uint16_t *)&nrf_tx_buf[0] = accelerator;
            *(int16_t *)&nrf_tx_buf[2]  = pitch;
            *(int16_t *)&nrf_tx_buf[4]  = roll;
            *(uint8_t *)&nrf_tx_buf[6]  = key_val;
            
            ret = nrf24l01_tx_packet(nrf_tx_buf, 8);
        }
        else if (PRODUCT == CAR)
        {
            speed = (int16_t)(g_adc_val[0] * 200.0  / ((1 << 12) - 1) + 0.5) - 100;     /* -100~100 */        
            turn  = 2000 - (int16_t)(g_adc_val[3] * 4000.0  / ((1 << 12) - 1) + 0.5);   /* 2000~-2000 */
            
            /* 消除摇杆电位器零点偏差(0.1) */
            speed  = (speed > -10 && speed < 10) ? 0 : speed;
            turn  = (turn > -200 && turn < 200) ? 0 : turn;
            
            *(int16_t *)&nrf_tx_buf[0]  = speed;
            *(int16_t *)&nrf_tx_buf[2]  = turn;
            
            ret = nrf24l01_tx_packet(nrf_tx_buf, 4);
        }

        printf("TX:%d %d %d %d %x\r\n", *(int16_t *)&nrf_tx_buf[0], *(int16_t *)&nrf_tx_buf[2], *(int16_t *)&nrf_tx_buf[4], *(int16_t *)&nrf_tx_buf[6], ret);
        if (ret == 0)
        {
            led_set(LED_L, TOGGLE);
        }
        
        ret = nrf24l01_rx_packet(nrf_rx_buf);
        if (ret == 0)
        {
            led_set(LED_R, TOGGLE);
            if (PRODUCT == FLY)
            {
                batt_remote = *(uint16_t *)&nrf_rx_buf[0];
                pitch       = *(int16_t *)&nrf_rx_buf[2];
                roll        = *(int16_t *)&nrf_rx_buf[4];
                yaw         = *(int16_t *)&nrf_rx_buf[6];
//                printf("RX:%d %d %d %d\r\n", batt_remote, pitch, roll, yaw);
            }
            else if (PRODUCT == CAR)
            {
                batt_remote = *(uint16_t *)&nrf_rx_buf[0];
                speed = *(int16_t *)&nrf_rx_buf[2];
                turn = *(int16_t *)&nrf_rx_buf[4];
//                printf("RX:%d %d %d\r\n", batt_remote, speed, turn);
            }

            batt_local = batt_aver_filter((float)g_adc_val[4] / 4095 * 3.3 * 1.36 * 100 + 0.5);
            
            if (PRODUCT == FLY)
            {
                BATT_VOLT_REMOTE_LIMIT = BATT_VOLT_FLY_LIMIT;
            }
            else if (PRODUCT == CAR)
            {
                BATT_VOLT_REMOTE_LIMIT = BATT_VOLT_CAR_LIMIT;
            }
            
            if (g_5000ms_flag == 1)
            {
                g_5000ms_flag = 0;
                disp_self = !disp_self;
                if ((disp_self && batt_local < BATT_VOLT_LOCAL_LIMIT) ||
                    (!disp_self && batt_remote < BATT_VOLT_REMOTE_LIMIT))
                {
                    disp_set_all_flick_cycle(200);
                }
                else
                {
                    disp_set_all_flick_cycle(0);
                }
            }
            if (disp_self)
            {
                int2str(batt_local, str_num);
                str_volt[0] = 'L';
                
            }
            else
            {
                int2str(batt_remote, str_num);
                str_volt[0] = 'r';
            }
            str_volt[1] = str_num[0];
            str_volt[3] = str_num[1];
            str_volt[4] = str_num[2];
            disp_set_string(str_volt);
            
            if (batt_local < BATT_VOLT_LOCAL_LIMIT ||
                batt_remote < BATT_VOLT_REMOTE_LIMIT)
            {
                play_tone(TONE_EMG);
            }
            else
            {
                play_tone(TONE_STOP);
            }
        }
    }
}
