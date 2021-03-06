/***********************************************************************
文件名称：LED.C
功    能：led  IO初始化
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#include <stm32f4xx_hal.h>
#include <cstdint>
#include <stm32f407xx.h>
#include <LED.H>
#include "main.h"

/***********************************************************************
函数名称：LED_Configuration(void)
功    能：完成LED的配置
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void LED_Configuration(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
    __GPIOE_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

}

/***********************************************************************
函数名称：LED_Blink(void)
功    能：完成LED闪烁
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void LED_Blink(void)
{
	/*****熄灭四个led灯******/
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	LED_Delay(0x4fffff);
	/*****点亮四个led灯******/
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
	LED_Delay(0x4fffff);
}
/***********************************************************************
函数名称：One_LED_ON(unsigned char led_num)
功    能：实现点亮一个LED灯
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void One_LED_ON(unsigned char led_num)
{	
	/*****熄灭四个led灯******/
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;

	switch(led_num)
	{
		case 1:
		{
			LED1_ON;
			break;
		}
		case 2:
		{
			LED2_ON;
			break;		
		}
		case 3:
		{
			LED3_ON;
			break;		
		}
		case 4:
		{
			LED4_ON;
			break;		
		}
		default:
		{
			break;	
		}
	}		
}

void Multiple_LED_ON(unsigned char led_num)
{
    /*****熄灭四个led灯******/
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;

    switch(led_num)
    {
        case 1:
        {
            LED1_ON;
            break;
        }
        case 2:
        {
            LED1_ON;
            LED2_ON;
            break;
        }
        case 3:
        {
            LED1_ON;
            LED2_ON;
            LED3_ON;
            break;
        }
        case 4:
        {
            LED1_ON;
            LED2_ON;
            LED3_ON;
            LED4_ON;
            break;
        }
        default:
        {
            break;
        }
    }
}


void static LED_Delay(uint32_t nCount)
{ 
  while(nCount > 0)
  { 
  	  nCount --;   
  }
}
