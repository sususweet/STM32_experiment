//
// Created by tangyq on 2017/10/12.
//
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "main.h"

void Key_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   //输入模式
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
  if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0) 
		return 0;
	else return 1;
}

uint8_t Key_Operate(){
    if (Key_Scan(GPIOE, GPIO_Pin_12) == 0) return 1;
    if (Key_Scan(GPIOE, GPIO_Pin_13) == 0) return 2;
    if (Key_Scan(GPIOE, GPIO_Pin_14) == 0) return 3;
    if (Key_Scan(GPIOE, GPIO_Pin_15) == 0) return 4;
    return 0;
}
