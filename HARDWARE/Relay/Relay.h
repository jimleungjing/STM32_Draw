#ifndef __RELAY_H
#define __RELAY_H

/*-----MACRO-----*/
#define Relay1_RCC 		RCC_AHB1Periph_GPIOB
#define Relay2_RCC 		RCC_AHB1Periph_GPIOB
#define OVR_RCC			RCC_AHB1Periph_GPIOB

#define Relay1_Port 	GPIOB
#define Relay2_Port 	GPIOB
#define OVR_Port		GPIOB

#define Relay1_Pin 		GPIO_Pin_4
#define Relay2_Pin		GPIO_Pin_5
#define OVR_Pin			GPIO_Pin_6

#define On				1
#define Off				0

#define Relay1(x)								\
		if(x)									\
		GPIO_SetBits(Relay1_Port,Relay1_Pin);	\
		else									\
		GPIO_ResetBits(Relay1_Port,Relay1_Pin);
		
#define Relay2(x)								\
		if(x)									\
		GPIO_SetBits(Relay2_Port,Relay2_Pin);	\
		else									\
		GPIO_ResetBits(Relay2_Port,Relay2_Pin);
		
#define OVR_IN			GPIO_ReadInputDataBit(OVR_Port,OVR_Pin)
		
typedef enum
{
	GearI=0,
	GearII=1,
	GearIII=2,
}Relay_State_e;

/*-----FUNCTION------*/
void Relay_Init(void);

#endif

