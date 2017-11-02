
/***********************************************************************
文件名称：UDP_SERVER.h
功    能：完成UDP的数据收发
编写时间：2013.4.25
编 写 人：赵志峰
注    意：
***********************************************************************/
#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_



/***************开发板ip及MAC定义*************************/
#define BOARD_IP  			 	192,168,1,11   		//开发板ip 
#define BOARD_NETMASK   	 	255,255,255,0   		//开发板子网掩码
#define BOARD_WG		   	 	192,168,1,1   			//开发板子网关
#define BOARD_MAC_ADDR      	0,0,0,0,0,1				//开发板MAC地址

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

