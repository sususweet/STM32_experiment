
#include <stm32f4xx_gpio.h>
#include "main.h"
#include "stm32f4xx_conf.h"

/***********************************************************************
�ļ����ƣ�SCI.C
��    �ܣ���ɶ�usart1��usart2�Ĳ���
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/

volatile unsigned char RS232_REC_Flag = 0;
volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE];//���ڽ�������
volatile unsigned int RS232rec_counter = 0;//����RS232���ռ���

unsigned short RS232_send_data_length = 0;
volatile FrameDef ReceivedFrame, SendingFrame;
volatile u8 ReceiveState = 0, ReceiveLen = 0, SerialN = 0, RFrameFlag = 0;

void CheckFrameSum(void);

/***********************************************************************
�������ƣ�void USART_232_Configuration(void) 
��    �ܣ�
���������
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺RS232�õ���USART1
***********************************************************************/
void USART_232_Configuration(void) {

    GPIO_InitTypeDef GPIO_InitStructure;//����GPIO_InitTypeDef���͵Ľṹ���ԱGPIO_InitStructure

    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    //ʹ����Ҫ�õ���GPIO�ܽ�ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);
    //ʹ��USART1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    ///��λ����1
    USART_DeInit(USART1);

    USART_StructInit(&USART_InitStructure);//����Ĭ��USART����
    USART_ClockStructInit(&USART_ClockInitStruct);//����Ĭ��USART����
    //���ô���1�Ĺܽ� PA8 USART1_EN PA9 USART1_TX PA10 USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);        //�ܽ�PA9����ΪUSART1
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
    USART_ClearITPendingBit(USART1, USART_IT_TC);//����ж�TCλ

}

/***********************************************************************
�������ƣ�void USART1_IRQHandler(void) 
��    �ܣ����SCI�����ݵĽ��գ�������ʶ
���������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺RS232�õ���USART1
***********************************************************************/
void USART1_IRQHandler(void)            //��ȫ�˺���
{

    unsigned char temp = 0;
    ResetTimer();

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        temp = USART1->SR;
        temp = USART1->DR;                                                //��USART_IT_IDLE��־����������ִ����󴮿ڽ��յ������ݾ��Ѿ�����temp������

        switch (ReceiveState)        //ReceiveState���ڼ�¼�յ�����һ֡�е��ĸ��ֽڣ�������ֽ��մ����뽫�丳ֵΪ255
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
                RS232rec_counter = 0;       //ע������
                CheckFrameSum();
                break;

            case 255:  //���case�ﲻ��д����
                break;

            default:break;
        }
//		RS232_REC_Flag = 1;	   											//DMA���ձ�־

//		DMA_Cmd(DMA2_Stream5, DISABLE); 							  	//��ȡ���ݳ����ȹر�DMA 
//		RS232_send_data_length = RS232_REC_BUFF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);//��ȡDMA���յ����ݳ��ȣ�
//		
//		DMA_SetCurrDataCounter(DMA2_Stream5,RS232_REC_BUFF_SIZE);		//���ô������ݳ���    
//		DMA_Cmd(DMA2_Stream5, ENABLE);
    }
    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)                    //���ڷ����ж�
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
      //  RS232_dma_send_flag = 0;                                        //�����ٴη���
    }
}

/***********************************************************************
�������ƣ�RS232_DMA_Send(unsigned char *send_buff,unsigned int length)
��    �ܣ�RS232  DMA��ʽ�����ַ���
���������send_buff:�����͵�����ָ�룻length�����͵����ݳ��ȣ��ַ�������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/
//void RS232_DMA_Send(unsigned char *send_buff,unsigned int length)
//{
//	
//	unsigned int counter_232 = 0;
//	while(RS232_dma_send_flag != 0)					//�ȴ��ϴε�DMA�������
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
//	DMA_SetCurrDataCounter(DMA2_Stream7,length);  	//���ô��䳤��
//	DMA_Cmd(DMA2_Stream7,ENABLE);					//����DMA����
//}

/***********************************************************************
�������ƣ�RS232_Send_Data(unsigned char *send_buff,unsigned int length)
��    �ܣ�RS232�����ַ���
���������send_buff:�����͵�����ָ�룻length�����͵����ݳ��ȣ��ַ�������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
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
�������ƣ�void RS232_Delay(uint32_t nCount)
��    �ܣ�RS232�շ���ʱ����
���������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
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