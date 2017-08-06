/*
 * ADS1118_SPI.h
 *
 *  Created on: 2016年7月23日
 *      Author: yusnows
 */

#ifndef ADS1118_SPI_H_
#define ADS1118_SPI_H_

#include "sys.h"

//ADS1118专属
#define SS						0x8000				//(BITF) 
#define ENA_CH0_GND			0x4000						//(BITE) 
#define ENA_CH1_GND			(0x4000+0x1000)			//(BITE+BITC)	
#define ENA_CH2_GND			(0x4000+0x2000)			//(BITE+BITD)
#define ENA_CH3_GND			0x4000+0x2000+0x1000) //(BITE+BITD+BITC) 

#define VOL_REF_6_144  		0x0000
#define V_REF_AD 				4.096
#define VOL_REF_4_096  		0x0100 		//(BIT9)
#define VOL_REF_2_048  	  	0x0200		//(BITA)


#define MODE_single			0x0x0080		//BIT8


#define DR						0x0x0040		//BIT7 //默认数据传输速率1238sps

#define TS_MODE				0X00 			//默认选择ADC模式

#define PULL_UP_EN 			0x0004		//BIT3

#define NOP						(0x0002+0x0001)	//(BIT2+BIT0) //默认写入指令为有效，如需写入无效指令则~NOP

//#define LLL_0						BIT0

//

typedef struct
{
	u16 CH0;
	u16 CH1;
	u16 CH2;
	u16 CH3;	
	double AIN0;
	double AIN1;
	double AIN2;
	double AIN3;
}ads1118_cmd;

extern ads1118_cmd ADS1118;

#define RCC_ADS1118_CS		RCC_AHB1Periph_GPIOC
#define RCC_ADS1118_CLK		RCC_AHB1Periph_GPIOC
#define RCC_ADS1118_DIN		RCC_AHB1Periph_GPIOC
#define RCC_ADS1118_DOUT	RCC_AHB1Periph_GPIOC 

#define ADS1118_CLK_GPIO	GPIOC
#define ADS1118_CS_GPIO		GPIOC
#define ADS1118_DIN_GPIO	GPIOC
#define ADS1118_DOUT_GPIO	GPIOC

#define ADS1118_CS_Pin		GPIO_Pin_6
#define ADS1118_CLK_Pin		GPIO_Pin_7
#define ADS1118_DIN_Pin		GPIO_Pin_8
#define ADS1118_DOUT_Pin	GPIO_Pin_9

#define ADS1118_CLK_H  		GPIO_SetBits(ADS1118_CLK_GPIO,ADS1118_CLK_Pin)
#define ADS1118_CLK_L   	GPIO_ResetBits(ADS1118_CLK_GPIO,ADS1118_CLK_Pin)

#define ADS1118_DIN_H 		GPIO_SetBits(ADS1118_DIN_GPIO,ADS1118_DIN_Pin)
#define ADS1118_DIN_L  		GPIO_ResetBits(ADS1118_DIN_GPIO,ADS1118_DIN_Pin)

#define ADS1118_DOUT		GPIO_ReadInputDataBit(ADS1118_DOUT_GPIO,ADS1118_DOUT_Pin)

#define ADS1118_CS_H    	GPIO_SetBits(ADS1118_CS_GPIO,ADS1118_CS_Pin)
#define ADS1118_CS_L     	GPIO_ResetBits(ADS1118_CS_GPIO,ADS1118_CS_Pin)


void ADS1118_cmd_init(void);
void ADS1118_SPI_port_init(void );
u16 ADS1118_write_and_read(u16 cmd);
double sample_to_value(u16 sample);
void ReadADS1118MUX(void);
//unsigned int ADS1118_single_CH_0();
//unsigned int ADS1118_single_CH_1();
//unsigned int ADS1118_single_CH_2();
//unsigned int ADS1118_single_CH_3();




#endif /* ADS1118_SPI_H_ */
