
////////////////////////////////////////
//		PE0------CS(RD)
//		PE1------SCLK(WR)
//		PE2------UPDATE(U CLOCK)
//		PE3------SDIO(A0)
//		PE4------IO RESET(A2)
//		PE5------RST(RESET)
//		PE6------SDO(A1)
////////////////////////////////////////
//在写控制寄存器之前，默认更新时钟是内部
#ifndef __AD9854_H
#define __AD9854_H 

#include "sys.h"
/***************Base Data define***********************************/
#ifndef kHz
#define kHz						1000
#endif
#ifndef MHz
#define MHz						1000*kHz
#endif
#define AD9854_FREQ_REF			270000000			//MHz


/******************user interface function*************************************/
void AD9854WriteFreqSingle(double freq);
double AD9854ReadFreqSingle(void);
void AD9854Init(void);
void Update_AD9854(void);
void Freq_SW(void);

/*******************Base function*********************************************/
void AD9854WriteByte(u8 *Byte);
u8 AD9854ReadByte(void);
void AD9854IOReset(void);
void AD9854Reset(void);
void AD9854WriteCtR(unsigned char *ctr);
void AD9854PortInit(void);
void AMP_SETUP(unsigned char *IA,unsigned char *QA);	//幅度设置函数
void AD9854WriteFreqWord1(unsigned char *freq);
void AD9854WriteFerqWord2(unsigned char *freq);
uint64_t AD9854ReadFreqWord1(void );
uint64_t AD9854ReadFreqWord2(void);
void AD9854WritePhaseWord1(unsigned char *phase);
void AD9854WritePhaseWord2(unsigned char *phase);
void AD9854WriteFreqStay(unsigned char *freqstay);
void AD9854WriteFreqStep(unsigned char *freqstep);
void CalculateFreqWord(double freq);

/***************Pin define******************************************/
#define RCC_AD9854_CS				RCC_AHB1Periph_GPIOE
#define RCC_AD9854_SCLK				RCC_AHB1Periph_GPIOE
#define RCC_AD9854_UPDATE			RCC_AHB1Periph_GPIOE
#define RCC_AD9854_SDIO				RCC_AHB1Periph_GPIOE
#define RCC_AD9854_IO_RESET		RCC_AHB1Periph_GPIOE
#define RCC_AD9854_RST				RCC_AHB1Periph_GPIOE
#define RCC_AS9854_SDO				RCC_AHB1Periph_GPIOE

#define AD9854_CS_GPIO				GPIOE
#define AD9854_SCLK_GPIO			GPIOE
#define AD9854_UPDATE_GPIO			GPIOE
#define AD9854_SDIO_GPIO			GPIOE
#define AD9854_IO_RESET_GPIO		GPIOE
#define AD9854_RST_GPIO				GPIOE
#define AD9854_SDO_GPIO				GPIOE

#define AD9854_CS_Pin				GPIO_Pin_0
#define AD9854_SCLK_Pin				GPIO_Pin_1
#define AD9854_UPDATE_Pin			GPIO_Pin_2
#define AD9854_SDIO_Pin				GPIO_Pin_3
#define AD9854_IO_RESET_Pin		GPIO_Pin_4
#define AD9854_RST_Pin				GPIO_Pin_5
#define AD9854_SDO_Pin				GPIO_Pin_6


#define AD9854_CS_H				GPIO_SetBits(AD9854_CS_GPIO,AD9854_CS_Pin)
#define AD9854_CS_L     		GPIO_ResetBits(AD9854_CS_GPIO,AD9854_CS_Pin)
#define AD9854_SCLK_H			GPIO_SetBits(AD9854_SCLK_GPIO,AD9854_SCLK_Pin)
#define AD9854_SCLK_L			GPIO_ResetBits(AD9854_SCLK_GPIO,AD9854_SCLK_Pin)
#define AD9854_UPDATE_H			GPIO_SetBits(AD9854_UPDATE_GPIO,AD9854_UPDATE_Pin)
#define AD9854_UPDATE_L			GPIO_ResetBits(AD9854_UPDATE_GPIO,AD9854_UPDATE_Pin)
#define AD9854_SDIO_H			GPIO_SetBits(AD9854_SDIO_GPIO,AD9854_SDIO_Pin)
#define AD9854_SDIO_L			GPIO_ResetBits(AD9854_SDIO_GPIO,AD9854_SDIO_Pin)
#define AD9854_IO_RESET_H		GPIO_SetBits(AD9854_IO_RESET_GPIO,AD9854_IO_RESET_Pin)
#define AD9854_IO_RESET_L		GPIO_ResetBits(AD9854_IO_RESET_GPIO,AD9854_IO_RESET_Pin)
#define AD9854_RST_H				GPIO_SetBits(AD9854_RST_GPIO,AD9854_RST_Pin)
#define AD9854_RST_L				GPIO_ResetBits(AD9854_RST_GPIO,AD9854_RST_Pin)

//#define AD9854_SDO_H				GPIO_SetBits(AD9854_SDO_GPIO,AD9854_SDO_Pin)
//#define AD9854_SDO_L				GPIO_ResetBits(AD9854_SDO_GPIO,AD9854_SDO_Pin)

#define AD9854_SDO_INPUT		GPIO_ReadInputDataBit(AD9854_SDO_GPIO,AD9854_SDO_Pin)


//******************************************************************
//写       //寄存器功能   //寄存器串行地址 //寄存器大小
#define	  AD9854_PHA1       0x00      // 2 Bytes
#define	  AD9854_PHA2       0x01      // 2 Bytes
#define	  AD9854_FRE1       0x02      // 6 Bytes
#define	  AD9854_FRE2       0x03      // 6 Bytes
#define	  AD9854_DELTA      0x04      // 6 Bytes
#define	  AD9854_UPDATA     0x05      // 4 Bytes
#define	  AD9854_RAMP_CLK   0x06      // 3 Bytes
#define	  AD9854_CTR_REG    0x07      // 4 Bytes
#define	  AD9854_I_MUL      0x08      // 2 Bytes
#define	  AD9854_Q_MUL      0x09      // 2 Bytes
#define	  AD9854_SHAPED     0x0A      // 1 Bytes
#define	  AD9854_Q_DAC      0x0B      // 2 Bytes
static unsigned char addr[12]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};
static unsigned char addrRead[12]={0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B};
//******************************************************************
static unsigned char AMPI[2] = {0xff,0xff};
static unsigned char AMPQ[2] = {0xff,0xff};
//static unsigned char freq[6]={0xA,0xAA,0xAB,0x0,0x0,0x0};//利用计算器计算所需的精确频率
static unsigned char freqbg[6]={0x0,0xF2,0xBA,0x0,0x2,0x6D};//扫频起始频率1M
static unsigned char freqend[6]={0x25,0xED,0x9,0x0,0x0,0x0};
static unsigned char freqstep[6]={0x0,0x0,0x6,0x0,0x0,0x0};//步进频率 100k
static unsigned char freqstay[3]={0x09,0x27,0xc0};//驻足时间0.0025s,完成一次扫频（1~40M）1s
static unsigned char ctr[4]={0x00,0x49,0x00,0x01};//控制寄存器值，32位，PLL倍频9倍，单音模式，外部更新时钟，开启SINC
static unsigned char ctrsw[4]={0x00,0x49,0xa4,0x01};//控制寄存器值，32位，PLL倍频9倍，脉冲调频模式，外部更新时钟，开启SINC
//******************************************************************

#endif
