#include "stm32f4xx.h"
#include "..\TASK\GUITask.h"
#include "lcd.h"
#include "KeyBoard.h"
#include "ADS1118_SPI.h"
#include "AD9854.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"

/*
LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u8 mode)
LCD_ShowFloat(u16 x,u16 y,double num,u8 precision,u8 len,u8 size)
LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
*/

Message_t elem[StackDefaultSize];
HandleStack_t HandleStack = HandleStack_Default;

CoordinateState_e 		CoordinateState = CosCorrection;
static uint8_t CuttingOutFlag =0;
static uint8_t CosCorrectionFlag =0;
//CoordinateWorkState_e	CoordinateWorkState=CuttingLoading;

//KeyMessageDataProcess
FrequencyTester_t FrequencyTester = FrequencyTester_Default;
//Freq-Amp & Freq-Phase Response
FrequencyResponse_t FrequencyResponse = FrequencyResponse_Default;
/*
*************************************
*Name		: GUI Module
*Description:
*Arguments	:
*************************************
*/
void GUI_Init()
{
	LCD_Clear(BLACK);
	POINT_COLOR=BRED;
	HandleStack.push(&HandleStack,SweepFrequency);
	KeyMessageGUIProcess();
}

/*
******************************************
*Name 		: Stack Module
*Description:
*Arguments	:
*Notes		:
******************************************
*/
SubInterfaceState_e Stack_Pop(struct HandleStack_t* stack)
{
	SubInterfaceState_e temp=stack->_elem[stack->top].Lable;
	if(stack->size-1 <0)
		return None;
	stack->_elem[stack->top].Lable=None;
	stack->size--;
	stack->top--;
	return temp;
}

uint8_t Stack_Push(struct HandleStack_t* stack,SubInterfaceState_e data)
{
	if(stack->size+1 > StackDefaultSize)
		return 0;
	stack->top++;
	stack->size++;
	stack->_elem[stack->top].Lable=data;
	return 1;
}

/*
**************************************
*Name		: KeyResponse Module
*Description:
*Argument	:
*Note		:
**************************************
*/
extern uint8_t KeyValue;
void KeyProcess()
{
	switch(KeyValue)
	{
		case KEY_0:
		case KEY_1:
		case KEY_2:
		case KEY_3:
		case KEY_4:
		case KEY_5:
		case KEY_6:
		case KEY_7:
		case KEY_8:
		case KEY_9:
			Key0_9Process(); break;
		case KEY_A:
			KeyBackwardProcess(); break;
		case KEY_B:
			KeyUpProcess();break;
		case KEY_C:
			KeyDownProcess(); break;
		case KEY_D:
			KeyOKProcess();break;
		case 13:					// '*'
			CuttingOutFlag=0;
			CosCorrectionFlag=0;
			break;
		case 14:					// '#'
			break;
		default:break;
	}
		KeyMessageGUIProcess();
}

