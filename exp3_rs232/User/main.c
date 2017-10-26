/***********************************************************************
文件名称：main.C
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "main.h"

extern unsigned char BMP1[];
extern unsigned char BMP2[];
extern unsigned char BMP3[];
int i;
extern FrameDef ReceivedFrame;  //接受桢会被存放在次结构体中
extern FrameDef SendingFrame;
extern u8 RFrameFlag;          //接收到一帧标志；=1表示接收完成，会在函数CheckFrameSum中置位，此函数用于接到桢的校验
u8 SFrameFlag = 0, count = 0;
u8 FileRcvCompleteFlag = 0;      //文件接收完成标志；=1接收完成
u8 LastFrameSn = 255;            //上次接收帧序号，上位机发送桢的顺序为0~15循环
u16 FileLength = 0, FilePointer = 0; //FilPionter用于将接到的数据帧中的数敬Filebuff
u8 FileBuff[1024];              //用于给OLED传输数据的数组
void CheckRcvFrame(void);       //检查收到的桢是命令帧，数据帧，还是结束桢的函数
void SendFrame(void);            //发送确认帧的函数
void SetAckFrame(void);            //设置确认帧的函数

int main(void) {
    /*
        ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
        配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM等。
    */
    USART_232_Configuration();
    InitiateTimer();
    NVIC_Configuration();
    I2C_Configuration();
    OLED_Init();
    DelayMs(10);
    OLED_CLS();//清屏
    OLED_DrawBMP(0, 0, 128, 8, BMP1);//将数组BMP1传给OLED并显示

    while (1) {
        if (RFrameFlag == 1)       //如果串口接收到一帧数据
        {
            CheckRcvFrame();
            SetAckFrame();
            SendFrame();
            RFrameFlag = 0;
        }
        if (FileRcvCompleteFlag == 1)  //如果文件接收完毕
        {
            OLED_DrawBMP(0, 0, 128, 8, FileBuff);
        }
    }
}


void CheckRcvFrame(void)                                                 //补全此函数
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
