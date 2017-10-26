/***********************************************************************
文件名称：main.h
功    能：
编写时间：
编 写 人：
注    意：
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
