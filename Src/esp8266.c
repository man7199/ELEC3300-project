#include "esp8266.h"
#include "bsp_usart.h"
 #include "stdlib.h"
 
 int TEMPERATURE;
 int HUMIDITY;
 int TIMES;
//CH-PD 和RST 引脚配置
void ESP8266_GPIO_Config()
{
    //GPIO结构体
    GPIO_InitTypeDef  GPIO_InitStruct;
    //开启外设时钟
    ESP8266_CH_PD_CLK_ENABLE();
    ESP8266_RST_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = ESP8266_CH_PD_PIN;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ESP8266_CH_PD_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ESP8266_RST_PIN;
    HAL_GPIO_Init(ESP8266_RST_PORT,&GPIO_InitStruct);
}
 
//esp8266初始化 main调用
void ESP8266_Init(void)
{
    ESP8266_GPIO_Config();
    WIFI_USART_Config();
 
    //CHPD RST 初始化为高电平
    ESP8266_CH_PD_ENABLE();
    ESP8266_RST_ENABLE();
}
 
//发送AT指令
void ESP8266_Cmd(uint8_t *cmd)
{
    uint8_t data[2] = {'\r','\n'};
    
    Usart_SendString(&WifiUartHandle,cmd);    
    Usart_SendString(&WifiUartHandle,data);
}
 
//测试AT指令 main调用 测试成功
void ESP8266_AT_Test(void)
{    
    ESP8266_Cmd("AT");
    HAL_Delay(2000);
    //ESP8266_Cmd("AT+CWLAP");
    //HAL_Delay(2000);
    //ESP8266_JoinAP("HonorNote10","zdw123456");
    //HAL_Delay(2000);
 
}
 
//查找附近wifi
void ESP8266_Find_Wifi(void)
{
    ESP8266_Cmd("AT+CWLAP");
}
 
//ESP8266工作模式选择
void ESP8266_Net_Mode(Net_ModeTypeDef enum_mode)
{
    switch(enum_mode)
    {
        case AP:
            ESP8266_Cmd("AT+CWMODE=2");
        case STA:
            ESP8266_Cmd("AT+CWMODE=1");
				break;
        case STA_AP:
            ESP8266_Cmd("AT+CWMODE=3");
        default :
            break;
    }
}
 
//连接外部wifi热点
void ESP8266_JoinAP(char *name,char * password)
{
    char cCmd [120];
	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", name, password );
    ESP8266_Cmd((uint8_t*)cCmd);
}
 
//ESP8266模块创建WiFi热点：wifi名 wifi密码 wifi加密方式
void ESP8266_BuildAP(char* name,char* password,ENUM_AP_PsdMode_TypeDef enunPsdMode)
{
    char cCmd [120];
	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", name, password, enunPsdMode );	
	ESP8266_Cmd ( (uint8_t*)cCmd );
}
 
 
//ESP8266模块启动多连接
void ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );	
	ESP8266_Cmd ( (uint8_t*)cStr);	
}
 
//Wifi模块连接外部服务器：网络协议 IP 端口 模块连接服务器ID
void ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * port, ENUM_ID_NO_TypeDef id)
{
    char cStr [100] = { 0 }, cCmd [120];
    switch (  enumE )
    {
        case enumTCP:
            sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, port );
            break;        
        case enumUDP:
            sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, port );
            break;        
        default:
            break;
    }
    if ( id < 5 )
        sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);
    else
        sprintf ( cCmd, "AT+CIPSTART=%s", cStr );
 
	ESP8266_Cmd ( (uint8_t*)cCmd );
    HAL_Delay(3000);	
}
 
 
//ESP8266模块开启或关闭服务器模式:enumMode(开启/关闭)  pPortNum，服务器端口号字符串 pTimeOver服务器超时时间字符串，单位：秒
void ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];
	if ( enumMode )	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );
 
		ESP8266_Cmd ( (uint8_t*)cCmd1 );
		ESP8266_Cmd ( (uint8_t*)cCmd2 );
	}	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );
		ESP8266_Cmd ( (uint8_t*)cCmd1 );
	}	
}
 
//获取 WF-ESP8266 的连接状态，较适合单端口时使用
uint8_t ESP8266_Get_LinkStatus ( void )
{
	ESP8266_Cmd ( "AT+CIPSTATUS" );
   
    //这里要解决：读取ESP返回的数据
    //当前处理的方法是 将ESP的数据直接转uart1 打印到上位机
    /*if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
        return 2; //获得ip
    
    else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
        return 3;//建立连接
    
    else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
        return 4;//失去连接	
    */	
	return 0;//0，获取状态失败
	
}
 
//获取 F-ESP8266 的 AP IP :pApIp，存放 AP IP 的数组的首地址 ucArrayLength，存放 AP IP 的数组的长度
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;	
	char * pCh;	
    ESP8266_Cmd ( "AT+CIFSR" );
	
    //获取ESP8266结果 放内存中
    /*
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	*/
	return 1;	
}		
 
