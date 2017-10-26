/***********************************************************************
�ļ����ƣ�main.C
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
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
        ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
        ����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM�ȡ�
    */
    NVIC_Config(); //�����ж����ú���
    USART_Config(); //�������ú���
    Key_Configuration();//IO�����ú���
    NVIC_TIM5Configuration();//��ʱ���ж����ú���
    TIM5_Configuration();//��ʱ�����ú���

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

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                                  //�˴���д���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                         //�˴���д���ȼ�
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
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);        //�ܽ�PA9����ΪUSART1
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

    USART_InitStruct.USART_BaudRate = 115200;                                                 //�����ʿ����и���
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
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
}*/

/*500ms ��ʱ�ж�*/
void TIM5_Configuration(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    //ʱ����ʼ��
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1679; //һ��Ƶ��TIM5��ʱ��Ƶ��Ϊ84MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //ClockDivisionһ�������÷���������ж�
    TIM_TimeBaseInitStructure.TIM_Period = 24999;                                                    //�˴����ö�ʱ������
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 12499;// ռ�ձ�50%�������ֵ�����޸�ռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM5, &TIM_OCInitStructure);//����TIM5��CH3���
    TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Disable);//ʹ����������Ч
    TIM_ClearFlag(TIM5, TIM_IT_CC3);
    TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);//�˴��жϱ�־ʹTIM5��CC3��
    TIM_Cmd(TIM5, ENABLE);    //������ʱ��
}

void NVIC_TIM5Configuration(void)        //�ж�������
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                                    //�˴���д�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                                           //�˴���д�ж����ȼ�
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
