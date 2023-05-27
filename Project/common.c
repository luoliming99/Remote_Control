#include "common.h"

uint16_t    PRODUCT                 = FLY;                  /* ��Ʒ���壺FLY - �����������CAR - ����ƽ�⳵ */
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
void int2str(int num, char *str)   /* 10���� */
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
    
    /* ����õ��˵�����ַ���������������õ�������ַ��� */
    if (str[0] == '-')   /* ���Ų��õ���  */
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
