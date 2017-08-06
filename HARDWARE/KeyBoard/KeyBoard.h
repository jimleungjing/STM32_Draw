#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include "stm32f4xx.h"

#define KEYBOARD_REAL	0
#define KEYBOARD_MIRROR	1

#define 	KEY_INTERRUPT_GPIO			GPIOB
#define 	KEY_INTERRUPT_GPIO_BIT		GPIO_Pin_8

#define 	ROW0_GPIO	GPIOB
#define	ROW1_GPIO	GPIOB
#define 	ROW2_GPIO	GPIOB
#define	ROW3_GPIO	GPIOB

#define 	COL0_GPIO	GPIOB
#define 	COL1_GPIO	GPIOG
#define 	COL2_GPIO	GPIOG
#define 	COL3_GPIO	GPIOG

#define 	ROW0_GPIO_BIT	GPIO_Pin_7
#define	ROW1_GPIO_BIT	GPIO_Pin_6
#define 	ROW2_GPIO_BIT	GPIO_Pin_5
#define	ROW3_GPIO_BIT	GPIO_Pin_4

#define 	COL0_GPIO_BIT	GPIO_Pin_3
#define 	COL1_GPIO_BIT	GPIO_Pin_15
#define 	COL2_GPIO_BIT	GPIO_Pin_14
#define 	COL3_GPIO_BIT	GPIO_Pin_13

#define	RCC_ROW0_GPIO	RCC_AHB1Periph_GPIOB
#define	RCC_ROW1_GPIO	RCC_AHB1Periph_GPIOB
#define	RCC_ROW2_GPIO	RCC_AHB1Periph_GPIOB
#define	RCC_ROW3_GPIO	RCC_AHB1Periph_GPIOB

#define RCC_COL0_GPIO	RCC_AHB1Periph_GPIOB
#define RCC_COL1_GPIO	RCC_AHB1Periph_GPIOG
#define RCC_COL2_GPIO	RCC_AHB1Periph_GPIOG
#define RCC_COL3_GPIO	RCC_AHB1Periph_GPIOG

#define ROW_H	{ROW0_H;ROW1_H;ROW2_H;ROW3_H;}
#define ROW_L	{ROW0_L;ROW1_L;ROW2_L;ROW3_L;}

#define COL_H	{COL0_H;COL1_H;COL2_H;COL3_H;}
#define COL_L	{COL0_L;COL1_L;COL2_L;COL3_L;}

#define ROW0_H 	GPIO_SetBits(ROW0_GPIO,ROW0_GPIO_BIT)
#define ROW1_H 	GPIO_SetBits(ROW1_GPIO,ROW1_GPIO_BIT)
#define ROW2_H 	GPIO_SetBits(ROW2_GPIO,ROW2_GPIO_BIT)
#define ROW3_H 	GPIO_SetBits(ROW3_GPIO,ROW3_GPIO_BIT)

#define ROW0_L 	GPIO_ResetBits(ROW0_GPIO,ROW0_GPIO_BIT)
#define ROW1_L 	GPIO_ResetBits(ROW1_GPIO,ROW1_GPIO_BIT)
#define ROW2_L 	GPIO_ResetBits(ROW2_GPIO,ROW2_GPIO_BIT)
#define ROW3_L 	GPIO_ResetBits(ROW3_GPIO,ROW3_GPIO_BIT)

#define COL0_H	GPIO_SetBits(COL0_GPIO,COL0_GPIO_BIT)
#define COL1_H	GPIO_SetBits(COL1_GPIO,COL1_GPIO_BIT)
#define COL2_H	GPIO_SetBits(COL2_GPIO,COL2_GPIO_BIT)
#define COL3_H	GPIO_SetBits(COL3_GPIO,COL3_GPIO_BIT)

#define COL0_L	GPIO_ResetBits(COL0_GPIO,COL0_GPIO_BIT)
#define COL1_L	GPIO_ResetBits(COL1_GPIO,COL1_GPIO_BIT)
#define COL2_L	GPIO_ResetBits(COL2_GPIO,COL2_GPIO_BIT)
#define COL3_L	GPIO_ResetBits(COL3_GPIO,COL3_GPIO_BIT)

#define ROW0	GPIO_ReadInputDataBit(ROW0_GPIO,ROW0_GPIO_BIT) 
#define ROW1	GPIO_ReadInputDataBit(ROW1_GPIO,ROW1_GPIO_BIT) 
#define ROW2	GPIO_ReadInputDataBit(ROW2_GPIO,ROW2_GPIO_BIT) 
#define ROW3	GPIO_ReadInputDataBit(ROW3_GPIO,ROW3_GPIO_BIT) 

#define COL0	GPIO_ReadInputDataBit(COL0_GPIO,COL0_GPIO_BIT)
#define COL1	GPIO_ReadInputDataBit(COL1_GPIO,COL1_GPIO_BIT)
#define COL2	GPIO_ReadInputDataBit(COL2_GPIO,COL2_GPIO_BIT)
#define COL3	GPIO_ReadInputDataBit(COL3_GPIO,COL3_GPIO_BIT)

typedef struct
{
	u8 Row0Val;
	u8 Row1Val;
	u8 Row2Val;
	u8 Row3Val;
	u8 Col0Val;
	u8 Col1Val;
	u8 Col2Val;
	u8 Col3Val;
}KeyBoradCode;

extern KeyBoradCode KBCode;

extern uint8_t KeyValue;
extern uint8_t KeyChange_Flag;
extern int Enter_flag;

int input_int(int row,int col);
double input_double(int row,int col);
double input_double_s(int row,int col);
double Char_2_Double(unsigned char *CHAR_temp);

char ReadKeyBoard(void);
int TestRow(void);
int TestCol(void);

void Init_KeyBoard_Port(void);
void KeyBoard_Interrupt_Init(void);

void ROW_H_COL_L(void);
void ROW_L_COL_H(void);

void ENABLE_KeyBoard_CLK(void);
void ROW_IN_PULL_UP(void);
void ROW_OUT_L(void);
void COL_OUT_L(void);
void COL_IN_PULL_UP(void);

void InitRowColValue(u8 dir);

#endif
