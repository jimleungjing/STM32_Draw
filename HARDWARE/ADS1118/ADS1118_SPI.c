/*
 * ADS1118_SPI.c
 *
 *  Created on: 2016年7月23日
 *      Author: snows
 */
#include "sys.h"
#include "delay.h"
#include "ADS1118_SPI.h"

u16 cmd_receiver;
ads1118_cmd ADS1118;

void ADS1118_SPI_port_init()
{
	GPIO_InitTypeDef ADS1118_GPIO;
	
	RCC_AHB1PeriphClockCmd(RCC_ADS1118_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_ADS1118_CS, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_ADS1118_DIN, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_ADS1118_DOUT, ENABLE);
	

	
	ADS1118_GPIO.GPIO_Mode=GPIO_Mode_OUT;
	ADS1118_GPIO.GPIO_OType=GPIO_OType_PP;
	ADS1118_GPIO.GPIO_Speed=GPIO_Speed_50MHz;
	ADS1118_GPIO.GPIO_PuPd = GPIO_PuPd_UP;
	
	ADS1118_GPIO.GPIO_Pin=ADS1118_CLK_Pin;
	GPIO_Init(ADS1118_CLK_GPIO,&ADS1118_GPIO);
	
	ADS1118_GPIO.GPIO_Pin=ADS1118_CS_Pin;
	GPIO_Init(ADS1118_CS_GPIO,&ADS1118_GPIO);
	
	ADS1118_GPIO.GPIO_Pin=ADS1118_DIN_Pin;
	GPIO_Init(ADS1118_DIN_GPIO,&ADS1118_GPIO);
	
	ADS1118_GPIO.GPIO_Pin=ADS1118_DOUT_Pin;
	ADS1118_GPIO.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(ADS1118_DOUT_GPIO,&ADS1118_GPIO);
	
	ADS1118_CLK_L;
	ADS1118_DIN_L;
	ADS1118_CS_H;
	
	ADS1118_cmd_init();
	ADS1118_write_and_read(ADS1118.CH2);
}

//32位数据传输周期。 应注意：理论上该函数返回的data_read是上一个指令cmd使能的通道转换的值
u16 ADS1118_write_and_read(u16 cmd)
{
	int i=0;
	u16 flag=0x8000;
	u16 cmd_temp;
	u16 data_read=0;
	cmd_receiver=0;
	cmd_temp=cmd;
	
	ADS1118_CS_L;
	delay_us(1);
	for(i=0;i<16;i++)
	{
		ADS1118_CLK_H;
		delay_us(1);
		if(cmd_temp&flag)
			ADS1118_DIN_H;
		else
			ADS1118_DIN_L;
		cmd_temp<<=1;
		delay_us(1);
//		data_read<<=1;
		ADS1118_CLK_L;
		delay_us(1);
		if(ADS1118_DOUT)
			data_read |=0x0001;
		else
			data_read&=~0x0001;
		data_read<<=1;
		delay_us(1);
	}
	cmd_temp=cmd;
	for(i=0;i<16;i++)
	{
		ADS1118_CLK_H;	
		delay_us(1);
		if(cmd_temp&flag)
			ADS1118_DIN_H;
		else
			ADS1118_DIN_L;
		cmd_temp<<=1;
		delay_us(1);
//		cmd_receiver<<=1;
		ADS1118_CLK_L;
		delay_us(1);
		if(ADS1118_DOUT)
			cmd_receiver |=0x0001;
		else
			cmd_receiver&=~0x0001;
		cmd_receiver<<=1;
		delay_us(1);
	}
	ADS1118_CS_H;
	delay_us(1500);					//等待写入cmd使能的采样转换完成;	取决于采样率：delay=1/x(SPS) S
	return (data_read);
}

double sample_to_value(u16 sample)
{
	double sample_temp;
	double value_return;
	sample_temp=sample;
	value_return=sample_temp*V_REF_AD/65535;
	return (value_return);
}

/*
 ****************************
 *Name : Cmd
 *Notes: 0x43C3 
	   ：   5 -> 0101,  2.048 V
	   :	3 -> 0011,  4.096 V
	   :     8-> 1000,  128 SPS(Default) 
	   :     C-> 1100,  475 SPS
	   :     E-> 1110,  860 SPS
	   :	Range : 0 V ~ 3.8 V
 ****************************
 */
void ADS1118_cmd_init(void)
{
	ADS1118.CH0=0x43E3|0x8000;
	ADS1118.CH1=0x53E3|0x8000;
	ADS1118.CH2=0x63E3|0x8000;
	ADS1118.CH3=0x73E3|0x8000;
}

void ReadADS1118MUX(void)
{
/*	ADS1118.AIN0=sample_to_value(ADS1118_write_and_read(ADS1118.CH1));		//sample_to_value
	ADS1118.AIN1=sample_to_value(ADS1118_write_and_read(ADS1118.CH2));		//CH2*/
	ADS1118.AIN2=sample_to_value(ADS1118_write_and_read(ADS1118.CH3));
	ADS1118.AIN3=sample_to_value(ADS1118_write_and_read(ADS1118.CH2));		//CH0
}