void KeyMessageGUIProcess()
{
	double Temp_Double=0;
	//if(!KeyChange_Flag)
	switch(HandleStack._elem[HandleStack.top].Lable)
	{
		case SweepFrequency:
		{
			LCD_Clear(BLACK);
			LCD_ShowString(30,10,900,16,48,(u8*)"Mode:      Single-Tone",0);
			LCD_ShowString(30,110,300,16,48,(u8*)"Frequency:",0);
			LCD_ShowString(30,210,300,16,48,(u8*)"Step:",0);
			
			LCD_ShowString(650,110,300,16,48,(u8*)"MHz",0);
			LCD_ShowString(650,210,300,16,48,(u8*)"kHz",0);
			
			LCD_ShowFloat(300,110,FrequencyTester.SetADS9854Freq,4,12,48);
			LCD_ShowFloat(300,210,FrequencyTester.SetSingleToneStep,4,12,48);
		}break;
		case AmplitudeFrequency:
		{
			LCD_Clear(BLACK);
			LCD_DrawCartesianCoordinate(50,140,"f/100kHz","20lg|A|/dB");
			if(CoordinateState == CuttingOut)
				LCD_ShowString(650,0,120,24,24,(u8*)"CuttingOut",0);
			else if (CoordinateState == CosCorrection)
				LCD_ShowString(650,0,160,24,24,(u8*)"CosCorrection",0);
			else if(CoordinateState == Normal)
				LCD_ShowString(650,0,120,24,24,(u8*)"Normal",0);
		}break;
		case PhaseFrequency:
		{
			LCD_Clear(BLACK);
			LCD_DrawCartesianCoordinate(50,240,"f/100kHz","Phase/degree");
			if(CoordinateState == CuttingOut)
				LCD_ShowString(650,0,120,24,24,(u8*)"CuttingOut",0);
			else if (CoordinateState == CosCorrection)
				LCD_ShowString(650,0,160,24,24,(u8*)"CosCorrection",0);
			else if(CoordinateState == Normal)
				LCD_ShowString(650,0,120,24,24,(u8*)"Normal",0);
		}break;
		case SingleToneMode:
			LCD_Clear(BLACK);
			LCD_DrawArrow(799-50,10,48,48,POINT_COLOR);
			LCD_ShowString(30,10,900,16,48,(u8*)"Mode:      Single-Tone",0);
			LCD_ShowString(30,110,300,16,48,(u8*)"Frequency:",0);
			LCD_ShowString(30,210,300,16,48,(u8*)"Step:",0);
		
			LCD_ShowString(650,110,300,16,48,(u8*)"MHz",0);
			LCD_ShowString(650,210,300,16,48,(u8*)"kHz",0);
		
			LCD_ShowFloat(300,110,FrequencyTester.SetADS9854Freq,4,12,48);
			LCD_ShowFloat(300,210,FrequencyTester.SetSingleToneStep,4,12,48);
		break;
		case SweepMode:
			LCD_Clear(BLACK);
			LCD_DrawArrow(799-50,10,48,48,POINT_COLOR);		
			LCD_ShowString(30,10,900,16,48,(u8*)"Mode:      Sweep-Frequency",0);
			LCD_ShowString(30,110,300,16,48,(u8*)"Min:",0);
			LCD_ShowString(30,210,300,16,48,(u8*)"Max:",0);
			LCD_ShowString(30,310,300,16,48,(u8*)"Step:",0);
		
			LCD_ShowString(650,110,300,16,48,(u8*)"MHz",0);
			LCD_ShowString(650,210,300,16,48,(u8*)"MHz",0);
			LCD_ShowString(650,310,300,16,48,(u8*)"kHz",0);
		
			LCD_ShowFloat(300,110,FrequencyTester.SetSweepMin,4,12,48);
			LCD_ShowFloat(300,210,FrequencyTester.SetSweepMax,4,12,48);
			LCD_ShowFloat(300,310,FrequencyTester.SetSweepStep,4,12,48);
		break;
		case SetFrequency:
				Temp_Double=input_double_s(300,110);
				if(Enter_flag==1)
				{	
					if(Temp_Double>40)		//Max: 40MHz
						Temp_Double=40;
					if(Temp_Double<1)		//Min: 1MHz
						Temp_Double=1;
					LCD_ShowFloat(300,110,Temp_Double,4,10,48);
					FrequencyTester.SetADS9854Freq=Temp_Double;
					Enter_flag=0;
				}
				LCD_Fill(799-50,10,799,410+48,BACK_COLOR);
				LCD_DrawArrow(799-48,110,48,48,POINT_COLOR);		
		break;
		case SetStep:
				LCD_Fill(799-50,10,799,410+48,BACK_COLOR);
				if(HandleStack._elem[HandleStack.top-1].Lable == SingleToneMode)
				{
					LCD_DrawArrow(799-48,210,48,48,POINT_COLOR);
					Temp_Double=input_double_s(300,210);
					if(Enter_flag==1)
					{	
						if(Temp_Double>FrequencyTester.SetADS9854Freq*kHz)
						{
							Temp_Double=FrequencyTester.SetADS9854Freq*kHz;		// SetFreq :MHz
						}
						else if(Temp_Double>20000)	// Max:20MHz
							Temp_Double=20000;
						LCD_ShowFloat(300,210,Temp_Double,4,10,48);
						FrequencyTester.SetSingleToneStep=Temp_Double;
						Enter_flag=0;
					}
				}
				else if(HandleStack._elem[HandleStack.top-1].Lable == SweepMode)
				{
					LCD_DrawArrow(799-48,310,48,48,POINT_COLOR);	
					Temp_Double=input_double_s(300,310);
					if(Enter_flag==1)
					{	
						if(Temp_Double>20000)							// Max:20MHz
							Temp_Double=20000;	
						if(Temp_Double<1)								// Min:1kHz
							Temp_Double=1;
						
						LCD_ShowFloat(300,310,Temp_Double,4,10,48);
						FrequencyTester.SetSweepStep=Temp_Double;						
						Enter_flag=0;
					}					
				}
		break;
		case SetRangeMin:
				Temp_Double=input_double_s(300,110);
				if(Enter_flag==1)
				{	
					if(Temp_Double>FrequencyTester.SetSweepMax)
						Temp_Double=FrequencyTester.SetSweepMax;
					
					LCD_ShowFloat(300,110,Temp_Double,4,10,48);
					FrequencyTester.SetSweepMin=Temp_Double;
					Enter_flag=0;
				}
			LCD_Fill(799-50,10,799,410+48,BACK_COLOR);
			LCD_DrawArrow(799-48,110,48,48,POINT_COLOR);		
		break;
		case SetRangeMax:
				Temp_Double=input_double_s(300,210);
				if(Enter_flag==1)
				{	
					if(Temp_Double>50)			// Max:50MHz
						Temp_Double=50;
					if(Temp_Double<FrequencyTester.SetSweepMin)
						Temp_Double=FrequencyTester.SetSweepMin;
					
					LCD_ShowFloat(300,210,Temp_Double,4,10,48);
					FrequencyTester.SetSweepMax=Temp_Double;
					Enter_flag=0;
				}			
			LCD_Fill(799-50,10,799,410+48,BACK_COLOR);
			LCD_DrawArrow(799-48,210,48,48,POINT_COLOR);				
		break;
	default:
		{
			LCD_ShowString(30,10,300,16,48,(u8*)"Frequency:",0);	// u16 x, u16 y,u16 width(字宽), u16 height(行距), u8 size(字体大小), u8 *p
			LCD_ShowString(30,110,300,16,48,(u8*)"Duty:",0);
			LCD_ShowString(30,210,300,16,48,(u8*)"Interval:",0);
			LCD_ShowString(30,310,300,16,48,(u8*)"Amplitude:",0);
	
			LCD_ShowString(650,110,300,16,48,(u8*)"%",0);
			LCD_ShowString(650,210,300,16,48,(u8*)"uS",0);
			LCD_ShowString(650,310,300,16,48,(u8*)"mV",0);
		}break;
	}
}

