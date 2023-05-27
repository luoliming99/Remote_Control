#include "bsp_uart.h"

/* Support Printf Function Definition */
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;

/* �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf���� */
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ����� */
    USART_SendData(DEBUG_USART, (uint8_t) ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		

    return (ch);
}

/* �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ��� */
int fgetc(FILE *f)
{
    /* �ȴ������������� */
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(DEBUG_USART);
}

static void __nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����USARTΪ�ж�Դ */
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
    /* �������ȼ�*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    /* �����ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    /* ʹ���ж� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* ��ʼ������NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void uart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};

	/* �򿪴���GPIO��ʱ�� */
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	/* �򿪴��������ʱ�� */
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	/* ��USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    /* ��USART Rx��GPIO����Ϊ��������ģʽ */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* ���ô��ڵĹ������� */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(DEBUG_USART, &USART_InitStructure);
	
	/* �����ж����ȼ����� */
	__nvic_config();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);	
	
	/* ʹ�ܴ��� */
	USART_Cmd(DEBUG_USART, ENABLE);		
}

/******************************************************************************/
void uart_send_one_byte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/******************************************************************************/
void uart_send_string(USART_TypeDef *pUSARTx, char *str)
{
	uint32_t i = 0;
    
    while(*(str + i) != '\0') 
    {
        uart_send_one_byte(pUSARTx, *(str + i));
        i++;
    }
}

/******************************************************************************/
void uart_send_data_buf(USART_TypeDef *pUSARTx, uint8_t *pBuf, uint32_t len)
{
    uint32_t i = 0;
    
	for (i = 0; i < len; i++)
    {
        uart_send_one_byte(pUSARTx, *(pBuf + i));
    }
}


