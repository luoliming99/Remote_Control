#ifndef __COMMON_H
#define __COMMON_H

#include "ch32f20x.h"

#define FLY                     1
#define CAR                     2

#define BATT_VOLT_FLY_LIMIT     320             /* �ɿص�ص�ѹ������� */    
#define BATT_VOLT_CAR_LIMIT     320             /* ƽ�⳵��ص�ѹ������� */
#define BATT_VOLT_LOCAL_LIMIT   360             /* ң������ص�ѹ������� */

extern uint16_t PRODUCT;
extern uint16_t BATT_VOLT_REMOTE_LIMIT;

#define SMG_NUM     4

typedef enum
{
    ON = 0,
    OFF,
    TOGGLE
} sw_status_e;

#define BATT_FILTER_NUM         10

float batt_aver_filter(float data_in);
void int2str(int num, char *str);   /* 10���� */
void display_batt(uint16_t batt_remote);
    
#endif
