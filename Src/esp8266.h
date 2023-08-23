#ifndef ESP8266_H
#define ESP8266_H
 
#include "stm32f1xx.h"
#include <stdio.h>
 
 
/*******************************************STA模式下*******************************/
//选择要连接的热点名称和密码
#define ESP8266_JOIN_AP_NAME             "TP-Link_4FCB"
#define ESP8266_JOIN_AP_PASSWORD         "24412865"
//选择要连接的服务器和IP 端口
#define ESP8266_CONNECT_TCPSERVER_IP     "quan.suning.com"
#define ESP8266_CONNECT_TCPSERVER_PORT   "80"
#define ESP8266_CONNECT_WEATHER_IP     "api.openweathermap.org"
#define ESP8266_CONNECT_WEATHER_PORT   "80"
#define ESP_TIME_INFO  "GET http://quan.suning.com/getSysTime.do HTTP/1.1\r\nHost: quan.suning.com\r\n\r\n" 
/*******************************************STA模式下*******************************/
 
 
/*******************************************AP模式下*******************************/
//创建wifi热点：名称 密码 加密方式
#define ESP8266_BUILD_AP_NAME             "ZDW_STM32F103"
#define ESP8266_BUILD_AP_PASSWORD         "zdw123456"
#define ESP8266_BUILD_AP_FUNCTION         OPEN 
//wifi热点的IP 端口
#define ESP8266_TcpServer_IP              "192.168.1.1"     
#define ESP8266_TcpServer_Port            "8080"               
#define ESP8266_TcpServer_OverTime        "1800"          //秒          
/*******************************************AP模式下*******************************/
 
/* 
ESP8266 IO  <------>    STM32 IO
URXD                    PB10（tx）
UTXD                    PB11 (rx)
CH-PD                   PB8
RST                     PB9
*/
/******* ESP8266 pins *************/
//8266 CH_PD引脚  
#define ESP8266_CH_PD_PORT                         GPIOB
#define ESP8266_CH_PD_PIN                          GPIO_PIN_8
#define ESP8266_CH_PD_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
 #define HOURS_ADD_DRES 19
#define MINUTES_ADD_DRES 21
#define SECONDS_ADD_DRES 23
//8266 RST引脚
#define ESP8266_RST_PORT                           GPIOB
#define ESP8266_RST_PIN                            GPIO_PIN_9
#define ESP8266_RST_CLK_ENABLE()                    __HAL_RCC_GPIOB_CLK_ENABLE()
 
#define ESP8266_CH_PD_ENABLE()                     HAL_GPIO_WritePin(ESP8266_CH_PD_PORT,ESP8266_CH_PD_PIN,GPIO_PIN_SET)
#define ESP8266_RST_ENABLE()                       HAL_GPIO_WritePin(ESP8266_RST_PORT,ESP8266_RST_PIN,GPIO_PIN_SET)
 
extern UART_HandleTypeDef WifiUartHandle;
 extern int HUMIDITY;
extern int TEMPERATURE;
extern int TIMES;
//工作模式
typedef enum
{
    AP,
    STA,
    STA_AP
}Net_ModeTypeDef;
 
//wifi创建热点 加密方式
typedef enum
{
    OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
}ENUM_AP_PsdMode_TypeDef;
 
//网络协议
typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
 
//模块连接服务器ID
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
 
 
//ESP8266GPIO初始化
void ESP8266_GPIO_Config(void);
 
//ESP8266模块初始化
void ESP8266_Init(void);
 
//发送AT指令到ESP8266
void ESP8266_Cmd(uint8_t *cmd);
 
//查找附近wifi
void ESP8266_Find_Wifi(void);
 
//测试AT指令
void ESP8266_AT_Test(void);
 
//测试透传模式
void ESP8266_PassThrough_Test(void);
 
//ESP8266工作模式选择
void ESP8266_Net_Mode(Net_ModeTypeDef enum_mode);
 
//连接外部wifi热点
void ESP8266_JoinAP(char *name,char * password);
 
//ESP8266模块创建WiFi热点：wifi名 wifi密码 wifi加密方式
void ESP8266_BuildAP(char *name,char *password,ENUM_AP_PsdMode_TypeDef enunPsdMode);
 
//ESP8266模块启动多连接
void ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
 
//Wifi模块连接外部服务器：网络协议 IP 端口 模块连接服务器ID
void ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * port, ENUM_ID_NO_TypeDef id);
 
//ESP8266模块开启或关闭服务器模式:enumMode(开启/关闭)  pPortNum，服务器端口号字符串 pTimeOver服务器超时时间字符串，单位：秒
void ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
 
//获取 WF-ESP8266 的连接状态，较适合单端口时使用
uint8_t ESP8266_Get_LinkStatus ( void );
 void ESP8266_Disconnect (void);
 void GET_Temp(void);
//获取 F-ESP8266 的 AP IP :pApIp，存放 AP IP 的数组的首地址 ucArrayLength，存放 AP IP 的数组的长度
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength );
 void Get_Weather(void);
//ESP8266模块进入透传发送
void ESP8266_UnvarnishSend ( void );
 int Get_Times(char *h, char *m, char *s);
 void GET_Net_Time(void);
 
#endif