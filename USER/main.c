#include "main.h"
#include "stm32f4xx.h"
#include "ADS1118_SPI.h"

#include "KeyBoard.h"
#include "AD9854.h"
#include "..\TASK\GUITask.h"

CoordinateSystem_t Coordinates=CoordinateSystem_Default;

int main(void)
{
/*	double i=1;
	u32 ReadData=0;
*/	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);		  //初始化延时函数
	usart3_init(9600);
/*	
	Init_KeyBoard_Port();
	KeyBoard_Interrupt_Init();
*/	
	TFTLCD_Init();
//	GUI_Init();
	
	FPGASPIPortInit();
	ADS1118_SPI_port_init();
	AD9854Init();

	LCD_Clear(BLACK);
	POINT_COLOR=BRED;

	Coordinates.xlinspace(&Coordinates,0,40,0.1);
	Coordinates.ylinspace(&Coordinates,-40,20,0.1);
	Coordinates.axis(&Coordinates,100,700,100,400);
//	Coordinates.CartesianCoordinate(&Coordinates,150,200,"x","y");
	Coordinates.plot(&Coordinates,"x","y");
	Coordinates.grid(&Coordinates,GRAY);	
	LCD_Set_Window(700,350,100,100);
//	Coordinates.legend(&Coordinates,"cos");


	while(1)
	{ 
	//	LCD_ShowChar(700,420,HandleStack._elem[HandleStack.top].Lable+'0',24,0);
	//	LCD_ShowChar(750,450,HandleStack.size+'0',24,0);
	//	KeyMessageDataProcess();
	//	delay_ms(100);
	}
}

/*
		ReadADS1118MUX();
		SPIWriteRead32Bits(DAC904Addr);
		SPIWriteRead32Bits(0x80000000);						//3fff
		LCD_ShowFloat(300,10,ADS1118.AIN0,5,12,48);
		LCD_ShowFloat(300,110,ADS1118.AIN1,5,12,48);
		LCD_ShowFloat(300,210,ADS1118.AIN2,5,12,48);
		LCD_ShowFloat(300,310,ADS1118.AIN3,5,12,48);
*/
