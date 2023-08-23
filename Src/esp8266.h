#ifndef ESP8266_H
#define ESP8266_H
 
#include "stm32f1xx.h"
#include <stdio.h>
 
 
/*******************************************STAģʽ��*******************************/
//ѡ��Ҫ���ӵ��ȵ����ƺ�����
#define ESP8266_JOIN_AP_NAME             "TP-Link_4FCB"
#define ESP8266_JOIN_AP_PASSWORD         "24412865"
//ѡ��Ҫ���ӵķ�������IP �˿�
#define ESP8266_CONNECT_TCPSERVER_IP     "quan.suning.com"
#define ESP8266_CONNECT_TCPSERVER_PORT   "80"
#define ESP8266_CONNECT_WEATHER_IP     "api.openweathermap.org"
#define ESP8266_CONNECT_WEATHER_PORT   "80"
#define ESP_TIME_INFO  "GET http://quan.suning.com/getSysTime.do HTTP/1.1\r\nHost: quan.suning.com\r\n\r\n" 
/*******************************************STAģʽ��*******************************/
 
 
/*******************************************APģʽ��*******************************/
//����wifi�ȵ㣺���� ���� ���ܷ�ʽ
#define ESP8266_BUILD_AP_NAME             "ZDW_STM32F103"
#define ESP8266_BUILD_AP_PASSWORD         "zdw123456"
#define ESP8266_BUILD_AP_FUNCTION         OPEN 
//wifi�ȵ��IP �˿�
#define ESP8266_TcpServer_IP              "192.168.1.1"     
#define ESP8266_TcpServer_Port            "8080"               
#define ESP8266_TcpServer_OverTime        "1800"          //��          
/*******************************************APģʽ��*******************************/
 
/* 
ESP8266 IO  <------>    STM32 IO
URXD                    PB10��tx��
UTXD                    PB11 (rx)
CH-PD                   PB8
RST                     PB9
*/
/******* ESP8266 pins *************/
//8266 CH_PD����  
#define ESP8266_CH_PD_PORT                         GPIOB
#define ESP8266_CH_PD_PIN                          GPIO_PIN_8
#define ESP8266_CH_PD_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
 #define HOURS_ADD_DRES 19
#define MINUTES_ADD_DRES 21
#define SECONDS_ADD_DRES 23
//8266 RST����
#define ESP8266_RST_PORT                           GPIOB
#define ESP8266_RST_PIN                            GPIO_PIN_9
#define ESP8266_RST_CLK_ENABLE()                    __HAL_RCC_GPIOB_CLK_ENABLE()
 
#define ESP8266_CH_PD_ENABLE()                     HAL_GPIO_WritePin(ESP8266_CH_PD_PORT,ESP8266_CH_PD_PIN,GPIO_PIN_SET)
#define ESP8266_RST_ENABLE()                       HAL_GPIO_WritePin(ESP8266_RST_PORT,ESP8266_RST_PIN,GPIO_PIN_SET)
 
extern UART_HandleTypeDef WifiUartHandle;
 extern int HUMIDITY;
extern int TEMPERATURE;
extern int TIMES;
//����ģʽ
typedef enum
{
    AP,
    STA,
    STA_AP
}Net_ModeTypeDef;
 
//wifi�����ȵ� ���ܷ�ʽ
typedef enum
{
    OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
}ENUM_AP_PsdMode_TypeDef;
 
//����Э��
typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
 
//ģ�����ӷ�����ID
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
 
 
//ESP8266GPIO��ʼ��
void ESP8266_GPIO_Config(void);
 
//ESP8266ģ���ʼ��
void ESP8266_Init(void);
 
//����ATָ�ESP8266
void ESP8266_Cmd(uint8_t *cmd);
 
//���Ҹ���wifi
void ESP8266_Find_Wifi(void);
 
//����ATָ��
void ESP8266_AT_Test(void);
 
//����͸��ģʽ
void ESP8266_PassThrough_Test(void);
 
//ESP8266����ģʽѡ��
void ESP8266_Net_Mode(Net_ModeTypeDef enum_mode);
 
//�����ⲿwifi�ȵ�
void ESP8266_JoinAP(char *name,char * password);
 
//ESP8266ģ�鴴��WiFi�ȵ㣺wifi�� wifi���� wifi���ܷ�ʽ
void ESP8266_BuildAP(char *name,char *password,ENUM_AP_PsdMode_TypeDef enunPsdMode);
 
//ESP8266ģ������������
void ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
 
//Wifiģ�������ⲿ������������Э�� IP �˿� ģ�����ӷ�����ID
void ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * port, ENUM_ID_NO_TypeDef id);
 
//ESP8266ģ�鿪����رշ�����ģʽ:enumMode(����/�ر�)  pPortNum���������˿ں��ַ��� pTimeOver��������ʱʱ���ַ�������λ����
void ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
 
//��ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
uint8_t ESP8266_Get_LinkStatus ( void );
 void ESP8266_Disconnect (void);
 void GET_Temp(void);
//��ȡ F-ESP8266 �� AP IP :pApIp����� AP IP ��������׵�ַ ucArrayLength����� AP IP ������ĳ���
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength );
 void Get_Weather(void);
//ESP8266ģ�����͸������
void ESP8266_UnvarnishSend ( void );
 int Get_Times(char *h, char *m, char *s);
 void GET_Net_Time(void);
 
#endif