#define Origin_X 50
#define Origin_Y 140
void KeyMessageDataProcess(void)
{
	double BottomFreq=0;
	double Bottomdb=0;
	//static uint8_t CuttingOutFlag =0;
	uint16_t tempCoordinate[3]={0};
	float CntStep=0;
	uint16_t CntData=0;
	uint16_t DelayCoef=0;
	
	uint8_t BWFlag=0;
	switch(HandleStack._elem[HandleStack.top].Lable)
	{
		case SweepFrequency:
			break;
		case AmplitudeFrequency:
		case PhaseFrequency:
			if(CoordinateState == CosCorrection)
			{
				CntData=0;
				CntStep=0;			//MHz
				FrequencyTester.SweepInterval=(FrequencyTester.SetSweepMax-FrequencyTester.SetSweepMin)*1.0f/(FrequencyTester.SetSweepStep)*kHz;//MHz
				if(!CosCorrectionFlag)
				{
					LCD_ShowString(700,450,100,24,24,(u8*)"Loading...",0);
					for(CntStep=FrequencyTester.SetSweepMin;CntStep<FrequencyTester.SetSweepMax;CntStep=CntStep+FrequencyTester.SetSweepStep/kHz)
					{
						if(CoordinateState !=CosCorrection)
							goto CorrectionBreak;
						INTX_DISABLE();
						AD9854WriteFreqSingle(CntStep*MHz);
						//delay_us(20);								//MHz
						ReadADS1118MUX();																		// Read I & Q data
						FrequencyResponse.INoiseTest (&FrequencyResponse,CntData,ADS1118.AIN3);		// I:AIN3	Q:AIN2
						I_Noise2Coordinate(FrequencyResponse.I_Noise[CntData],&tempCoordinate[0]);
						if(CntData)
						{
							LCD_Fast_DrawPoint(Origin_X+CntData,tempCoordinate[0],WHITE);
						}
						CntData++;
						INTX_ENABLE();
					}
					CosCorrectionFlag=1;				
					if(CosCorrectionFlag)
						LCD_ShowString(700,450,100,24,24,(u8*)"Finished",0);					
				}
				CorrectionBreak: __NOP;
				CntData=0;				
			}
			if(CoordinateState == CuttingOut)
			{
				CntData=0;
				CntStep=0;			//MHz
				FrequencyTester.SweepInterval=(FrequencyTester.SetSweepMax-FrequencyTester.SetSweepMin)*1.0f/(FrequencyTester.SetSweepStep)*kHz;//MHz
				DelayCoef=1*MHz/FrequencyTester.SweepInterval;
				if(!CuttingOutFlag)
				{
					LCD_ShowString(700,450,100,24,24,(u8*)"Loading...",0);
					for(CntStep=FrequencyTester.SetSweepMin;CntStep<FrequencyTester.SetSweepMax;CntStep=CntStep+FrequencyTester.SetSweepStep/kHz)
					{
						if(CoordinateState !=CuttingOut)
							goto CuttingOutBreak;
						INTX_DISABLE();
						AD9854WriteFreqSingle(CntStep*MHz);
						//delay_us(50);								//MHz
						ReadADS1118MUX();																		// Read I & Q data
						FrequencyResponse.preTest (&FrequencyResponse,CntData,ADS1118.AIN3,ADS1118.AIN2);		// I:AIN3	Q:AIN2
						I_Noise2Coordinate(FrequencyResponse.I_Amp[CntData],&tempCoordinate[0]);
						I_Noise2Coordinate(ADS1118.AIN3,&tempCoordinate[1]);
						I_Noise2Coordinate(ADS1118.AIN2,&tempCoordinate[2]);
						if(CntData)
						{
							LCD_Fast_DrawPoint(Origin_X+CntData,tempCoordinate[0],POINT_COLOR);
							LCD_Fast_DrawPoint(Origin_X+CntData,tempCoordinate[1],BLUE);
							LCD_Fast_DrawPoint(Origin_X+CntData,tempCoordinate[2],GREEN);
						}

						//LCD_ShowFloat(650,48,FrequencyResponse.I_Amp[CntData],2,8,24);
						//LCD_ShowFloat(650,72,ADS1118.AIN2,4,8,24);
						CntData++;
						INTX_ENABLE();
					}
					CuttingOutFlag=1;				
					if(CuttingOutFlag)
						LCD_ShowString(700,450,100,24,24,(u8*)"Finished",0);					
				}
				CuttingOutBreak: __NOP;
				CntData=0;
			}
			
			else if(CoordinateState == Normal)
			{
				CntData=0;
				CntStep=0;
				FrequencyTester.SweepInterval=(FrequencyTester.SetSweepMax-FrequencyTester.SetSweepMin)*1.0f/(FrequencyTester.SetSweepStep)*kHz;//MHz
				DelayCoef=1*MHz/FrequencyTester.SweepInterval;
				if(HandleStack._elem[HandleStack.top].Lable == AmplitudeFrequency)
				{
					LCD_ShowString(650,24,240,24,24,(u8*)"A:",0);
					LCD_ShowString(650,48,240,24,24,(u8*)"f:",0);
						
					LCD_ShowString(650+120,24,24,24,24,(u8*)"dB",0);
					LCD_ShowString(650+108,48,36,24,24,(u8*)"MHz",0);
					LCD_ShowString(Origin_X-40,Origin_Y+75,40,24,24,(u8*)"-10",0);
					LCD_ShowString(Origin_X-40,Origin_Y+150,40,24,24,(u8*)"-20",0);
					LCD_ShowString(Origin_X-40,Origin_Y+225,40,24,24,(u8*)"-30",0);
					LCD_ShowString(Origin_X-40,Origin_Y+300,40,24,24,(u8*)"-40",0);
					
					LCD_ShowString(Origin_X+100-12,Origin_Y+2,40,24,24,(u8*)"10",0);
					LCD_ShowString(Origin_X+200-12,Origin_Y+2,40,24,24,(u8*)"20",0);
					LCD_ShowString(Origin_X+300-12,Origin_Y+2,40,24,24,(u8*)"30",0);
					LCD_ShowString(Origin_X+400-12,Origin_Y+2,40,24,24,(u8*)"40",0);
					
					for(CntStep=FrequencyTester.SetSweepMin;CntStep<FrequencyTester.SetSweepMax;CntStep=CntStep+FrequencyTester.SetSweepStep/kHz)
					{
						if(CoordinateState !=Normal || HandleStack._elem[HandleStack.top].Lable != AmplitudeFrequency)
							goto BreakFlag;
						INTX_DISABLE();
						AD9854WriteFreqSingle(CntStep*MHz);
						//delay_us(50);								//DelayCoef
						ReadADS1118MUX();																		// Read I & Q data
						FrequencyResponse.AmpFreq(&FrequencyResponse,CntData,ADS1118.AIN3, ADS1118.AIN2);		// I:AIN3	Q:AIN2
						FrequencyResponse.PhaseFreq(&FrequencyResponse,CntData, ADS1118.AIN3,ADS1118.AIN2);
					
						//Central Frequency
						if(FrequencyResponse.AmplitudeFreqData[FrequencyResponse.CentralCnt]<FrequencyResponse.AmplitudeFreqData[CntData])
						{
							FrequencyResponse.CentralFreq=CntStep;
							FrequencyResponse.CentralCnt=CntData;
						}
						//Bottom Frequency
						if(CntStep>20)
						{
							if(Bottomdb>FrequencyResponse.AmplitudeFreqData[CntData])
							{
								Bottomdb=FrequencyResponse.AmplitudeFreqData[CntData];
								BottomFreq=CntStep;
							}
						}
						//Bandwidth
						if(FrequencyResponse.AmplitudeFreqData[CntData] >-3.05f && FrequencyResponse.AmplitudeFreqData[CntData]< -2.95f)
						{
							if(!BWFlag)
							{
								FrequencyResponse.Left_3dB=CntStep;
								BWFlag=1;
							}
							else
								FrequencyResponse.Right_3dB=CntStep;							
						}
						//Draw
						LCD_ShowFloat(650+24,24,FrequencyResponse.AmplitudeFreqData[CntData],4,8,24);
						LCD_ShowFloat(650+24,48,CntStep,3,7,24);
						if(CntData)
							//LCD_DrawLine(Origin_X+50+CntData-1,FrequencyResponse.AmpCoordinate[CntData-1],Origin_X+50+CntData+1,FrequencyResponse.AmpCoordinate[CntData]);
						{	
							LCD_Fast_DrawPoint(Origin_X+CntData,FrequencyResponse.AmpCoordinate[CntData],POINT_COLOR);
							LCD_Fast_DrawPoint(Origin_X+CntData,240- 30*(ADS1118.AIN3-FrequencyResponse.I_Noise[CntData]),BLUE);
							LCD_Fast_DrawPoint(Origin_X+CntData,240- 30*(ADS1118.AIN2-FrequencyResponse.Q_Noise[CntData]),GREEN);
						}
						CntData++;
						INTX_ENABLE();			
					}
					LCD_Clear(BLACK);
					LCD_DrawCartesianCoordinate(Origin_X,Origin_Y,"f/100kHz","20lg|A|/dB");
					if(CoordinateState == CuttingOut)
						LCD_ShowString(650,0,120,24,24,(u8*)"CuttingOut",0);
					else if (CoordinateState == CosCorrection)
						LCD_ShowString(650,0,160,24,24,(u8*)"CosCorrection",0);
					else if(CoordinateState == Normal)
						LCD_ShowString(650,0,120,24,24,(u8*)"Normal",0);
				}
				else if(HandleStack._elem[HandleStack.top].Lable == PhaseFrequency)
				{
					CntData=0;
					CntStep=0;
					LCD_ShowString(650,24,240,24,24,(u8*)"p:",0);
					LCD_ShowString(650,48,240,24,24,(u8*)"f:",0);
					LCD_ShowString(650+108,48,36,24,24,(u8*)"MHz",0);
					
					LCD_ShowString(Origin_X-50,240-150,60,24,24,(u8*)"0.5Pi",0);
					LCD_ShowString(Origin_X-50,240+150,80,24,24,(u8*)"-0.5Pi",0);
					
					LCD_ShowString(Origin_X+100-12,240+2,40,24,24,(u8*)"10",0);
					LCD_ShowString(Origin_X+200-12,240+2,40,24,24,(u8*)"20",0);
					LCD_ShowString(Origin_X+300-12,240+2,40,24,24,(u8*)"30",0);
					LCD_ShowString(Origin_X+400-12,240+2,40,24,24,(u8*)"40",0);					
					//LCD_ShowFloat(650+24,72,FrequencyResponse.PhaseCoordinate[CntData],3,7,24);
					for(CntStep=FrequencyTester.SetSweepMin;CntStep<FrequencyTester.SetSweepMax;CntStep=CntStep+FrequencyTester.SetSweepStep/kHz)
					{
						if(CoordinateState !=Normal ||HandleStack._elem[HandleStack.top].Lable != PhaseFrequency)
							goto BreakFlag;
						INTX_DISABLE();
						AD9854WriteFreqSingle(CntStep*MHz);
						delay_us(50);
						ReadADS1118MUX();																		// Read I & Q data
						FrequencyResponse.AmpFreq(&FrequencyResponse,CntData,ADS1118.AIN3, ADS1118.AIN2);		// I:AIN3	Q:AIN2
						FrequencyResponse.PhaseFreq(&FrequencyResponse,CntData, ADS1118.AIN3,ADS1118.AIN2);
					
						if(FrequencyResponse.PhaseFreqData[CntData]<0.5f && FrequencyResponse.PhaseFreqData[CntData]>-0.5f)
						{
							FrequencyResponse.CentralFreq=CntStep;
							FrequencyResponse.CentralCnt=CntData;
						}
						//Bandwidth
						if(FrequencyResponse.AmplitudeFreqData[CntData] >-3.1f && FrequencyResponse.AmplitudeFreqData[CntData]< -2.9f)
						{
							if(!BWFlag)
							{
								FrequencyResponse.Left_3dB=CntStep;
								BWFlag=1;
							}
							else
								FrequencyResponse.Right_3dB=CntStep;							
						}
						//Draw 
						LCD_ShowFloat(650+24,24,FrequencyResponse.PhaseFreqData[CntData],4,8,24);
						LCD_ShowFloat(650+24,48,CntStep,4,7,24);
						if(CntData)
							//LCD_DrawLine(Origin_X+CntData-1,FrequencyResponse.PhaseCoordinate[CntData-1],Origin_X+CntData,FrequencyResponse.PhaseCoordinate[CntData]);
							LCD_Fast_DrawPoint(Origin_X+CntData,FrequencyResponse.PhaseCoordinate[CntData],RED);
						CntData++;
						INTX_ENABLE();					
					}
					LCD_Clear(BLACK);
					LCD_DrawCartesianCoordinate(50,240,"f/100kHz","Phase/degree");
					if(CoordinateState == CuttingOut)
						LCD_ShowString(650,0,120,24,24,(u8*)"CuttingOut",0);
					else if (CoordinateState == CosCorrection)
						LCD_ShowString(650,0,160,24,24,(u8*)"CosCorrection",0);
					else if(CoordinateState == Normal)
						LCD_ShowString(650,0,120,24,24,(u8*)"Normal",0);
				}
				LCD_ShowString(650,96,240,24,24,(u8*)"CentralFreq:",0);
				LCD_ShowString(650,144,240,24,24,(u8*)"BandWidth",0);
				FrequencyResponse.BandWidth=FrequencyResponse.Right_3dB-FrequencyResponse.Left_3dB;
				LCD_ShowString(650+108,120,36,24,24,(u8*)"MHz",0);
				LCD_ShowFloat(650,120,FrequencyResponse.CentralFreq,2,12,24);
				LCD_ShowString(650+108,168,36,24,24,(u8*)"MHz",0);
				LCD_ShowFloat(650,168,FrequencyResponse.BandWidth,2,12,24);
				//LCD_ShowFloat(650,192,FrequencyResponse.AmplitudeFreqData[FrequencyResponse.CentralCnt],2,12,24);
				//LCD_ShowFloat(650,192,BottomFreq,2,12,24);
				BreakFlag: __NOP;
			}
			break;
		case SingleToneMode:
		case SetFrequency:
		case SetStep:
			AD9854WriteFreqSingle(FrequencyTester.SetADS9854Freq*MHz);
			break;
		case SweepMode:
		case SetRangeMin:
		case SetRangeMax:	
			FrequencyTester.SweepInterval=(FrequencyTester.SetSweepMax-FrequencyTester.SetSweepMin)*1.0f/(FrequencyTester.SetSweepStep)*kHz;
			DelayCoef=1.0f*MHz/FrequencyTester.SweepInterval;
			for(CntStep=FrequencyTester.SetSweepMin;CntStep<FrequencyTester.SetSweepMax;CntStep=CntStep+FrequencyTester.SetSweepStep/kHz)
			{
				AD9854WriteFreqSingle(CntStep*MHz);
				delay_us(DelayCoef);
			}
		
			break;
		default:
		/*	ReadADS1118MUX();		
			LCD_ShowFloat(300,10,Freq,5,12,48);
			LCD_ShowFloat(300,110,Duty,4,12,48);
			LCD_ShowFloat(300,210,IntervalTime,4,12,48);
			LCD_ShowFloat(300,310,Amp,2,12,48);		
			LCD_ShowFloat(300,410,ADS1118.AIN0,5,12,48);*/
			break;
	}
}


