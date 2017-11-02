/***********************************************************************
�ļ����ƣ�SCI.h
��    �ܣ�
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/
#ifndef  _SCI_H_
#define  _SCI_H_

#define RS232_REC_BUFF_SIZE				1024
#include "stm32f4xx.h"
extern volatile unsigned char RS232_REC_Flag ;
extern volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE] ;//���ڽ�������
extern volatile unsigned int RS232_rec_counter ;//����RS232���ռ���

extern unsigned short RS232_send_data_length;

void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void RS232_DMA_Send(unsigned char *send_buff,unsigned int length);
void USART_232_Configuration(void);
static void RS232_Delay(uint32_t nCount);


#endif
