#ifndef __NIMING_H
#define __NIMING_H

#include "ch32f20x.h"

void niming_report_imu(uint8_t *nrf_rx_buf); 
void niming_report_data(uint8_t *nrf_rx_buf);

#endif
