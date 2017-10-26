/***********************************************************************
文件名称：LED.h
功    能：led  IO初始化
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/

#ifndef _LED_H_
#define _LED_H_

#include <cstdint>

#define LED1_ON 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET)
#define LED2_ON 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET)
#define LED3_ON 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET)
#define LED4_ON 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET)


#define LED1_OFF 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET)
#define LED2_OFF 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET)
#define LED3_OFF 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET)
#define LED4_OFF 		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET)

void LED_Configuration(void);
void LED_Blink(void);
void One_LED_ON(unsigned char led_num);
void Multiple_LED_ON(unsigned char led_num);
void static LED_Delay(uint32_t nCount);

#endif
