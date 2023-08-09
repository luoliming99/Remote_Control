#include "niming.h"
#include "bsp_uart.h"

/**
 * \brief �ϱ����ݸ�������λ��
 * \param[in] fun   ������(0xA0~0xAF)
 * \param[in] p_buf �ϱ����ݻ���
 * \param[in] len   �ϱ����ݳ���
 * \retval  0       �ϱ��ɹ�
 * \retval -1       �ϱ�ʧ��
 */
static int __niming_report(uint8_t fun, uint8_t *p_buf, uint8_t len)
{
    uint8_t buf[36];
    uint8_t i;
    
    if (len > 32) return -1;
    
    buf[len+3] = 0; /* У������� */
    buf[0] = 0x88;  /* ֡ͷ */
    buf[1] = fun;   /* ������ */
    buf[2] = len;   /* ���ݳ��� */
    /* װ������ */
    for (i = 0; i < len; i++)
    {
        buf[3+i] = p_buf[i];
    }
    /* ����У��� */
    for (i = 0; i < len+3; i++)
    {
        buf[len+3] += buf[i];
    }
    /* ���ڷ������� */
    uart_send_data_buf(DEBUG_USART, buf, len+4);
    
    return 0;
}

/******************************************************************************/
void niming_report_imu(uint8_t *nrf_rx_buf)
{
    uint8_t buf[28] = {0};
    
    buf[0] = nrf_rx_buf[4];
    buf[1] = nrf_rx_buf[5];
        
    buf[2] = nrf_rx_buf[6];
    buf[3] = nrf_rx_buf[7];
    
    buf[4] = nrf_rx_buf[8];
    buf[5] = nrf_rx_buf[9];
    
    buf[6] = nrf_rx_buf[10];
    buf[7] = nrf_rx_buf[11];
    
    buf[8] = nrf_rx_buf[12];
    buf[9] = nrf_rx_buf[13];
    
    buf[10] = nrf_rx_buf[14];
    buf[11] = nrf_rx_buf[15];
    
    /* buf[12]~buf[17]Ϊ�ų����������� */
    
    buf[18] = nrf_rx_buf[16];
    buf[19] = nrf_rx_buf[17];
    
    buf[20] = nrf_rx_buf[18];
    buf[21] = nrf_rx_buf[19];
    
    buf[22] = nrf_rx_buf[20];
    buf[23] = nrf_rx_buf[21];
    
    /* buf[24]~buf[27]Ӧ���0 */
    
    __niming_report(0XAF, buf, 28);    /* �ɿ���ʾ֡��0XAF */
}

/******************************************************************************/
void niming_report_data(uint8_t *nrf_rx_buf)
{
    uint8_t buf[24] = {0};
    
    for (uint8_t i = 0; i < 24; i++)
    {
        buf[i] = nrf_rx_buf[4+i];
    }
    
    __niming_report(0XA1, buf, 24);     /* �۲첨�Σ�0xA1 */
}
