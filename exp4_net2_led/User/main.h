/***********************************************************************
文件名称：main.h
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "string.h"
#include "UDP_SERVER.h"
#include "I2C.H"

#define RMII_MODE

void Time_Update(void);

#endif
