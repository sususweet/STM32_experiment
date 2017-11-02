/***********************************************************************
文件名称：UDP_SERVER.C
功    能：完成UDP的数据收发
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#include "main.h"

unsigned char BMPx[1024];
uint16_t Bk;
uint16_t k = 0, j = 0;
unsigned char kk[9];  //用来设定返回的值
u8 RFrameFlag, SframeFlag; //接收到一帧标志；=1有效
FrameDef ReceivedFrame, SendingFrame;
u8 FileRcvCompleteFlag = 0;      //文件接收完成标志；=1有效
u8 LastFrameSn = 255;            //上次接收帧序号
u16 FileLength = 0, FilePointer = 0;


/***********************************************************************
函数名称：udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,struct ip_addr *addr, u16_t port)
功    能：udp数据接收和发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个udp段到达这个连接时会被调用
***********************************************************************/
void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port) {
    struct ip_addr destAddr = *addr; /* 获取远程主机 IP地址 */
    struct pbuf *p_temp = p;
    struct pbuf *pp;
    char *data;
    unsigned char temp[40];
    uint16_t i = 0;
    while (p_temp != NULL)            //收到数据，开始进行帧操作。因为帧较短，所以本循环只进行第一次。
    {

        if (SframeFlag == 1 && RFrameFlag == 1) {
            //在此处填写代码
            udp_sendto(pcb, p_temp, &destAddr, port); /* 返回确认帧 */
        }


        if (FileRcvCompleteFlag == 1)        //接收到一个文件
        {
            //在此处填写代码
        }

        p_temp = p_temp->next;
    }
    pbuf_free(p);                        /* 释放该UDP段 */
}

/***********************************************************************
函数名称：UDP_server_init(void)
功    能：完成UDP服务器的初始化，主要是使得UDP通讯快进入监听状态
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void UDP_server_init(void) {
    struct udp_pcb *pcb;
    pcb = udp_new();                                //申请udp控制块
    udp_bind(pcb, IP_ADDR_ANY, UDP_LOCAL_PORT);        /* 绑定本地IP地址和端口号（作为udp服务器） */
    udp_recv(pcb, udp_server_recv, NULL);                /* 设置UDP段到时的回调函数 */

}

//将数组temp转化为结构体
void Change_struct(unsigned char *temp) {
    u16_t i;
    ReceivedFrame.Flag = temp[0];
    ReceivedFrame.SourceAddr = temp[1];
    ReceivedFrame.DestAddr = temp[2];
    ReceivedFrame.Command = temp[3];
    ReceivedFrame.SerialN = temp[4];
    ReceivedFrame.Length = temp[5];
    for (i = 6; i < 6 + temp[5]; i++) {
        ReceivedFrame.Data[i - 6] = temp[i];
    }
    ReceivedFrame.SumH = temp[6 + temp[5]];
    ReceivedFrame.SumL = temp[7 + temp[5]];
    if (ReceivedFrame.Flag == 0x7e && ReceivedFrame.Length <= 32)
        SframeFlag = 1;
}

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
    if (sum == (uint16_t) (ReceivedFrame.SumH * 256 + (uint16_t) (ReceivedFrame.SumL)))
        RFrameFlag = 1;
}

void SetAckFrame(void) {
    SendingFrame.Flag = 0x7f;
    SendingFrame.SourceAddr = ReceivedFrame.DestAddr;
    SendingFrame.DestAddr = ReceivedFrame.SourceAddr;
    SendingFrame.Command = ReceivedFrame.Command;
    SendingFrame.SerialN = ReceivedFrame.SerialN;
    SendingFrame.Length = 1;
    SendingFrame.Data[0] = 1;
}

void CheckRcvFrame(void) {
    u8 i, j;
    switch (ReceivedFrame.Command) {
        case 0x00:
            FileLength = ReceivedFrame.Data[0] * 256 + ReceivedFrame.Data[1];
            LastFrameSn = ReceivedFrame.SerialN;
            FilePointer = 0;
            FileRcvCompleteFlag = 0;
            SetAckFrame();
            break;

        case 0x01:
            if (((ReceivedFrame.SerialN - LastFrameSn) & 15) == 1) {

                for (i = 0; i < ReceivedFrame.Length; i++) {
                    BMPx[FilePointer] = ReceivedFrame.Data[i];
                    FilePointer++;
                }
            }
            SetAckFrame();
            LastFrameSn = ReceivedFrame.SerialN;
            break;
        case 0x81:
            FileRcvCompleteFlag = 1;
            SetAckFrame();;
            break;
    }
}

void SendFrame(void) {
    u8 i = 0, j;
    u16 sum;
    kk[i++] = 0x7e;
    kk[i++] = SendingFrame.SourceAddr;
    sum = SendingFrame.SourceAddr;
    kk[i++] = SendingFrame.DestAddr;
    sum += SendingFrame.DestAddr;
    kk[i++] = SendingFrame.Command;
    sum += SendingFrame.Command;
    kk[i++] = SendingFrame.SerialN;
    sum += SendingFrame.SerialN;
    kk[i++] = SendingFrame.Length;
    sum += SendingFrame.Length;
    for (j = 0; j < SendingFrame.Length; j++) {
        kk[i++] = SendingFrame.Data[j];
        sum += SendingFrame.Data[j];
    }
    kk[i++] = sum / 256;
    kk[i] = sum % 256;
}
