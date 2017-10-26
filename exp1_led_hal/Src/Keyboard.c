//
// Created by tangyq on 2017/10/12.
//

#include <stm32f407xx.h>
#include <stm32f4xx_hal.h>

#include "main.h"

void Key_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
	__GPIOE_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

}

uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0) return 0;
	return 1;
}

uint8_t Key_Operate(){
    if (Key_Scan(GPIOE, GPIO_PIN_12) == 0) return 1;
    if (Key_Scan(GPIOE, GPIO_PIN_13) == 0) return 2;
    if (Key_Scan(GPIOE, GPIO_PIN_14) == 0) return 3;
    if (Key_Scan(GPIOE, GPIO_PIN_15) == 0) return 4;
    return 0;
}
