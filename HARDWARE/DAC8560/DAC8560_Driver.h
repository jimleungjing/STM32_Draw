#ifndef __DAC8560_DRIVER_H
#define __DAC8560_DRIVER_H
#include "stm32f4xx.h"

/*------MACRO------*/
/*------DAC8560----*/
#define DAC_CS_RCC		RCC_AHB1Periph_GPIOD
#define DAC_CLK_RCC		RCC_AHB1Periph_GPIOD
#define DAC_DIN_RCC		RCC_AHB1Periph_GPIOD

#define DAC_CS_PORT		GPIOD
#define DAC_CLK_PORT	GPIOD
#define DAC_DIN_PORT	GPIOD

#define DAC_CS_PIN		GPIO_Pin_3
#define DAC_CLK_PIN		GPIO_Pin_4
#define DAC_DIN_PIN		GPIO_Pin_2

#define DAC_CS_H		GPIO_SetBits(DAC_CS_PORT,DAC_CS_PIN)
#define DAC_CS_L		GPIO_ResetBits(DAC_CS_PORT,DAC_CS_PIN)
#define DAC_CLK_H		GPIO_SetBits(DAC_CLK_PORT,DAC_CLK_PIN)
#define DAC_CLK_L		GPIO_ResetBits(DAC_CLK_PORT,DAC_CLK_PIN)
#define DAC_DIN_H		GPIO_SetBits(DAC_DIN_PORT,DAC_DIN_PIN)
#define DAC_DIN_L		GPIO_ResetBits(DAC_DIN_PORT,DAC_DIN_PIN)

/*-------------------DAC8531--------------------*/
#define DAC8531_CS_RCC		RCC_AHB1Periph_GPIOD
#define DAC8531_CLK_RCC		RCC_AHB1Periph_GPIOD
#define DAC8531_DIN_RCC		RCC_AHB1Periph_GPIOD

#define DAC8531_CS_PORT		GPIOD
#define DAC8531_CLK_PORT	GPIOD
#define DAC8531_DIN_PORT	GPIOD

#define DAC8531_CS_PIN		GPIO_Pin_5
#define DAC8531_CLK_PIN		GPIO_Pin_6
#define DAC8531_DIN_PIN		GPIO_Pin_7

#define DAC8531_CS_H		GPIO_SetBits(DAC8531_CS_PORT,DAC8531_CS_PIN)
#define DAC8531_CS_L		GPIO_ResetBits(DAC8531_CS_PORT,DAC8531_CS_PIN)
#define DAC8531_CLK_H		GPIO_SetBits(DAC8531_CLK_PORT,DAC8531_CLK_PIN)
#define DAC8531_CLK_L		GPIO_ResetBits(DAC8531_CLK_PORT,DAC8531_CLK_PIN)
#define DAC8531_DIN_H		GPIO_SetBits(DAC8531_DIN_PORT,DAC8531_DIN_PIN)
#define DAC8531_DIN_L		GPIO_ResetBits(DAC8531_DIN_PORT,DAC8531_DIN_PIN)

#define DACVref			5.0f
#define DACBias			2.5f
#define ChannelA		0x00		//DAC8552
#define ChannelB		0x04		//DAC8552
/*-------FUNCTION--------*/
void DAC8560WriteData(uint8_t Cmd,uint16_t Data);
void DAC8531WriteData(uint8_t Cmd,uint16_t Data);

void DAC8560Init(void);
void DAC8531Init(void);

void DAC8560_Send(uint8_t Cmd,double Vout);
void DAC8531_Send(uint8_t Cmd,double Vout);
#endif 