/*
***********************************************
*Name		: Correction	Network
*Description: I= A^2/2 + D1(w)
			: Q= D2(w)
*Arguments	: I_Data = I
			: Q_Data = Q 
*Return		: None
*Note		:
***********************************************
*/
#define Pi 3.1415926f

void INoiseTest(struct FrequencyResponse_t* FreqRes,uint16_t pData,double I_Data)
{
	FreqRes->I_Noise[pData] = I_Data;
}

void preTest (struct FrequencyResponse_t* FreqRes,uint16_t pData,double I_Data,double Q_Data)
{
	FreqRes->I_Amp[pData]=I_Data-FreqRes->I_Noise[pData];					// A^2/2
	FreqRes->Q_Noise[pData]=Q_Data;
}


void AmpFreq(struct FrequencyResponse_t* FreqRes,uint16_t pData,double I, double Q)
{
	double slope = 150/20;		//300									// Range: -20~20dB
	double I_SampleData=(I-FreqRes->I_Noise[pData])/(FreqRes->I_Amp[pData]);	// H(w)cos(sita)
	double Q_SampleData=(Q-FreqRes->Q_Noise[pData])/(FreqRes->I_Amp[pData]);	// H(w)sin(sita)
	double Amplitude=sqrt(I_SampleData*I_SampleData+Q_SampleData*Q_SampleData);
	Amplitude=(Amplitude)/(2.0f-Amplitude);
	//double Amplitude=sqrt(I*I+Q*Q)/(FreqRes->I_Amp[pData]);
	FreqRes->AmplitudeFreqData[pData]=20*log10(Amplitude);			// I_Amp[pData]-> A^2/2
	FreqRes->AmpCoordinate[pData]=(u16)(Origin_Y-slope*FreqRes->AmplitudeFreqData[pData]);
}

