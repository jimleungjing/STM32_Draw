#include "main.h"

void Relay_Init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(Relay1_RCC,ENABLE);
	RCC_AHB1PeriphClockCmd(Relay2_RCC,ENABLE);
	RCC_AHB1PeriphClockCmd(OVR_RCC,ENABLE);
	
	gpio.GPIO_Pin=Relay1_Pin;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(Relay1_Port,&gpio);
	
	gpio.GPIO_Pin=Relay2_Pin;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(Relay2_Port,&gpio);
	
	gpio.GPIO_Pin=OVR_Pin;
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(OVR_Port,&gpio);
	
	Relay1(Off);
	Relay2(Off);
}