//ESP8266模块进入透传发送
void ESP8266_UnvarnishSend ( void )
{
	ESP8266_Cmd ( "AT+CIPMODE=1" );
    HAL_Delay(2000);
	ESP8266_Cmd ( "AT+CIPSEND");
	HAL_Delay(2000);
 
}
 
//ESP8266模块退出透传模式
void ESP8266_ExitUnvarnishSend ( void )
{
	HAL_Delay(500);
	
	Usart_SendString (&WifiUartHandle ,"+++" );
	
	HAL_Delay ( 500 ); 
	
}
 
/*
 * 函数名：ESP8266_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
 */
/*
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;	
	if ( enumEnUnvarnishTx )
	{
		macESP8266_Usart ( "%s", pStr );	
		bRet = true;	
	}
	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );
		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );
		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	return bRet;
}*/
/*
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
 */
/*
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	}
	return pRecStr;
	
}*/
 

void ESP8266_PassThrough_Test(void)
{
    ESP8266_AT_Test();

    ESP8266_Net_Mode(STA);
    HAL_Delay(3000);
    

    ESP8266_JoinAP(ESP8266_JOIN_AP_NAME,ESP8266_JOIN_AP_PASSWORD);
    HAL_Delay(3000);
    

    ESP8266_Enable_MultipleId ( DISABLE );
    HAL_Delay(3000);

    ESP8266_Link_Server(enumTCP,ESP8266_CONNECT_TCPSERVER_IP,ESP8266_CONNECT_TCPSERVER_PORT,Single_ID_0);
    HAL_Delay(5000);

    ESP8266_UnvarnishSend();    
	HAL_Delay(2000);
    printf("set ESP8266 OK,now go into PassThrough Mode!\n");
	ESP_Rxbuf_Init();
		ESP8266_Cmd("GET http://quan.suning.com/getSysTime.do HTTP/1.1\r\nHost: quan.suning.com\r\n\r\n");
		HAL_Delay(4000);
				GET_Net_Time();
}

void ESP8266_Disconnect (void){
	ESP8266_Cmd ( "AT+CIPCLOSE" );
}

void Get_Weather(void){
	 ESP8266_ExitUnvarnishSend () ;
	HAL_Delay(2000);
	ESP8266_Disconnect();
	HAL_Delay(2000);
ESP8266_Link_Server(enumTCP,ESP8266_CONNECT_WEATHER_IP,ESP8266_CONNECT_WEATHER_PORT,Single_ID_0);
	HAL_Delay(5000);
 ESP8266_UnvarnishSend();    
	ESP_Rxbuf_Init();
	ESP8266_Cmd("GET https://api.openweathermap.org/data/2.5/weather?lat=22.3027&lon=114.1772&appid=9b096bf058b84cc5a20620fb6bbe5be5&units=metric HTTP/1.1\r\nHost: api.openweathermap.org\r\n\r\n" );
	HAL_Delay(4000);
	GET_Temp();
}
void GET_Temp(void){
	
	  char *temp_pt = NULL;  
	  char *humidity_pt = NULL;
	  int i = 0;
	  char humidity[2] = {0};   
	  char temperature[2] = {0};
	 
	
			    
		
		temp_pt  = strstr((const char *) ESP_RX_BUF.buf,(const char *)"temp") + 6;   
		humidity_pt  = strstr((const char *) ESP_RX_BUF.buf,(const char *)"humidity") + 10;  
		
		for(i = 0; i < 2; i++)  
		{
		    temperature[i] = *temp_pt;
			  temp_pt ++;
		}
		TEMPERATURE = atoi(&temperature[0]); 
		
		for(i = 0; i < 2; i++)   
		{
		    humidity[i] = *humidity_pt;
			  humidity_pt ++;
		}
		HUMIDITY = atoi(&humidity[0]); 
		
 printf ( "\r\nnetnet\r\n" );
	  
		 

}
void GET_Net_Time(void)
{
	   char *data_pt = NULL;
	
	  
	   char *hour_string;
	   char *minute_string;
	   char *second_string;

			 
			 data_pt = strstr((const char *) ESP_RX_BUF.buf,(const char *)"sysTime1");
			 			 
			if(data_pt != NULL)
			{
		    	
	            hour_string = data_pt + HOURS_ADD_DRES; 
	            minute_string = data_pt + MINUTES_ADD_DRES;  
	            second_string = data_pt + SECONDS_ADD_DRES;  			 
		 
			    TIMES = Get_Times(hour_string, minute_string, second_string);
			}
			else
			{
			     printf("get net time failed!\r\n");
			}
}
int Get_Times(char *h, char *m, char *s)
{
    int time_return;
	  int hour_return;
    int min_return;
    int sec_return;	
	
	  hour_return = atoi(h)/10000; 
	  min_return = atoi(m)/100;  
	  sec_return = atoi(s);  
	
	  time_return = hour_return*3600 + min_return*60 + sec_return; 
	
	  return time_return;
}

 
 