void PhaseFreq(struct FrequencyResponse_t* FreqRes,uint16_t pData, double I,double Q)
{
	double slope = 300/Pi;	// Range: -0.5Pi~0.5Pi
	double I_SampleData=I-FreqRes->I_Noise[pData];
	double Q_SampleData=Q-FreqRes->Q_Noise[pData];
	//double I_SampleData=I;
	//double Q_SampleData=Q;
	FreqRes->PhaseFreqData[pData]= atan(Q_SampleData/I_SampleData);
	FreqRes->PhaseCoordinate[pData]=(u16)(240-slope*FreqRes->PhaseFreqData[pData]);	//Origin_Y-slope*FreqRes->PhaseFreqData[pData]
}

void I_Noise2Coordinate(double I_Noise,uint16_t* Coordinate)
{
	double slope= 300/20;
	double tempNoise = 20*log10(I_Noise);
	*Coordinate =(uint16_t)(240- slope*tempNoise);//Origin_Y
}

/*
 ****************************************
 *Name		 : Key Response
 *Description:
 *Arguments  :
 *Notes		 :
 ****************************************
 */

void Key0_9Process()
{
			/*Temp_Double=input_double_s(0,0);
		if(Enter_flag==1)
		{	
			LCD_TData(3,0,Temp_Double);
			Enter_flag=0;
		}*/
}

