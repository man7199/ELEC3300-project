#include "bsp_usart.h"
ESP_BufTypeDef ESP_RX_BUF;  
 
 uint8_t usart1_rx_buffer[1000];
 uint8_t usart3_rx_buffer[1000];
 
void ESP_Rxbuf_Init(void)
{
	  int i ;
	  memset(ESP_RX_BUF.buf,0,sizeof(ESP_RX_BUF.buf));   //ʹ��memset����������Ҫ����ͷ�ļ�<string.h>
//	for(i=0;i< ESP_BUF_SIZE; i++)
//	  {
//	      ESP_RX_BUF.buf[i] = 0;
//	  }
      ESP_RX_BUF.fornt = 0;
	  ESP_RX_BUF.length = 0;
	  ESP_RX_BUF.rear = 0;
	
       //?��D��?����?��y?Y
}

 
 
//DebugUartHandle��������������
UART_HandleTypeDef DebugUartHandle;
//WifiUart
UART_HandleTypeDef WifiUartHandle;
 
//������Ӳ���ײ��޹����ݣ��紮��Э�飬���а��������ʣ���żУ�飬ֹͣλ
void DEBUG_USART_Config()
{
    DebugUartHandle.Instance = DEBUG_USART;
    
    //�����ʣ�8λ�ֳ���1ֹͣλ������żУ�飬��Ӳ�����ƣ��շ�ģʽ
    DebugUartHandle.Init.BaudRate = DEBUG_USART_BAUDRATE;
    DebugUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    DebugUartHandle.Init.StopBits = UART_STOPBITS_1;//stm32f1xx_hal_uart.h
    DebugUartHandle.Init.Parity = UART_PARITY_NONE;
    DebugUartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    DebugUartHandle.Init.Mode = UART_MODE_TX_RX;
    
    HAL_UART_Init(&DebugUartHandle);
    
    //ʹ�ܴ��ڽ���
    __HAL_UART_ENABLE_IT(&DebugUartHandle,UART_IT_RXNE);  
}
 
//����WiFi��ͨ�ŵ�uart3
void WIFI_USART_Config()
{
    WifiUartHandle.Instance = WIFI_USART;
 
    WifiUartHandle.Init.BaudRate = DEBUG_USART_BAUDRATE;
    WifiUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    WifiUartHandle.Init.StopBits = UART_STOPBITS_1;//stm32f1xx_hal_uart.h
    WifiUartHandle.Init.Parity = UART_PARITY_NONE;
    WifiUartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    WifiUartHandle.Init.Mode = UART_MODE_TX_RX;
 
    HAL_UART_Init(&WifiUartHandle);
 
    //ʹ�ܴ��ڽ���
    __HAL_UART_ENABLE_IT(&WifiUartHandle,UART_IT_RXNE);
}
 
// mcu �ײ�Ӳ����ص����������š�ʱ�ӡ�DMA���ж�
//ʵ�ʱ�HAL_UART_Init��stm32f1xx_hal_uart.c���ú�������
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        GPIO_InitTypeDef GPIO_Init1;
        
        //����1ʱ�� GPIOʱ��
        DEBUG_USART_CLK_ENABLE();
        DEBUG_USART_RX_GPIO_CLK_ENABLE();
        DEBUG_USART_TX_GPIO_CLK_ENABLE();
        
        //�������Ÿ��ù��� TX 
        GPIO_Init1.Pin = DEBUG_USART_TX_PIN;
        GPIO_Init1.Mode = GPIO_MODE_AF_PP;
        GPIO_Init1.Pull = GPIO_PULLUP;
        GPIO_Init1.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_Init1);
        
        //����Rx
        GPIO_Init1.Pin = DEBUG_USART_RX_PIN;
        GPIO_Init1.Mode = GPIO_MODE_AF_INPUT;//��������ģʽ
        HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_Init1); 
        
        HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//��ռ���ȼ�0�������ȼ�1
        HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //ʹ��USART1�ж�ͨ�� 
    }
    
    if(huart->Instance == USART3)
    {
        //����3������
        GPIO_InitTypeDef GPIO_Init3;
        
        WIFI_USART_CLK_ENABLE();
        WIFI_USART_RX_GPIO_CLK_ENABLE();
        WIFI_USART_TX_GPIO_CLK_ENABLE();
        //����Tx���Ÿ��ù���
        GPIO_Init3.Pin = WIFI_USART_TX_GPIO_PIN;
        GPIO_Init3.Mode = GPIO_MODE_AF_PP;
        GPIO_Init3.Pull = GPIO_PULLUP;
        GPIO_Init3.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(WIFI_USART_TX_GPIO_PORT, &GPIO_Init3);
        
        //����Rx
        GPIO_Init3.Pin = WIFI_USART_RX_GPIO_PIN;
        GPIO_Init3.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(WIFI_USART_RX_GPIO_PORT, &GPIO_Init3); 
        
        HAL_NVIC_SetPriority(WIFI_UASRT_IRQ ,0,1);	//��ռ���ȼ�0�������ȼ�1
        HAL_NVIC_EnableIRQ(WIFI_UASRT_IRQ );		    //ʹ��USART3�ж�ͨ�� 
    }
}


 
 
 
/*****************  �����ַ��� **********************/
/*  HAL_UART_Transmit ����������һ�������ķ��ͺ����������ظ��жϴ����Ƿ�����ɣ�
    ����ÿ���ַ���ֱ������
    ���ַ���ֹͣ���͡�*/
void Usart_SendString(UART_HandleTypeDef *usarthandle,uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(usarthandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
//�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);
	
	return (ch);
}
 
//�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{		
	int ch;
	HAL_UART_Receive(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);
	return (ch);
}