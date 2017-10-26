/***********************************************************************
文件名称：main.C
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include <misc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "main.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"

//extern unsigned char usart1_to_send_flag;
extern uint16_t usart1_aRxData[USART1_RXDATASIZE];

void USART_SendString(USART_TypeDef *USARTx, uint16_t Data[]);

void NVIC_Config(void);

void USART_Config(void);

void USART_Gpio_Config(void);

void NVIC_TIM5Configuration(void);

void TIM5_Configuration(void);

void TIM5_GPIO_Configuration(void);

uint16_t i = 0, k = 0;
uint8_t sendData = 0;
uint8_t usart1_send_flag = 0;

void delay(uint32_t nCount);

int main(void) {
    uint8_t keycode = 0;
    /*
        ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
        配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM等。
    */
    NVIC_Config(); //串口中断配置函数
    USART_Config(); //串口配置函数
    Key_Configuration();//IO口配置函数
    NVIC_TIM5Configuration();//定时器中断配置函数
    TIM5_Configuration();//定时器配置函数

    while (1) {
        keycode = Key_Operate();
        //LED_Blink();
        if (keycode == 1) {
            if (usart1_send_flag == 0) usart1_send_flag = 1;
        }

    }
}

void NVIC_Config() {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                                  //此处填写优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                         //此处填写优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void USART_Gpio_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    //USART1 Tx(PA.09)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);        //管脚PA9复用为USART1
    //USART1 Rx(PA.10)

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

}

void USART_Config(void) {
    USART_InitTypeDef USART_InitStruct;
    USART_ClockInitTypeDef USART_ClockInitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    USART_DeInit(USART1);
    USART_StructInit(&USART_InitStruct);
    USART_ClockStructInit(&USART_ClockInitStruct);

    USART_Gpio_Config();

    USART_ClockInit(USART1, &USART_ClockInitStruct);

    USART_InitStruct.USART_BaudRate = 115200;                                                 //波特率可自行更改
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);

    USART_ClearITPendingBit(USART1, USART_IT_TC);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    USART_ClearITPendingBit(USART1, USART_IT_TXE);
    USART_Cmd(USART1, ENABLE);
}
/*
void TIM5_GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
}*/

/*500ms 定时中断*/
void TIM5_Configuration(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    //时基初始化
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1679; //一分频，TIM5的时钟频率为84MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //ClockDivision一定得配置否则进不了中断
    TIM_TimeBaseInitStructure.TIM_Period = 24999;                                                    //此处设置定时器常数
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 12499;// 占空比50%，改这个值可以修改占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM5, &TIM_OCInitStructure);//配置TIM5的CH3输出
    TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Disable);//使配置立刻生效
    TIM_ClearFlag(TIM5, TIM_IT_CC3);
    TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);//此处中断标志使TIM5的CC3。
    TIM_Cmd(TIM5, ENABLE);    //开启定时器
}

void NVIC_TIM5Configuration(void)        //中断配置器
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                                    //此处填写中断优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                                           //此处填写中断优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART_SendString(USART_TypeDef *USARTx, uint16_t Data[]) {
    uint8_t index = 9;
    for (; index >= 0; index--) {
        USART_SendData(USARTx, Data[index]);
        delay(1000);
    }
    //usart1_to_send_flag = 0;
}

void delay(uint32_t nCount) {
    while (nCount > 0) {
        nCount--;
    }
}

void USART1_IRQHandler(void) {
    uint16_t tmp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        //flag = 1;
        tmp = USART_ReceiveData(USART1);
        if (tmp == 0xAA){
            usart1_send_flag = 0;
        }







        /*
#ifdef EXP2_1
        USART_SendData(USART1, tmp);
        if (usart1_aRxData_Index >= USART1_RXDATASIZE) {
            usart1_aRxData_Index = 0;
        } else {
            usart1_aRxData[usart1_aRxData_Index++] = tmp;
        }

#endif

#ifdef EXP2_2

        if (usart1_aRxData_Index >= 9) {
            usart1_aRxData[usart1_aRxData_Index++] = tmp;
            if (tmp == 0xff){
                usart1_to_send_flag = 1;
                //USART_SendString(USART1, usart1_aRxData);
            }
            usart1_check_flag = 0;
            usart1_aRxData_Index = 0;
        } else if (usart1_check_flag == 1){
            usart1_aRxData[usart1_aRxData_Index++] = tmp;
        } else if (tmp == 0xaa){
            usart1_check_flag = 1;
            usart1_aRxData_Index = 0;
            usart1_aRxData[usart1_aRxData_Index++] = tmp;
        }

#endif*/
    }
}


void TIM5_IRQHandler(void) {
    if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET) {
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
        if (usart1_send_flag == 1) {
            USART_SendData(USART1, sendData);
            sendData++;
        }else if (usart1_send_flag == 0){
            sendData = 0;
        }
    }
}
