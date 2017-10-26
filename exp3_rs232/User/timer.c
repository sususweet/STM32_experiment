#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include "stm32f4xx.h"
#include "NVIC.H"

extern volatile u8 ReceiveState;

void InitiateTimer(void);

void ResetTimer(void);

void CloseTimer(void);

void InitiateTimer(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Prescaler = 83;                    /* ?????? */
    TIM_TimeBaseStructure.TIM_Period = 10000;                      //10ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            /* ???? */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        /* ?????? */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void ResetTimer(void) {
    TIM_Cmd(TIM3, ENABLE);
    TIM3->CNT = 0;
}

void CloseTimer(void) {
    TIM_Cmd(TIM3, DISABLE);
}


void TIM3_IRQHandler(void) {
    ReceiveState = 0;
    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
}
