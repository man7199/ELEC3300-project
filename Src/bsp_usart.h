 
#ifndef BSP_USART_H
#define BSP_USART_H
 
#include "stm32f1xx.h"
#include <stdio.h>
 #include "string.h"
//串口波特率
#define DEBUG_USART_BAUDRATE                    115200
#define WIFI_USART_BAUDRATE                     115200
#define ESP_BUF_SIZE  					 1024  
 
/*******************************************************/ /*********************************************************/
//引脚定义 串口1
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE()
 
#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10
 
#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9
 
#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		USART1_IRQn
  //宏定义缓冲队列最大存储上限
   //ESP?o3???×?′óèYá?

//循环对列结构体
typedef struct 
{
    unsigned char buf[ESP_BUF_SIZE];
	unsigned short int length;
    unsigned short int fornt;               //ESP?óáDí·????
    unsigned short int rear;               //ESP?óáD?2????
}ESP_BufTypeDef;                     //?¨ò?ESP?-?·?óáD?o3????á11ì?

//声明一个队列缓冲区，作为全局变量
    //Wifi′??ú?óê??o3???
//在头文件中声明外部变量，这样可以在其他C文件中调用
extern ESP_BufTypeDef ESP_RX_BUF;      //Wifi′??ú?óê??o3???
//接收缓冲区初始化ESP?óê??o3???3?ê??ˉ


 
void Usart_SendString(UART_HandleTypeDef *usarthandle ,uint8_t *str);
void DEBUG_USART_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
  extern uint8_t usart1_rx_buffer[1000];
extern uint8_t usart3_rx_buffer[1000];
extern UART_HandleTypeDef DebugUartHandle;
extern UART_HandleTypeDef WifiUartHandle;
/*********************************************************/ /*********************************************************/
 
//引脚定义：串口3
#define WIFI_USART                              USART3
#define WIFI_USART_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
 
#define WIFI_USART_RX_GPIO_PORT                 GPIOB
#define WIFI_USART_RX_GPIO_PIN                  GPIO_PIN_11 //PB11(UART RXD) <--> ESP8266 UTXD
#define WIFI_USART_RX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
 
#define WIFI_USART_TX_GPIO_PORT                 GPIOB
#define WIFI_USART_TX_GPIO_PIN                  GPIO_PIN_10 //PB10(UART TXD) <--> ESP8266 URXD
#define WIFI_USART_TX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
 
#define WIFI_USART_IRQHandle                    USART3_IRQHandler
#define WIFI_UASRT_IRQ                          USART3_IRQn
 
void WIFI_USART_Config(void);
 
#endif