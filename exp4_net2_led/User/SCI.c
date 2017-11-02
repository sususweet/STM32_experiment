
#include "main.h"

/***********************************************************************
文件名称：SCI.C
功    能：完成对usart1和usart2的操作
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/

volatile unsigned char RS232_REC_Flag = 0;
volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE];//用于接收数据
volatile unsigned int RS232rec_counter = 0;//用于RS232接收计数

unsigned short RS232_send_data_length = 0;






