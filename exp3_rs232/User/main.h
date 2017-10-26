/***********************************************************************
�ļ����ƣ�main.h
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

extern void InitiateTimer(void);

extern void ResetTimer(void);

extern void CloseTimer(void);

#include "stm32f4xx.h"
#include "SCI.H"
#include "NVIC.H"
#include "DMA.H"
#include "I2C.H"

#endif

typedef struct {
    uint8_t Flag;
    uint8_t SourceAddr;
    uint8_t DestAddr;
    uint8_t Command;
    uint8_t SerialN;
    uint8_t Length;
    uint8_t Data[32];
    uint8_t SumH;
    uint8_t SumL;
} FrameDef;
