#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "main.h"
#define USART3_REC_LEN  		200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收

#define _USART3_RX_BUF_SIZE 128u
#define _USART3_DMA_RX_BUF_LEN 30u
#define _USART3_RX_BUF_SIZE_IN_FRAMES (_USART3_RX_BUF_SIZE/RC_FRAME_LENGTH)
#define RC_FRAME_LENGTH 18u
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;        

void usart3_init(u32 bound);
void USART3_IRQHandler(void);

#endif
