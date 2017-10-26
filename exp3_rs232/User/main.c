/***********************************************************************
�ļ����ƣ�main.C
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "main.h"

extern unsigned char BMP1[];
extern unsigned char BMP2[];
extern unsigned char BMP3[];
int i;
extern FrameDef ReceivedFrame;  //������ᱻ����ڴνṹ����
extern FrameDef SendingFrame;
extern u8 RFrameFlag;          //���յ�һ֡��־��=1��ʾ������ɣ����ں���CheckFrameSum����λ���˺������ڽӵ����У��
u8 SFrameFlag = 0, count = 0;
u8 FileRcvCompleteFlag = 0;      //�ļ�������ɱ�־��=1�������
u8 LastFrameSn = 255;            //�ϴν���֡��ţ���λ���������˳��Ϊ0~15ѭ��
u16 FileLength = 0, FilePointer = 0; //FilPionter���ڽ��ӵ�������֡�е�����Filebuff
u8 FileBuff[1024];              //���ڸ�OLED�������ݵ�����
void CheckRcvFrame(void);       //����յ�����������֡������֡�����ǽ�����ĺ���
void SendFrame(void);            //����ȷ��֡�ĺ���
void SetAckFrame(void);            //����ȷ��֡�ĺ���

int main(void) {
    /*
        ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
        ����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM�ȡ�
    */
    USART_232_Configuration();
    InitiateTimer();
    NVIC_Configuration();
    I2C_Configuration();
    OLED_Init();
    DelayMs(10);
    OLED_CLS();//����
    OLED_DrawBMP(0, 0, 128, 8, BMP1);//������BMP1����OLED����ʾ

    while (1) {
        if (RFrameFlag == 1)       //������ڽ��յ�һ֡����
        {
            CheckRcvFrame();
            SetAckFrame();
            SendFrame();
            RFrameFlag = 0;
        }
        if (FileRcvCompleteFlag == 1)  //����ļ��������
        {
            OLED_DrawBMP(0, 0, 128, 8, FileBuff);
        }
    }
}


void CheckRcvFrame(void)                                                 //��ȫ�˺���
{
    u8 i;
    switch (ReceivedFrame.Command) {
        case 0x00:
            FilePointer = 0;
            break;

        case 0x01:
            for (i = 0; i < ReceivedFrame.Length; i++) {
                FileBuff[FilePointer] = ReceivedFrame.Data[i];
                FilePointer++;
                if (FilePointer >= 1024) FilePointer = 0;
            }
            break;

        case 0x81:
            FileRcvCompleteFlag = 1;
            break;

    }
}

void SendFrame(void) {
    u8 i = 0, j;
    u16 sum;
    RS232_buff[i++] = 0x7e;
    RS232_buff[i++] = SendingFrame.SourceAddr;
    sum = SendingFrame.SourceAddr;
    RS232_buff[i++] = SendingFrame.DestAddr;
    sum += SendingFrame.DestAddr;
    RS232_buff[i++] = SendingFrame.Command;
    sum += SendingFrame.Command;
    RS232_buff[i++] = SendingFrame.SerialN;
    sum += SendingFrame.SerialN;
    RS232_buff[i++] = SendingFrame.Length;
    sum += SendingFrame.Length;
    for (j = 0; j < SendingFrame.Length; j++) {
        RS232_buff[i++] = SendingFrame.Data[j];
        sum += SendingFrame.Data[j];
    }
    RS232_buff[i++] = sum / 256;
    RS232_buff[i] = sum % 256;
    RS232_send_data_length = SendingFrame.Length + 8;
    RS232_Send_Data(RS232_buff, RS232_send_data_length);

}

void SetAckFrame(void) {
    SendingFrame.Flag = 0x7e;
    SendingFrame.SourceAddr = ReceivedFrame.DestAddr;
    SendingFrame.DestAddr = ReceivedFrame.SourceAddr;
    SendingFrame.Command = ReceivedFrame.Command;
    SendingFrame.SerialN = ReceivedFrame.SerialN;
    SendingFrame.Length = 1;
    SendingFrame.Data[0] = 1;
}
