
/***********************************************************************
�ļ����ƣ�UDP_SERVER.h
��    �ܣ����UDP�������շ�
��дʱ�䣺2013.4.25
�� д �ˣ���־��
ע    �⣺
***********************************************************************/
#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_



/***************������ip��MAC����*************************/
#define BOARD_IP  			 	192,168,1,11   		//������ip 
#define BOARD_NETMASK   	 	255,255,255,0   		//��������������
#define BOARD_WG		   	 	192,168,1,1   			//������������
#define BOARD_MAC_ADDR      	0,0,0,0,0,1				//������MAC��ַ

#define UDP_LOCAL_PORT     		8000 
#include "stm32f4xx.h"

typedef struct
{
  uint8_t Flag;            
	uint8_t SourceAddr; 
	uint8_t DestAddr; 
	uint8_t Command; 
	uint8_t SerialN; 
	uint8_t Length;
	uint8_t Data[32]; 
	uint8_t SumH; 
	uint8_t SumL; 
}FrameDef;

extern u8 RFrameFlag;

void UDP_server_init(void);
void Change_struct(unsigned char *temp);
void CheckFrameSum(void);
void ExtractData(void);
void SetAckFrame(void);
void CHECK(void);
void CheckRcvFrame(void);
void SendFrame(void);

#endif

