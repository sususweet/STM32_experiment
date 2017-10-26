//
// Created by tangyq on 2017/10/12.
//

#ifndef EXP1_LED_KEYBOARD_H
#define EXP1_LED_KEYBOARD_H

void Key_Configuration(void);

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

uint8_t Key_Operate(void);

#endif
