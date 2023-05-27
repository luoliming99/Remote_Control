#include "bsp_spi.h"

/**
 * \brief ��������
 * CSN  - PB12 
 * SCK  - PB13
 * MISO - PB14
 * MOSI - PB15
 */
static void __spi_gpio_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0}; 

	SPI_GPIO_CLK_CMD(SPI_GPIO_CLK, ENABLE);     /* ʹ��GPIOʱ�� */
	
    /* GPIO���� */
    GPIO_InitStructure.GPIO_Pin = SPI_CSN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CSN_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
    GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
    
    SPI_CSN_HIGH;
}

static void __spi_mode_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure = {0};
    
    SPI_CLK_CMD(SPI_CLK, ENABLE);   /* ʹ��SPIʱ�� */

    /* 
     * SPIģʽ����
     */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  /* ȫ˫�� */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       /* ����ģʽ */
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                   /* 8λ����֡��ʽ */
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                          /* ʱ�Ӽ��ԣ�����״̬ʱ��SCK����Ϊ�͵�ƽ */
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                        /* ʱ����λ�����ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ */
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                           /* ����������豸����CSN�����ϵĵ�ƽ��SSIλ��ֵ���� */
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  /* ������ = Fpclk1 / 16�������� = ������ / 2 */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  /* �ȷ���MSB */
    SPI_InitStructure.SPI_CRCPolynomial = 7;                            /* ָ��CRC����ʱ�õ��Ķ���ʽ */
    
    SPI_Init(SPI_x , &SPI_InitStructure);

    SPI_Cmd(SPI_x, ENABLE);
}

/******************************************************************************/
void spi_init(void)
{
    __spi_gpio_config();
    __spi_mode_config();
}

/******************************************************************************/
int spi_swap_byte(uint8_t *byte_out, uint8_t byte_in)
{
    uint32_t timeout_cnt = 0;
    
    /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    timeout_cnt = 0;
    while(SPI_I2S_GetFlagStatus(SPI_x, SPI_I2S_FLAG_TXE) == RESET)
    {
        if (timeout_cnt++ > SPI_TIMEOUT_CNT)
        {
            return -1;
        }
    }
    
    /* ��Ҫд�������д�뷢�ͻ����� */
    SPI_I2S_SendData(SPI_x, byte_in);
    
    /* �ȴ����ջ������ǿգ�RXNE�¼� */
    timeout_cnt = 0;
    while(SPI_I2S_GetFlagStatus(SPI_x, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if (timeout_cnt++ > SPI_TIMEOUT_CNT)
        {
            return -1;
        }
    }
    
    /* ��ȡ���ջ��������� */
    *byte_out = (uint8_t)SPI_I2S_ReceiveData(SPI_x);
    return 0;
}

/******************************************************************************/
int spi_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t dumy;
    
    SPI_CSN_LOW;
    if (0 != spi_swap_byte(&dumy, reg))     /* д�Ĵ�����ַ */
    {
        return -1;
    }
    if (0 != spi_swap_byte(&dumy, value))   /* д�Ĵ���ֵ */
    {
        return -1;
    }
    SPI_CSN_HIGH;
    return 0;
}

/******************************************************************************/
int spi_read_reg(uint8_t reg, uint8_t *value)
{
    uint8_t dumy;
    
    SPI_CSN_LOW;
    if (0 != spi_swap_byte(&dumy, reg))     /* д�Ĵ�����ַ */
    {
        return -1;
    }
    if (0 != spi_swap_byte(value, 0xFF))    /* ���Ĵ���ֵ */
    {
        return -1;
    }
    SPI_CSN_HIGH;
    return 0;
}

/******************************************************************************/
int spi_write(uint8_t reg, uint32_t n_bytes, uint8_t *p_data)
{
    uint8_t dumy;
    
    SPI_CSN_LOW;
    if (0 != spi_swap_byte(&dumy, reg))         /* д�Ĵ�����ַ */
    {
        return -1;
    }
    while (n_bytes)
    {
        if (0 != spi_swap_byte(&dumy, *p_data)) /* д�Ĵ���ֵ */
        {
            return -1;
        }
        p_data++;
        n_bytes--;
    }  
    SPI_CSN_HIGH;
    return 0;
}

/******************************************************************************/
int spi_read(uint8_t reg, uint32_t n_bytes, uint8_t *p_data)
{
    uint8_t dumy;
    
    SPI_CSN_LOW;
    if (0 != spi_swap_byte(&dumy, reg))         /* д�Ĵ�����ַ */
    {
        return -1;
    }
    while (n_bytes)
    {
        if (0 != spi_swap_byte(p_data, 0xFF))   /* ���Ĵ���ֵ */
        {
            return -1;
        }
        
        p_data++;
        n_bytes--;
    }  
    SPI_CSN_HIGH;
    return 0;
}