void KeyUpProcess()
{
	switch(HandleStack._elem[HandleStack.top].Lable)
	{
		case MainMenu:
			break;
		case SweepFrequency:
			if(HandleStack.size == 1)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,PhaseFrequency);
			}
			break;
		case AmplitudeFrequency:
			if(HandleStack.size == 1)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SweepFrequency);
			}
			break;
		case PhaseFrequency:
			if(HandleStack.size == 1)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,AmplitudeFrequency);			
			}
			break;
		case SingleToneMode:
			if(HandleStack.size == 2)
			{
				//HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetStep);						
			}
			break;
		case SweepMode:
			if(HandleStack.size == 2)
			{
				//HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetStep);	
			}
			break;
		case SetFrequency:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				//HandleStack.push(&HandleStack,SingleToneMode);			
			}
			break;
		case SetStep:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				if(HandleStack._elem[HandleStack.top].Lable == SingleToneMode)
					HandleStack.push(&HandleStack,SetFrequency);	
				else
					HandleStack.push(&HandleStack,SetRangeMax);
			}
			break;
		case SetRangeMin:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				//HandleStack.push(&HandleStack,SweepMode);
			}
			break;
		case SetRangeMax:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetRangeMin);					
			}
			break;
		case None:
			break;
		default: break;
	}
}


