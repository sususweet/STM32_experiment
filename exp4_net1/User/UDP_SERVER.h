
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
#define BOARD_IP  			 	192,168,1,250   		//开发板ip 
#define BOARD_NETMASK   	 	255,255,255,0   		//开发板子网掩码
#define BOARD_WG		   	 	192,168,1,1   			//开发板子网关
#define BOARD_MAC_ADDR      	13,24,22,1,0,1				//开发板MAC地址

#define UDP_LOCAL_PORT     		8000

void UDP_server_init(void);


#endif

