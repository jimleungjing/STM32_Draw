/*
 * OrthogonalSquareWave.h
 *
 *  Created on: 2017Äê3ÔÂ20ÈÕ
 *      Author: 99748
 */

#ifndef FPGASPI_H_
#define FPGASPI_H_
#include "sys.h"

/*******Pin define***********************/

#define FPGASPI_CS_RCC			RCC_AHB1Periph_GPIOB
#define FPGASPI_SCLK_RCC		RCC_AHB1Periph_GPIOB
#define FPGASPI_SDI_RCC			RCC_AHB1Periph_GPIOF
#define FPGASPI_SDO_RCC			RCC_AHB1Periph_GPIOG

#define FPGASPI_CS_GPIO			GPIOB
#define FPGASPI_SCLK_GPIO		GPIOB
#define FPGASPI_SDI_GPIO		GPIOF
#define FPGASPI_SDO_GPIO		GPIOG

#define FPGASPI_CS_Pin			GPIO_Pin_13
#define FPGASPI_SCLK_Pin		GPIO_Pin_12
#define FPGASPI_SDI_Pin			GPIO_Pin_14
#define FPGASPI_SDO_Pin			GPIO_Pin_0



#define FPGASPI_CS_H				GPIO_SetBits(FPGASPI_CS_GPIO,FPGASPI_CS_Pin)
#define FPGASPI_CS_L				GPIO_ResetBits(FPGASPI_CS_GPIO,FPGASPI_CS_Pin)

#define FPGASPI_SCLK_H				GPIO_SetBits(FPGASPI_SCLK_GPIO,FPGASPI_SCLK_Pin)
#define FPGASPI_SCLK_L				GPIO_ResetBits(FPGASPI_SCLK_GPIO,FPGASPI_SCLK_Pin)

#define FPGASPI_SDI_H				GPIO_SetBits(FPGASPI_SDI_GPIO,FPGASPI_SDI_Pin)
#define FPGASPI_SDI_L				GPIO_ResetBits(FPGASPI_SDI_GPIO,FPGASPI_SDI_Pin)

#define FPGASPI_SDO_IN				GPIO_ReadInputDataBit(FPGASPI_SDO_GPIO,FPGASPI_SDO_Pin)

u32 SPIWriteRead32Bits(u32 Data);
void FPGASPIPortInit(void);


#endif /* FPGASPI_H_ */

