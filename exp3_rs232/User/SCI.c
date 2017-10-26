
#include <stm32f4xx_gpio.h>
#include "main.h"
#include "stm32f4xx_conf.h"

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
volatile FrameDef ReceivedFrame, SendingFrame;
volatile u8 ReceiveState = 0, ReceiveLen = 0, SerialN = 0, RFrameFlag = 0;

void CheckFrameSum(void);

/***********************************************************************
函数名称：void USART_232_Configuration(void) 
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：RS232用的是USART1
***********************************************************************/
void USART_232_Configuration(void) {

    GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO_InitTypeDef类型的结构体成员GPIO_InitStructure

    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    //使能需要用到的GPIO管脚时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);
    //使能USART1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    ///复位串口1
    USART_DeInit(USART1);

    USART_StructInit(&USART_InitStructure);//载入默认USART参数
    USART_ClockStructInit(&USART_ClockInitStruct);//载入默认USART参数
    //配置串口1的管脚 PA8 USART1_EN PA9 USART1_TX PA10 USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //复用
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);        //管脚PA9复用为USART1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    USART_ClockInit(USART1, &USART_ClockInitStruct);


    USART_InitStructure.USART_BaudRate = 19200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
// 	USART_ITConfig(USART1, USART_IT_TC, ENABLE);// 

    USART_Cmd(USART1, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_TC);//清除中断TC位

}

/***********************************************************************
函数名称：void USART1_IRQHandler(void) 
功    能：完成SCI的数据的接收，并做标识
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：RS232用的是USART1
***********************************************************************/
void USART1_IRQHandler(void)            //补全此函数
{

    unsigned char temp = 0;
    ResetTimer();

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        temp = USART1->SR;
        temp = USART1->DR;                                                //清USART_IT_IDLE标志，这条代码执行完后串口接收到的数据就已经存在temp里面了

        switch (ReceiveState)        //ReceiveState用于记录收到的是一帧中的哪个字节，如果出现接收错误请将其赋值为255
        {
            case 0x00:
                ReceivedFrame.Flag = temp;
                ReceiveState++;
                break;
            case 0x01:
                ReceivedFrame.SourceAddr = temp;
                ReceiveState++;
                break;
            case 0x02:
                ReceivedFrame.DestAddr = temp;
                ReceiveState++;
                break;
            case 0x03:
                ReceivedFrame.Command = temp;
                ReceiveState++;
                break;
            case 0x04:
                ReceivedFrame.SerialN = temp;
                ReceiveState++;
                break;
            case 0x05:
                ReceivedFrame.Length = temp;
                ReceiveState++;
                break;
            case 0x06:
                ReceivedFrame.Data[RS232rec_counter] = temp;
                RS232rec_counter++;
                if (RS232rec_counter >= ReceivedFrame.Length) ReceiveState++;
                break;
            case 0x07:
                ReceivedFrame.SumH = temp;
                ReceiveState++;
                break;
            case 0x08:
                ReceivedFrame.SumL = temp;
                ReceiveState = 0;
                RS232rec_counter = 0;       //注意清零
                CheckFrameSum();
                break;

            case 255:  //这个case里不用写东西
                break;

            default:break;
        }
//		RS232_REC_Flag = 1;	   											//DMA接收标志

//		DMA_Cmd(DMA2_Stream5, DISABLE); 							  	//读取数据长度先关闭DMA 
//		RS232_send_data_length = RS232_REC_BUFF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);//获取DMA接收的数据长度，
//		
//		DMA_SetCurrDataCounter(DMA2_Stream5,RS232_REC_BUFF_SIZE);		//设置传输数据长度    
//		DMA_Cmd(DMA2_Stream5, ENABLE);
    }
    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)                    //串口发送中断
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
      //  RS232_dma_send_flag = 0;                                        //允许再次发送
    }
}

/***********************************************************************
函数名称：RS232_DMA_Send(unsigned char *send_buff,unsigned int length)
功    能：RS232  DMA方式发送字符串
输入参数：send_buff:待发送的数据指针；length：发送的数据长度（字符个数）
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
//void RS232_DMA_Send(unsigned char *send_buff,unsigned int length)
//{
//	
//	unsigned int counter_232 = 0;
//	while(RS232_dma_send_flag != 0)					//等待上次的DMA传输完成
//	{
//		counter_232 ++;
//		if(counter_232 >= 0xffff)
//		{
//			break;
//		}
//	}						 
//	RS232_dma_send_flag = 1;
//	
//	
//	DMA2_Stream7 -> M0AR = (u32)send_buff;
//	DMA_SetCurrDataCounter(DMA2_Stream7,length);  	//设置传输长度
//	DMA_Cmd(DMA2_Stream7,ENABLE);					//启动DMA传输
//}

/***********************************************************************
函数名称：RS232_Send_Data(unsigned char *send_buff,unsigned int length)
功    能：RS232发送字符串
输入参数：send_buff:待发送的数据指针；length：发送的数据长度（字符个数）
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
void RS232_Send_Data(unsigned char *send_buff, unsigned int length) {
    unsigned int i = 0;
    for (i = 0; i < length; i++) {
        while ((USART1->SR & 0x40) == 0);
        USART1->DR = send_buff[i];
        while ((USART1->SR & 0x40) == 0);
    }
}

/***********************************************************************
函数名称：void RS232_Delay(uint32_t nCount)
功    能：RS232收发延时函数
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
// static void RS232_Delay(uint32_t nCount)
// { 
//   while(nCount > 0)
//   { 
//   	  nCount --;   
//   }
// }


void CheckFrameSum(void) {
    u16 sum = 0;
    u8 i;
    sum += (uint16_t) (ReceivedFrame.SourceAddr);
    sum += (uint16_t) (ReceivedFrame.DestAddr);
    sum += (uint16_t) (ReceivedFrame.Command);
    sum += (uint16_t) (ReceivedFrame.SerialN);
    sum += (uint16_t) (ReceivedFrame.Length);
    for (i = 0; i < ReceivedFrame.Length; i++) {
        sum += (uint16_t) (ReceivedFrame.Data[i]);
    }
    if (sum == (uint16_t) (ReceivedFrame.SumH * 256 + (uint16_t) (ReceivedFrame.SumL))) RFrameFlag = 1;


}