void KeyDownProcess()
{
	switch(HandleStack._elem[HandleStack.top].Lable)
	{
		case MainMenu:
			break;
		case SweepFrequency:
			if(HandleStack.size ==1)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,AmplitudeFrequency);
			}
			break;
		case AmplitudeFrequency:
			if(HandleStack.size ==1)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,PhaseFrequency);
			}
			break;
		case PhaseFrequency:
			if(HandleStack.size ==1)
			{			
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SweepFrequency);
			}
			break;
		case SingleToneMode:
			if(HandleStack.size == 2)
			{
			//	HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetFrequency);
			}
			break;
		case SweepMode:
			if(HandleStack.size == 2)
			{
			//	HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetRangeMin);	
			}
			break;
		case SetFrequency:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetStep);	
			}
			break;
		case SetStep:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
			//	HandleStack.push(&HandleStack,SetFrequency);			
			}
			break;
		case SetRangeMin:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetRangeMax);	
			}	
			break;
		case SetRangeMax:
			if(HandleStack.size == 3)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SetStep);			
			}
			break;
		case None:
			break;
		default: break;
	}
}


void KeyOKProcess()
{
	switch(HandleStack._elem[HandleStack.top].Lable)
	{
		case MainMenu:
			break;
		case SweepFrequency:
			if(HandleStack.size == 1)
			{
				HandleStack.push(&HandleStack,SingleToneMode);		
			}
			break;
		case AmplitudeFrequency:
		case PhaseFrequency:
			if(CoordinateState == CosCorrection)
				CoordinateState = CuttingOut;
			else if(CoordinateState == CuttingOut)
				CoordinateState = Normal;
			else if (CoordinateState == Normal)
				CoordinateState = CosCorrection;
			break;
		case SingleToneMode:
			if(HandleStack.size == 2)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SweepMode);							
			}
			break;
		case SweepMode:
			if(HandleStack.size == 2)
			{
				HandleStack.pop(&HandleStack);
				HandleStack.push(&HandleStack,SingleToneMode);						
			}				
			break;
		case SetFrequency:			
			break;
		case SetStep:					
			break;
		case SetRangeMin:
			break;
		case SetRangeMax:
			break;
		case None:
			break;
		default: break;
	}
	
}

void KeyBackwardProcess()
{
	switch(HandleStack._elem[HandleStack.top].Lable)
	{
		case MainMenu:
			break;
		case SweepFrequency:
			break;
		case AmplitudeFrequency:
			break;
		case PhaseFrequency:
			break;
		case SingleToneMode:
			HandleStack.pop(&HandleStack);		
			break;
		case SweepMode:
			HandleStack.pop(&HandleStack);				
			break;
		case SetFrequency:	
		//	HandleStack.pop(&HandleStack);					
			break;
		case SetStep:	
		//	HandleStack.pop(&HandleStack);					
			break;
		case SetRangeMin:
		//	HandleStack.pop(&HandleStack);		
			break;
		case SetRangeMax:
		//	HandleStack.pop(&HandleStack);		
			break;
		case None:
			break;
		default: break;
	}
	
}


