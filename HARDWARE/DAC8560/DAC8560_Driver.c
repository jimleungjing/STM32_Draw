#include "main.h"

void DAC8560Init(void)
{
	GPIO_InitTypeDef  gpio[3];
	
	RCC_AHB1PeriphClockCmd(DAC_CS_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(DAC_CLK_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(DAC_DIN_RCC, ENABLE);
	
	gpio[0].GPIO_Pin=DAC_CS_PIN;
	gpio[0].GPIO_Mode=GPIO_Mode_OUT;
	gpio[0].GPIO_OType=GPIO_OType_PP;
	gpio[0].GPIO_Speed=GPIO_Speed_100MHz;
	gpio[0].GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DAC_CS_PORT,&gpio[0]);
	
	gpio[1].GPIO_Pin=DAC_CLK_PIN; 
	gpio[1].GPIO_Mode=GPIO_Mode_OUT;
	gpio[1].GPIO_OType=GPIO_OType_PP;
	gpio[1].GPIO_Speed=GPIO_Speed_100MHz;
	gpio[1].GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DAC_CLK_PORT,&gpio[1]);
	
	gpio[2].GPIO_Pin=DAC_DIN_PIN;
	gpio[2].GPIO_Mode=GPIO_Mode_OUT;
	gpio[2].GPIO_OType=GPIO_OType_PP;
	gpio[2].GPIO_Speed=GPIO_Speed_50MHz;
	gpio[2].GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DAC_DIN_PORT,&gpio[2]);

	DAC_CS_H;
	DAC_CLK_L;
	DAC_DIN_L;
}

void DAC8531Init(void)
{
	GPIO_InitTypeDef  gpio[3];
	
	RCC_AHB1PeriphClockCmd(DAC8531_CS_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(DAC8531_CLK_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(DAC8531_DIN_RCC, ENABLE);
	
	gpio[0].GPIO_Pin=DAC8531_CS_PIN;
	gpio[0].GPIO_Mode=GPIO_Mode_OUT;
	gpio[0].GPIO_OType=GPIO_OType_PP;
	gpio[0].GPIO_Speed=GPIO_Speed_100MHz;
	gpio[0].GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DAC8531_CS_PORT,&gpio[0]);
	
	gpio[1].GPIO_Pin=DAC8531_CLK_PIN; 
	gpio[1].GPIO_Mode=GPIO_Mode_OUT;
	gpio[1].GPIO_OType=GPIO_OType_PP;
	gpio[1].GPIO_Speed=GPIO_Speed_100MHz;
	gpio[1].GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DAC8531_CLK_PORT,&gpio[1]);
	
	gpio[2].GPIO_Pin=DAC8531_DIN_PIN;
	gpio[2].GPIO_Mode=GPIO_Mode_OUT;
	gpio[2].GPIO_OType=GPIO_OType_PP;
	gpio[2].GPIO_Speed=GPIO_Speed_50MHz;
	gpio[2].GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DAC8531_DIN_PORT,&gpio[2]);

	DAC8531_CS_H;
	DAC8531_CLK_L;
	DAC8531_DIN_L;
}


//24bits DATA={8'd0,16'dDATA}
void DAC8560WriteData(uint8_t Cmd,uint16_t Data)
{
	int cnt=0;
	//Cmd=0x00;
	DAC_CS_H;
	DAC_CS_L;
	for(cnt=0;cnt<8;cnt++)
	{
		DAC_CLK_H;
		if((Cmd<<cnt) & 0x80)
			DAC_DIN_H;
		else
			DAC_DIN_L;
		delay_us(1);
		DAC_CLK_L;
		delay_us(1);
	}
	for(cnt=0;cnt<16;cnt++)
	{
		DAC_CLK_H;
		if((Data<<cnt) & 0x8000)
			DAC_DIN_H;
		else
			DAC_DIN_L;
		delay_us(1);
		DAC_CLK_L;
		delay_us(1);
	}
	DAC_CS_H;
}

//24bits DATA={8'd0,16'dDATA}
void DAC8531WriteData(uint8_t Cmd,uint16_t Data)
{
	int cnt=0;
	//Cmd=0x00;
	DAC8531_CS_H;
	DAC8531_CS_L;
	for(cnt=0;cnt<8;cnt++)
	{
		DAC8531_CLK_H;
		if((Cmd<<cnt) & 0x80)
			DAC8531_DIN_H;
		else
			DAC8531_DIN_L;
		delay_us(1);
		DAC8531_CLK_L;
		delay_us(1);
	}
	for(cnt=0;cnt<16;cnt++)
	{
		DAC8531_CLK_H;
		if((Data<<cnt) & 0x8000)
			DAC8531_DIN_H;
		else
			DAC8531_DIN_L;
		delay_us(1);
		DAC8531_CLK_L;
		delay_us(1);
	}
	DAC8531_CS_H;
}


//value: xxxV;	Vout=Din*Vref/65535
void DAC8560_Send(uint8_t Cmd,double Vout)
{
	uint16_t Data=0;
	Data=Vout*1.0f/DACVref*65536;			//Default Vref:2.5V
	DAC8560WriteData(Cmd,Data);
}

void DAC8531_Send(uint8_t Cmd,double Vout)
{
	uint16_t Data=0;
	Data=Vout*1.0f/DACVref*65536;			//Default Vref:2.5V
	DAC8531WriteData(Cmd,Data);
}

