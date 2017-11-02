/***********************************************************************
文件名称：UDP_SERVER.C
功    能：完成UDP的数据收发
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#include <src/include/lwip/udp.h>
#include "main.h"
#include "UDP_SERVER.h"
uint8_t UDP_Send_Data[] = "CM4 received";

/***********************************************************************
函数名称：udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,struct ip_addr *addr, u16_t port)
功    能：udp数据接收和发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个udp段到达这个连接时会被调用
***********************************************************************/
void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,struct ip_addr *addr, u16_t port)
{
	struct ip_addr destAddr = *addr; /* 获取远程主机 IP地址 */
	struct pbuf *p_temp = p;
	int i=0;

    struct pbuf *pPbuf;

    pPbuf = pbuf_alloc(PBUF_RAW,sizeof(UDP_Send_Data),PBUF_POOL);
    pPbuf -> payload = (void *)UDP_Send_Data;       //把发送的数据首地址赋值给pbuf缓存中的payload
    udp_sendto(pcb,pPbuf,&destAddr,port);           //向对应IP，端口发送字符信息
    pbuf_free(pPbuf);                               //释放对应的pPbuf的空间

	//while(p_temp != NULL)
	{
		/******将数据原样返回 CM4 received*******************/
		
		udp_sendto(pcb,p_temp,&destAddr,port); /* 将收到的数据再发送出去 */	
		p_temp = p_temp->next;
	}
	pbuf_free(p); 						/* 释放该UDP段 */
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
void UDP_server_init(void)
{
	struct udp_pcb *pcb;
	pcb = udp_new();								//申请udp控制块
	udp_bind(pcb,IP_ADDR_ANY,UDP_LOCAL_PORT); 	    /* 绑定本地IP地址和端口号（作为udp服务器） */
	udp_recv(pcb,udp_server_recv,NULL); 				/* 设置UDP段到时的回调函数 */

}
