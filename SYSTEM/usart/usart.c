#include "usart.h"
#include "sys.h"
#include "main.h"
//#include "FIFO.h"

//#define DMA_ENABLE 1
#if DMA_ENABLE
static uint8_t _USART3_DMA_RX_BUF[_USART3_DMA_RX_BUF_LEN];
static uint8_t _USART3_RX_BUFFER[_USART3_RX_BUF_SIZE_IN_FRAMES*_USART3_DMA_RX_BUF_LEN];
//static FIFO_t _USART3_RX_FIFO;
#endif
 
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收到0x0d
//bit14~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	

#if 1
int fputc(int ch, FILE *f);    //fputc重定向

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	return x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	USART3->DR = (u8) ch;   
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
   
	return ch;
}
#endif

void usart3_init(u32 bound)
{
  //GPIO Initialization
	GPIO_InitTypeDef GPIOInitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//	DMA_InitTypeDef	DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//Enable GPIOA Clock
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);//PB.10->USART.TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);//PB.11->USART.RX
	//USART3 PB.10,PB.11
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_OType=GPIO_OType_PP;
	GPIOInitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIOInitStructure);                  //Initialize PB.10|PB.11
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//Enable USART3 Clock
	
	USART_InitStructure.USART_BaudRate=bound;// default:9600
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);//初始化串口
	USART_Cmd(USART3,ENABLE);               //使能串口
	USART_ClearFlag(USART3,USART_FLAG_TC);

#if DMA_ENABLE
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);// Enable USART1 DMA Transmission
	
	DMA_DeInit(DMA1_Stream1);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	while(DMA_GetCmdStatus(DMA1_Stream1)!=DISABLE){};
	/*Configurate DMA Stream*/
	DMA_InitStructure.DMA_Channel=DMA_Channel_4;							//Select Channel
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)(&USART3->DR);		//DMA Peripheral Address
	DMA_InitStructure.DMA_Memory0BaseAddr=(uint32_t)&_USART3_DMA_RX_BUF[0]; //DMA Memory0 Address
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;				    //MemoryToPeripheral Mode
	DMA_InitStructure.DMA_BufferSize=sizeof(_USART3_DMA_RX_BUF)/2;
#endif

#if EN_USART3_RX
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启RXNE中断
   //USART3 NVIC Initialization
   NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;       //响应优先级1
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_Init(&NVIC_InitStructure);
#endif 
}
//写入LCD太费时间，影响USART的接收
void USART3_IRQHandler()
{
	uint8_t Res;
	//extern uint8_t ResUsart;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET) //接收中断，（接收到的数据必须是0x0d结尾）
	{
	    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		Res=USART_ReceiveData(USART3);//(USART3->DR); //读取接收到的数据
		//ResUsart=Res; 
		if(!(USART3_RX_STA&0x8000))   //接收未完成
		{
			//接收到了0x0d,即回车：carriage return
			  if(Res==0x0d) USART3_RX_STA|=0x8000;//接收错误，重新开始
              else 	
			//没接收到0x0d
		    {
                USART3_RX_BUF[USART3_RX_STA&0x7fff]=Res;
				USART3_RX_STA++;
				if(USART3_RX_STA>(USART3_REC_LEN-1)) USART3_RX_STA=0;//接收数据错误，重新接收		  
		    }
		}
	}

}
#endif
