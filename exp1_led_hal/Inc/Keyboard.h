//
// Created by tangyq on 2017/10/12.
//

#ifndef EXP1_LED_KEYBOARD_H
#define EXP1_LED_KEYBOARD_H

#include <cstdint>
#include <stm32f407xx.h>

void Key_Configuration(void);

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

uint8_t Key_Operate(void);

#endif
