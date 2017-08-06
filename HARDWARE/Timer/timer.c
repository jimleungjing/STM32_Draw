#include "main.h"

Relay_State_e Relay_State=GearIII;

void TIM2_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	tim.TIM_Period=arr;
	tim.TIM_Prescaler=psc;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&tim);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
	nvic.NVIC_IRQChannel=TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=0x01;
	nvic.NVIC_IRQChannelSubPriority=0x01;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
}

void TIM2_IRQHandler(void)
{
	double Vamp=amplitude*1.0f/2048;				//Vref:1 V
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)			//Òç³öÖÐ¶Ï
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		switch(Relay_State)
		{
			case GearI:										//x20
			{
				Relay1(Off);
				Relay2(On);
				if(OVR_IN==1 && amplitude>2000)
				{
					Relay_State=GearIII;
				}
				else
				{
					DAC8560_Send(0,DACBias+0.1f*Vamp);
					DAC8531_Send(0,DACBias+0.9f*Vamp);
				}
			}break;
			case GearII:									//x2
			{
				Relay1(On);
				Relay2(Off);
				if(amplitude<204)							//Amp<100mV
				{
					Relay_State=GearI;
				}
				else if(OVR_IN==1 && amplitude>2000)
				{
					Relay_State=GearIII;
				}
				else
				{
					DAC8560_Send(0,DACBias+0.1f*Vamp);
					DAC8531_Send(0,DACBias+0.9f*Vamp);
				}
			}break;
			case GearIII:									//x0.2
			{
				Relay1(Off);
				Relay2(Off);
				if(amplitude<204)							//Amp<100mV
				{
					Relay_State=GearII;
				}
				else
				{
					DAC8560_Send(0,DACBias+0.1f*Vamp);
					DAC8531_Send(0,DACBias+0.9f*Vamp);
				}
			}break;
			default:break;
		}
	}
}
