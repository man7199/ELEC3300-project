/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcdtp.h"
#include "xpt2046.h"
#include "stdlib.h"
#include "math.h"
#include "stm32f1xx_it.h"
#include "esp8266.h"
#include "bsp_usart.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define PI 3.141592654
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t cnt_tim3_10ms;
uint32_t start_cnt_tim3_10ms;
uint32_t css,ss,mm;
uint32_t cnt_tim4_10ms;
int countercounter=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
if (htim->Instance ==TIM3){
		cnt_tim4_10ms++;
	if(countercounter==1){
		cnt_tim3_10ms++;
	
	}
	}



}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	cnt_tim3_10ms=0;
		cnt_tim4_10ms=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
                                         
                                                  
   
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	ESP8266_Init();
	 macXPT2046_CS_DISABLE();
	
	LCD_INIT();

	LCD_DrawString(70, 150, "Initializing...");			
	ESP8266_PassThrough_Test();
		cnt_tim4_10ms=500;
		HAL_TIM_Base_Start_IT(&htim3);
  Get_Weather();
ConvertedValue=0;
ConvertedValue2=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 


	while( ! XPT2046_Touch_Calibrate () );   
	
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	int mod=0;
	int mod2=0;
	
  while (1)
  {	
		if (mod==0){
			LCD_INIT();
			LCD_DrawString(70, 10, "Smart Watch");			
			
			LCD_Clear ( 10,  40,  100, 100, BLUE	);
			LCD_Clear ( 120,  40,  100, 100, RED	);
			LCD_Clear ( 10,  160,  100, 100, GREEN	);
			LCD_Clear ( 120,  160,  100, 100, YELLOW	);
			LCD_DrawString_Color(15, 60, "temperature",BLUE,BLACK);
			LCD_DrawString_Color(30, 100, "humidity",BLUE,BLACK);
			LCD_DrawString_Color(150, 60, "timer",RED,BLACK);
			LCD_DrawString_Color(140, 100, "counter",RED,BLACK);
			LCD_DrawString_Color(30, 200, "drawing",GREEN,BLACK);
			LCD_DrawString_Color(140, 200, "compass",YELLOW,BLACK);
			while(mod==0){
				mod=Main_touchkey();
				if(ConvertedValue2==1){
					mod=10;
				}
			}}
    else if ( mod == 1 )	         
    {
			int x=0;
			LCD_INIT();
			LCD_Clear ( 0,  0,  240, 320, GREEN	);
			LCD_DrawString_Color(20, 130, "current HK temperature:",GREEN,BLACK);
			LCD_DrawString_Color(20, 180, "current HK humidity:",GREEN,BLACK);
			char output[16];
			sprintf(output, "%02d", TEMPERATURE);
	LCD_DrawString_Color(120,150,output,GREEN,BLACK);
				sprintf(output, "%02d", HUMIDITY);
			LCD_DrawString_Color(120,200,output,GREEN,BLACK);
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
			while(x==0){
				x=Sub_touchkey();
				if(ConvertedValue2==1){
					x=1;
				}
			}
			mod=0;
    }					
		
		else if ( mod == 2 )	         
    {
			int x=0;
			int count=0;
			char output[16];
			int digit1=0;
				int digit2=0;
				int digit3=0;
			if(mod2==0){
			LCD_INIT();
			LCD_Clear ( 0, 0 ,  240, 320, GREY	);
			LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
			LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Pause",GREY,BLACK);
				LCD_Clear ( 150, 270 ,  60, 50, GREEN	);
				LCD_DrawString_Color(160,290,"Reset",GREEN,BLACK);
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
			LCD_Clear ( 30, 0 ,  100, 50, CYAN	);
					LCD_DrawString_Color(45,15,"Counter",CYAN,BLACK);
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
					cnt_tim3_10ms=0;
				digit1=0;
				digit2=0;
				digit3=0;
				LCD_DrawString(105,150,":");
				LCD_DrawString(145,150,":");
			while(x<4){
				x=Timer_touchkey();
				if(x==1){				
						countercounter=1;
					LCD_Clear ( 10, 270 ,  60, 50, GREY	);
			LCD_Clear ( 80, 270 ,  60, 50, RED	);
			LCD_DrawString_Color(20,290,"Start",GREY,BLACK);
			LCD_DrawString_Color(90,290,"Stop",RED,BLACK);
				}
				if(x==2){
				countercounter=0;
				LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
			LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Stop",GREY,BLACK);
				}
				if(x==3){
					cnt_tim3_10ms=0;
					countercounter=0;
				LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
			LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Stop",GREY,BLACK);
				
				}
					digit3=cnt_tim3_10ms%100;
					sprintf(output, "%02d", digit3);
				LCD_DrawString(160,150,output);
				digit2=cnt_tim3_10ms/100;
				digit1=digit2/60;
				digit2=digit2%60;
				sprintf(output, "%02d", digit2);
		LCD_DrawString(120,150,output);
				sprintf(output, "%02d", digit1);
		LCD_DrawString(80,150,output);
				if(ConvertedValue2==1){
					x=5;
				}
			}
			if(x==5){
			mod=0;
			}
			else {mod2=1;}
    }	
			else {
				
			LCD_INIT();
				LCD_Clear ( 0, 0 ,  240, 320, GREY	);
				int setted=0;
				int x1=0;
				int x2=0;
				uint32_t x3=0;
					LCD_Clear ( 60, 100 ,  120, 100,BLUE);
			LCD_DrawString_Color(90,140,"Set Time",BLUE,BLACK);
			LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
					LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Pause",GREY,BLACK);
				LCD_Clear ( 150, 270 ,  60, 50, GREEN	);
				LCD_DrawString_Color(160,290,"Reset",GREEN,BLACK);
			
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
			LCD_Clear ( 30, 0 ,  100, 50, CYAN	);
					LCD_DrawString_Color(55,15,"Timer",CYAN,BLACK);
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
					cnt_tim3_10ms=0;
				digit1=0;
				digit2=0;
				digit3=0;
				while(x<5){
				x=Counter_touchkey();
				if(x==4){
				LCD_INIT();
					int y=0;
					LCD_Clear ( 0, 40 ,  70, 60, BLACK	);
			LCD_Clear ( 80, 40 ,  70, 60, BLACK	);
					LCD_Clear ( 160, 40 ,  70, 60, BLACK	);
		LCD_Clear ( 0, 110 ,  70, 60, BLACK	);
			LCD_Clear ( 80, 110 ,  70, 60, BLACK	);
					LCD_Clear ( 160, 110 ,  70, 60, BLACK	);
					LCD_Clear ( 0, 180 ,  70, 60, BLACK	);
			LCD_Clear ( 80, 180 ,  70, 60, BLACK	);
					LCD_Clear ( 160, 180 ,  70, 60, BLACK	);
				LCD_Clear ( 0,250  ,  70, 60, BLACK	);
					LCD_Clear ( 80,250  ,  70, 60, BLACK	);
					LCD_Clear ( 160,250  ,  70, 60, BLACK	);
					LCD_DrawChar(30,70,'1');
					LCD_DrawChar(110,70,'2');
					LCD_DrawChar(190,70,'3');
					LCD_DrawChar(30,140,'4');
					LCD_DrawChar(110,140,'5');
					LCD_DrawChar(190,140,'6');
					LCD_DrawChar(30,210,'7');
					LCD_DrawChar(110,210,'8');
					LCD_DrawChar(190,210,'9');
					LCD_DrawChar(110,280,'0');
					LCD_DrawLine(30,280,60,280,WHITE);
					LCD_DrawLine(30,280,50,300,WHITE);
					LCD_DrawLine(30,280,50,260,WHITE);
					LCD_DrawLine(170,280,200,280,WHITE);
					LCD_DrawLine(200,280,180,300,WHITE);
					LCD_DrawLine(200,280,180,260,WHITE);
					while(y<4){
					
						sprintf(output, "%02d", x1);
		LCD_DrawString(80,10,output);
				sprintf(output, "%02d", x2);
						LCD_DrawString(100,10,":");
		LCD_DrawString(110,10,output);
						
						x=Counter_touchpad();
						if(x<12){
						if(x<10){
							if(y==0){
							x1=x*10;
							LCD_DrawLine(80,25,88,25,BLACK);
							}
							else if (y==1){
								x1=x1+x;
								LCD_DrawLine(80,25,88,25,WHITE);
								LCD_DrawLine(88,25,96,25,BLACK);
							}
							else if(y==2){
							x2=x*10;
																LCD_DrawLine(88,25,96,25,WHITE);
								LCD_DrawLine(110,25,118,25,BLACK);
							}
							else if(y==3){
							x2=x2+x;
								LCD_DrawLine(110,25,118,25,WHITE);
								LCD_DrawLine(118,25,126,25,BLACK);
							}
							y++;
						}
							else if(x==10){
							if(y>0){
							y--;
							}
							}
						else if(x==11){
						y++;
						}
						}
						else {
						if(y==0){
							LCD_DrawLine(80,25,88,25,BLACK);
							LCD_DrawLine(88,25,96,25,WHITE);
							}
							else if (y==1){
								LCD_DrawLine(80,25,88,25,WHITE);
								LCD_DrawLine(88,25,96,25,BLACK);
								LCD_DrawLine(110,25,118,25,WHITE);
							}
							else if(y==2){
																LCD_DrawLine(88,25,96,25,WHITE);
								LCD_DrawLine(118,25,126,25,WHITE);
								LCD_DrawLine(110,25,118,25,BLACK);
								
							}
							else if(y==3){
								LCD_DrawLine(110,25,118,25,WHITE);
								LCD_DrawLine(118,25,126,25,BLACK);
							}
						
						}
					HAL_Delay(100);
						x=12;
						
					}
					
					x=0;
					setted=1;
					x3=(x1*6000)+(x2*100);
					LCD_INIT();
			LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
			LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Pause",GREY,BLACK);
				LCD_Clear ( 150, 270 ,  60, 50, GREEN	);
						LCD_DrawString_Color(160,290,"Reset",GREEN,BLACK);
			
					LCD_Clear ( 30, 0 ,  100, 50, CYAN	);
			LCD_DrawString_Color(55,15,"Timer",CYAN,BLACK);
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
		
		
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
						LCD_DrawString(105,150,":");
				LCD_DrawString(145,150,":");
				}
			else	if(x==1){
						countercounter=1;
					LCD_Clear ( 10, 270 ,  60, 50, GREY	);
			LCD_Clear ( 80, 270 ,  60, 50, RED	);
					LCD_DrawString_Color(20,290,"Start",GREY,BLACK);
			LCD_DrawString_Color(90,290,"Pause",RED,BLACK);
				}
						else	if(x==2){
							
							countercounter=0;
				LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
				LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Pause",GREY,BLACK);
				}
							else	if(x==3){
						cnt_tim3_10ms=0;
					countercounter=0;
				LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
				LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Pause",GREY,BLACK);
				
				}
				if(setted==1){
					
				digit3=((x3-cnt_tim3_10ms))%100;
					sprintf(output, "%02d", digit3);
				LCD_DrawString(160,150,output);
				digit2=(x3-cnt_tim3_10ms)/100;
				digit1=digit2/60;
				digit2=digit2%60;
				sprintf(output, "%02d", digit2);
		LCD_DrawString(120,150,output);
				sprintf(output, "%02d", digit1);
		LCD_DrawString(80,150,output);
					
					if(x3<cnt_tim3_10ms){
						LCD_INIT();
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
						LCD_Clear(0,0,240,320,RED);
						LCD_DrawString_Color(80,150,"Time's up",RED,BLUE);
						HAL_Delay(500);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
						HAL_Delay(500);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
						HAL_Delay(500);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
						HAL_Delay(500);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
						HAL_Delay(500);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
						cnt_tim3_10ms=0;
					countercounter=0;
						LCD_INIT();
			LCD_Clear ( 10, 270 ,  60, 50, RED	);
			LCD_Clear ( 80, 270 ,  60, 50, GREY	);
					LCD_DrawString_Color(20,290,"Start",RED,BLACK);
			LCD_DrawString_Color(90,290,"Pause",GREY,BLACK);
				LCD_Clear ( 150, 270 ,  60, 50, GREEN	);
										LCD_DrawString_Color(160,290,"Reset",GREEN,BLACK);
											LCD_Clear ( 30, 0 ,  100, 50, CYAN	);
				LCD_DrawString_Color(55,15,"Timer",CYAN,BLACK);

			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
		
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
						LCD_DrawString(105,150,":");
				LCD_DrawString(145,150,":");
					}
				}
				if(ConvertedValue2==1){
					x=6;
				}
			}
			if(x==6){
			mod=0;
				mod2=0;
			}
			else {mod2=0;}
    }	
	}			
		else if ( mod == 3 )	         
    {
			LCD_INIT();
			int x=0;
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
			LCD_Clear(50,0,100,30,BLUE);
			LCD_DrawString_Color(70,5,"Refresh",BLUE,WHITE);
			LCD_DrawString(5,5,"Color");
			while(x<=1){
				x=Draw_touchkey(ConvertedValue);
			if(ConvertedValue2==1){
					x=2;
				}
			if(ConvertedValue==0){
			LCD_Clear(15,20,10,10,BLACK);
		}
		else if(ConvertedValue==1){
			LCD_Clear(15,20,10,10,RED);
		}
		else if(ConvertedValue==2){
			LCD_Clear(15,20,10,10,GREEN);
		}
		else if(ConvertedValue==3){
			LCD_Clear(15,20,10,10,BLUE);
		}
			if(x==1){
				LCD_INIT();
			int x=0;
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
			LCD_Clear(50,0,100,30,BLUE);
			LCD_DrawString_Color(70,5,"Refresh",BLUE,WHITE);
			LCD_DrawString(5,5,"Color");
				x=0;			
			}
			}
			mod=0;
			     
    }				
		else if ( mod == 4 )	         
    {
			LCD_INIT();
			LCD_Clear ( 0,  0,  240, 320, YELLOW	);
double angle;
	int temp=0;
	uint8_t CRA = 0x70; 
	uint8_t CRB = 0xA0;
	uint8_t abc = 0x01;
	uint8_t x1; uint8_t x2; uint8_t y1; uint8_t y2;
	int16_t x = 0; int16_t y = 0; 

		LCD_DrawString_Color(100,130,"Angle:",YELLOW,BLACK);
			LCD_DrawString_Color(70,190,"Direction:",YELLOW,BLACK);
			LCD_Clear ( 190, 0 ,  50, 50, BLACK	);
		LCD_DrawLine(190,0,240,50,RED);
		LCD_DrawLine(190,50,240,0,RED);
			while(temp==0){
				temp=Sub_touchkey();
			__HAL_RCC_FSMC_CLK_DISABLE();
			__HAL_RCC_I2C1_CLK_ENABLE();
				HAL_I2C_Mem_Write(&hi2c1,0x1E<<1,0x00,1,&CRA,1,100);
HAL_I2C_Mem_Write(&hi2c1,0x1E<<1,0x01,1,&CRB,1,100);
		HAL_I2C_Mem_Write(&hi2c1,0x1E<<1,0x02,1,&abc,1,100);
		HAL_Delay(50);

		HAL_I2C_Mem_Read(&hi2c1,0x1E<<1,0x03,1,&x1,1,100);
		HAL_I2C_Mem_Read(&hi2c1,0x1E<<1,0x04,1,&x2,1,100);
		x=((uint16_t)x1 << 8) | x2;
		HAL_I2C_Mem_Read(&hi2c1,0x1E<<1,0x07,1,&y1,1,100);
		HAL_I2C_Mem_Read(&hi2c1,0x1E<<1,0x08,1,&y2,1,100);
			y=((uint16_t)y1 << 8) | y2;
	
		__HAL_RCC_I2C1_CLK_DISABLE();
		__HAL_RCC_FSMC_CLK_ENABLE();
	char output[16];
		
	angle=atan2((double)x,(double)y);
		angle=angle*180/PI;
		int degree=angle;
		 degree=(degree+360)%360;
		sprintf(output, "%03d", degree);
	LCD_DrawString_Color(150,130,output,YELLOW,BLACK);
		if(degree>337.5||degree<22.5){
			LCD_DrawString_Color(150,190,"N",YELLOW,BLACK);
			LCD_Clear(158,190,8,16,YELLOW);
		}
		else if (degree>22.5&&degree<67.5){
			LCD_DrawString_Color(150,190,"NE",YELLOW,BLACK);
		}
		else if (degree>67.5&&degree<112.5){
			LCD_DrawString_Color(150,190,"E",YELLOW,BLACK);	LCD_Clear(158,190,8,16,YELLOW);
		}
		else if (degree>112.5&&degree<157.5){
			LCD_DrawString_Color(150,190,"SE",YELLOW,BLACK);
		}
		else if (degree>157.5&&degree<202.5){
			LCD_DrawString_Color(150,190,"S",YELLOW,BLACK);	LCD_Clear(158,190,8,16,YELLOW);
		}
		else if (degree>202.5&&degree<247.5){
			LCD_DrawString_Color(150,190,"SW",YELLOW,BLACK);
		}
		else if (degree>247.5&&degree<292.5){
			LCD_DrawString_Color(150,190,"W",YELLOW,BLACK);	LCD_Clear(158,190,8,16,YELLOW);
		}
		else if (degree>292.5&&degree<337.5){
			LCD_DrawString_Color(150,190,"NW",YELLOW,BLACK);
		}
			if(ConvertedValue2==1){
			temp=1;
			}
			}
			mod=0;           
    }				
		
		if (ConvertedValue2==1){
		LCD_INIT();
			char output[16];
			LCD_DrawString(105,150,":");
				LCD_DrawString(145,150,":");
			mod=0;
			
			

		while(ConvertedValue2==1){
		
		int digit1=0;
				int digit2=0;
				int digit3=0;
			int digit4;
			digit4=(cnt_tim4_10ms/100)+TIMES;
					
				digit3=digit4/3600;
				digit2=(digit4-(digit3*3600))/60;
				digit1=digit4-((digit3*3600)+(digit2*60));
			sprintf(output, "%02d", digit1);
				LCD_DrawString(160,150,output);
				sprintf(output, "%02d", digit2);
		LCD_DrawString(120,150,output);
				sprintf(output, "%02d", digit3);
		LCD_DrawString(80,150,output);
		}
	}
		HAL_Delay(50);		
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

