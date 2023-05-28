#include "common.h"
#include "bsp_adc.h"
#include "disp.h"
#include "tone.h"

uint16_t    PRODUCT                 = FLY;                  /* 产品定义：FLY - 四轴飞行器，CAR - 两轮平衡车 */
uint16_t    BATT_VOLT_REMOTE_LIMIT  = BATT_VOLT_FLY_LIMIT; 


/******************************************************************************/
float batt_aver_filter(float data_in)
{
    static float buf[BATT_FILTER_NUM] = {0};
    static uint8_t addr = 0;
    static uint8_t full = 0;
    float sum = 0, data_out;
    uint8_t i;
    
    buf[addr] = data_in;
    
    for (i = 0; i < BATT_FILTER_NUM; i++)
    {
        sum += buf[i];
    }
    
    addr++;
    if (addr == BATT_FILTER_NUM)
    {
        addr = 0;
        full = 1;
    }
    
    if (full == 0)
    {
        data_out = sum / addr;
    }
    else
    {
        data_out = sum / BATT_FILTER_NUM;
    }
    return data_out;
}

/******************************************************************************/
void int2str(int num, char *str)   /* 10进制 */
{
    int i = 0, j = 0;
    char tmp;
     
    if (num < 0) 
    {
        num = -num;
        str[i++] = '-';
    } 

    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num);
    
    str[i] = '\0';
    
    /* 上面得到了倒序的字符串，接下来反序得到正序的字符串 */
    if (str[0] == '-')   /* 负号不用调整  */
    {
        j = 1;
        i++;
    }
    for (; j < i / 2; j++)
    {
        tmp = str[j];
        str[j] = str[i-1-j];
        str[i-1-j] = tmp;
    } 
}

extern uint8_t  g_5000ms_flag;

void display_batt(uint16_t batt_remote)
{
    uint16_t batt_local = 0;    /* 遥控器电池电压，单位：10mV */
    
    char            str_num[SMG_NUM + 1]    = {0};
    char            str_volt[10]            = {"L0.00"};
    static uint8_t  disp_self               = 0;
    
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
