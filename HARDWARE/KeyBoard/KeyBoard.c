#include "KeyBoard.h"
#include "sys.h"
#include "delay.h"
#include "LCD12864.h"
#include "lcd.h"
#include "..\TASK\GUITask.h"

#define LCD12864 0
#define TFTLCD 1			// 1:On  0: Off
#define TFTFontSize 48


const char array_CHAR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','*','#','D'};
int Enter_flag;
KeyBoradCode KBCode;
uint8_t KeyValue;
uint8_t KeyChange_Flag=0;
//中断服务程序
void EXTI9_5_IRQHandler(void)
{
//	double Temp_Double;
	delay_us(200);
	if(EXTI_GetITStatus(EXTI_Line8)!=RESET)//判断某个线上的中断是否发生
	{
		KeyValue=ReadKeyBoard();
		KeyProcess();
		/*Temp_Double=input_double_s(0,0);
		if(Enter_flag==1)
		{	
			LCD_TData(3,0,Temp_Double);
			Enter_flag=0;
		}*/
		EXTI_ClearITPendingBit(EXTI_Line8); //清除 LINE 上的中断标志位
	}
	else;
}

//接口函数
int input_int(int row,int col)
{
//	const char array_CHAR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','*','#','D'};
	static int temp_int_s=0;
	int temp_int=0;
	char key_return_c=0;
	static int i;
//	int j=0;
	static unsigned char CHAR_temp[17];
	ROW_H_COL_L();
	#if LCD12864
	LCD_TLine(row,col,(unsigned char*)"                ");//清除所要输入的LCD行
	#endif
	#if TFTLCD
	LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"          ",0);
	#endif
	while(1)
	{
		key_return_c=ReadKeyBoard();
		if(key_return_c<10)
		{
			CHAR_temp[i]=array_CHAR[key_return_c];
			CHAR_temp[i+1]='\0';
			#if LCD12864
			LCD_TLine(row,col,CHAR_temp);
			#endif
			#if TFTLCD
			LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
			#endif
			temp_int_s=temp_int_s*10+key_return_c;
			i=i+1;
		}
		else if(key_return_c==13)//按下enter键
		{
			break;//直接跳出循环，待函数返回输入的值
		}
		else if(key_return_c==10)//按下A（退格）键
		{
			if(i>0)
			{
				i=i-1;
				CHAR_temp[i]='\0';
				#if LCD12864
					LCD_TLine(row,col,(unsigned char*)"                ");//16个空格
					LCD_TLine(row,col,CHAR_temp);
				#endif
				#if TFTLCD
					LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"           ",0);
					LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
				#endif
				temp_int_s=temp_int_s/10;
			}
		}
		else if(key_return_c==14)//按下‘#’（小数点）键，由于此函数为正数输入函数,故不作处理
		{
//			Decimal_point=i;
			CHAR_temp[i]='.';
			CHAR_temp[i+1]='\0';
			#if LCD12864
				LCD_TLine(row,col,CHAR_temp);
			#endif 
			#if TFTLCD
				LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
			#endif
			break;
		}
			//LCD_TData(row+1,col,(double)temp_int_s);
	}
	i=0;
	temp_int=temp_int_s;
	temp_int_s=0;
	return temp_int;
}

//接口函数；类似于阻塞性输入，该函数适合应用于有强烈时间先后顺序的地方
double input_double(int row,int col)
{
	double temp_double=0;
	char key_return_c=0;
	static int i;
	static unsigned char CHAR_temp[17];
	ROW_H_COL_L();
	#if LCD12864
	LCD_TLine(row,col,(unsigned char*)"                ");//清除所要输入的LCD行
	#endif
	#if TFTLCD
	LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"             ",0);
	#endif
	while(1)
	{
		key_return_c=ReadKeyBoard();
		if(key_return_c<10)
		{
			CHAR_temp[i]=array_CHAR[key_return_c];
			CHAR_temp[i+1]='\0';
			#if LCD12864
			LCD_TLine(row,col,CHAR_temp);
			#endif
			#if TFTLCD
			LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
			#endif
//			temp_int_s=temp_int_s*10+key_return_c;
			i=i+1;
		}
		else if(key_return_c==13)//按下enter键
		{
			break;//直接跳出循环，待函数返回输入的值
		}
		else if(key_return_c==10)//按下A（退格）键
		{
			if(i>0)
			{
				i=i-1;
				CHAR_temp[i]='\0';
				#if LCD12864
				LCD_TLine(row,col,(unsigned char*)"                ");//16个空格
				LCD_TLine(row,col,CHAR_temp);
				#endif
				#if TFTLCD
				LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ",0);
				LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
				#endif
			}
		}
		else if(key_return_c==14)//按下‘#’（小数点）键
		{
			CHAR_temp[i]='.';
			CHAR_temp[i+1]='\0';
			#if LCD12864
			LCD_TLine(row,col,CHAR_temp);
			#endif
			#if TFTLCD
			LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
			#endif
			i=i+1;
		}
		else if(key_return_c==11)//'B'
		{

		}
		else if(key_return_c==13)//'C'
		{

		}
		else 										//'D';作为清屏键
		{
			return 0.0;
		}
	}
	temp_double=Char_2_Double(CHAR_temp);
	i=0;
	return temp_double;
}

//非阻塞性输入函数；接口函数
//本函数需要定义一个全局的return_flag
double input_double_s(int row,int col)
{
	double temp_double=0;
	char key_return_c=0;
	static int i;
	static unsigned char CHAR_temp[17];
	ROW_H_COL_L();
//	key_return_c=ReadKeyBoard();
	key_return_c=KeyValue;
	if(key_return_c<10)
	{
		CHAR_temp[i]=array_CHAR[key_return_c];
		CHAR_temp[i+1]='\0';
		#if LCD12864
		LCD_TLine(row,col,(unsigned char*)"                ");
		LCD_TLine(row,col,CHAR_temp);
		#endif
		#if TFTLCD
		LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ",0);
		LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
		#endif
		i=i+1;
	}
	else if(key_return_c==13)//按下enter键
	{
		Enter_flag=1;
//		LCD_TLine(row,col,CHAR_temp);
	}
	else if(key_return_c==10)//按下A（退格）键
	{
		if(i>0)
		{
			i=i-1;
			CHAR_temp[i]='\0';
			#if LCD12864
			LCD_TLine(row,col,(unsigned char*)"                ");//16个空格
			LCD_TLine(row,col,CHAR_temp);
			#endif
			#if TFTLCD
			LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ",0);		
			LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);			
			#endif
		}
	}
	else if(key_return_c==14)//按下‘#’（小数点）键
	{
		CHAR_temp[i]='.';
		CHAR_temp[i+1]='\0';
		#if LCD12864
		LCD_TLine(row,col,(unsigned char*)"                ");
		LCD_TLine(row,col,CHAR_temp);
		#endif
		#if TFTLCD
		LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ",0);		
		LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);	
		#endif
		i=i+1;
	}
	else if(key_return_c==11)//'B'
	{
//		LCD_TLine(row,col,CHAR_temp);
	}
	else if(key_return_c==13)//'C'
	{
//		LCD_TLine(row,col,CHAR_temp);
	}
	else 										//'D'
	{
//		LCD_TLine(row,col,CHAR_temp);
	}
	if(Enter_flag==1)
	{
		i=0;
		temp_double=Char_2_Double(CHAR_temp);

	}
	return temp_double;
}

//将一个数子数组转化为double类型数据；非接口函数
double Char_2_Double(unsigned char *CHAR_temp)
{
	double temp_double=0;
	long long int temp_=0;
	int i=0,j=0;
	for(j=0,temp_=0;j<17&&CHAR_temp[j]!='\0';j=j+1)
	{
		if(CHAR_temp[j]!='.')
		{
			temp_=temp_*10+(CHAR_temp[j]-48);
		}
	}
	temp_double=temp_;
	i=0;
	while(CHAR_temp[i])
	{
		if(CHAR_temp[i]=='.')
		{
			i++;
			while(CHAR_temp[i])
			{
				temp_double=temp_double/10;
				i++;
			}
			break;
		}
		i++;
	}
	return temp_double;
}

char ReadKeyBoard(void)
{
	char return_temp;
	int nTemp=0;
	if(ROW0==0||ROW1==0||ROW2==0||ROW3==0)
	{	
		delay_us(1000);
		if(ROW0==0||ROW1==0||ROW2==0||ROW3==0)
		{		
			nTemp =TestRow();
			ROW_L_COL_H();
			ROW_L_COL_H();
			delay_us(100);
			nTemp|=TestCol();
			switch(nTemp)
			{
			case 0x77: return_temp= 1;break;//'1'
			case 0x7b: return_temp= 2;break;//'2'
			case 0x7d: return_temp= 3;break;//'3'
			case 0x7e: return_temp=	10;break;//'mode'
			case 0xb7: return_temp= 4;break;//'4'
			case 0xbb: return_temp= 5;break;//'5'
			case 0xbd: return_temp= 6;break;//'6'
			case 0xbe: return_temp= 11;break;//'B'
			case 0xd7: return_temp= 7;break;//'7'
			case 0xdb: return_temp= 8;break;//'8'
			case 0xdd: return_temp= 9;break;//'9'
			case 0xde: return_temp= 12;break;//'C'
			case 0xe7: return_temp= 13;break;//'*'
			case 0xeb: return_temp= 0;break;//'0'
			case 0xed: return_temp= 14;break;//'#'
			case 0xee: return_temp= 15;break;//'D'
			default:break;
			}
		while(COL0==0||COL1==0||COL2==0||COL3==0);
		delay_us(5);
		while(COL0==0||COL1==0||COL2==0||COL3==0);
		}
	}
	ROW_H_COL_L();
	return return_temp;
}

int TestRow(void)
{
	int nTemp=0;
	if(ROW0==0)		
		nTemp=KBCode.Row0Val;		//0x70;		
	else if(ROW1==0) 		
    	nTemp=KBCode.Row1Val;		//0xb0;		
	else if(ROW2==0)			
    	nTemp=KBCode.Row2Val;		//0xd0;		
	else if(ROW3==0)			
    	nTemp=KBCode.Row3Val;		//0xe0;			
    else 
		nTemp=0x0f;		//0xf0;

	return nTemp;
}

int TestCol(void)
{
	int nTemp=0;
	if(COL0==0)	
		nTemp=KBCode.Col0Val;		//0x07;

	else if(COL1==0) 		
    	nTemp=KBCode.Col1Val;		//0x0b;		
	else if(COL2==0)			
    	nTemp=KBCode.Col2Val;		//0x0d;		
	else if(COL3==0)		
		nTemp=KBCode.Col3Val;		//0x0e;
    else 
		nTemp=0x00;
	return nTemp;
}



void Init_KeyBoard_Port(void)
{
	GPIO_InitTypeDef key_interrupt;
	InitRowColValue(1);
	ENABLE_KeyBoard_CLK();
	key_interrupt.GPIO_Pin=KEY_INTERRUPT_GPIO_BIT;
	key_interrupt.GPIO_Mode=GPIO_Mode_IN;
	key_interrupt.GPIO_Speed=GPIO_Speed_100MHz;
	key_interrupt.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(KEY_INTERRUPT_GPIO,&key_interrupt);
	KeyBoard_Interrupt_Init();
	ROW_H_COL_L();
}

void KeyBoard_Interrupt_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);//PA5 连接到中断线5
	
	/* 配置EXTI_Line5 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;//LINE5
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断8
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置	
}

void ROW_H_COL_L(void)
{
	ROW_IN_PULL_UP();
	COL_OUT_L();
}

void ROW_L_COL_H()
{
	COL_IN_PULL_UP();
	ROW_OUT_L();
}

void ENABLE_KeyBoard_CLK(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}


void ROW_IN_PULL_UP(void)
{
	int i=0;
	GPIO_InitTypeDef ROW_GPIO[4];
	//init row
	for(i=0;i<4;i++)
	{
		ROW_GPIO[i].GPIO_Mode=GPIO_Mode_IN;
		ROW_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
		ROW_GPIO[i].GPIO_PuPd=GPIO_PuPd_UP;		
	}
	ROW_GPIO[0].GPIO_Pin=ROW0_GPIO_BIT;
	ROW_GPIO[1].GPIO_Pin=ROW1_GPIO_BIT;
	ROW_GPIO[2].GPIO_Pin=ROW2_GPIO_BIT;
	ROW_GPIO[3].GPIO_Pin=ROW3_GPIO_BIT;
	GPIO_Init(ROW0_GPIO, &ROW_GPIO[0]);
	GPIO_Init(ROW1_GPIO, &ROW_GPIO[1]);
	GPIO_Init(ROW2_GPIO, &ROW_GPIO[2]);
	GPIO_Init(ROW3_GPIO, &ROW_GPIO[3]);	
}


void ROW_OUT_L(void)
{
		int i=0;
	GPIO_InitTypeDef ROW_GPIO[4];
	//init row
	for(i=0;i<4;i++)
	{
		ROW_GPIO[i].GPIO_Mode=GPIO_Mode_OUT;
		ROW_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
		ROW_GPIO[i].GPIO_OType = GPIO_OType_PP;
		ROW_GPIO[i].GPIO_PuPd=GPIO_PuPd_DOWN;		
	}
	ROW_GPIO[0].GPIO_Pin=ROW0_GPIO_BIT;
	ROW_GPIO[1].GPIO_Pin=ROW1_GPIO_BIT;
	ROW_GPIO[2].GPIO_Pin=ROW2_GPIO_BIT;
	ROW_GPIO[3].GPIO_Pin=ROW3_GPIO_BIT;
	GPIO_Init(ROW0_GPIO, &ROW_GPIO[0]);
	GPIO_Init(ROW1_GPIO, &ROW_GPIO[1]);
	GPIO_Init(ROW2_GPIO, &ROW_GPIO[2]);
	GPIO_Init(ROW3_GPIO, &ROW_GPIO[3]);	
	ROW_L;
}


void COL_OUT_L(void)
{
	int i=0;
	GPIO_InitTypeDef COL_GPIO[4];
		//init col
	for(i=0;i<4;i++)
	{
		COL_GPIO[i].GPIO_Mode=GPIO_Mode_OUT;
		COL_GPIO[i].GPIO_OType = GPIO_OType_PP;
		COL_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
		COL_GPIO[i].GPIO_PuPd=GPIO_PuPd_DOWN;
	}
	COL_GPIO[0].GPIO_Pin=COL0_GPIO_BIT;
	COL_GPIO[1].GPIO_Pin=COL1_GPIO_BIT;
	COL_GPIO[2].GPIO_Pin=COL2_GPIO_BIT;
	COL_GPIO[3].GPIO_Pin=COL3_GPIO_BIT;
	GPIO_Init(COL0_GPIO, &COL_GPIO[0]);	
	GPIO_Init(COL1_GPIO, &COL_GPIO[1]);
	GPIO_Init(COL2_GPIO, &COL_GPIO[2]);
	GPIO_Init(COL3_GPIO, &COL_GPIO[3]);
	COL_L;
}


void COL_IN_PULL_UP(void)
{
	int i=0;
	GPIO_InitTypeDef COL_GPIO[4];
	//init col
	for(i=0;i<4;i++)
	{
		COL_GPIO[i].GPIO_Mode=GPIO_Mode_IN;
//		COL_GPIO[i].GPIO_OType = GPIO_OType_PP;
		COL_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
		COL_GPIO[i].GPIO_PuPd=GPIO_PuPd_UP;
	}
	COL_GPIO[0].GPIO_Pin=COL0_GPIO_BIT;
	COL_GPIO[1].GPIO_Pin=COL1_GPIO_BIT;
	COL_GPIO[2].GPIO_Pin=COL2_GPIO_BIT;
	COL_GPIO[3].GPIO_Pin=COL3_GPIO_BIT;
	GPIO_Init(COL0_GPIO, &COL_GPIO[0]);	
	GPIO_Init(COL1_GPIO, &COL_GPIO[1]);
	GPIO_Init(COL2_GPIO, &COL_GPIO[2]);
	GPIO_Init(COL3_GPIO, &COL_GPIO[3]);
}

void InitRowColValue(u8 dir)
{
	if(dir==KEYBOARD_MIRROR)
	{
		KBCode.Col0Val=0x70;
		KBCode.Col1Val=0xb0;
		KBCode.Col2Val=0xd0;
		KBCode.Col3Val=0xe0;
		KBCode.Row0Val=0x07;
		KBCode.Row1Val=0x0b;
		KBCode.Row2Val=0x0d;
		KBCode.Row3Val=0x0e;
	}
	else if(dir==KEYBOARD_REAL)
	{
		KBCode.Col0Val=0x07;
		KBCode.Col1Val=0x0b;
		KBCode.Col2Val=0x0d;
		KBCode.Col3Val=0x0e;
		KBCode.Row0Val=0x70;
		KBCode.Row1Val=0xb0;
		KBCode.Row2Val=0xd0;
		KBCode.Row3Val=0xe0;		
	}
}

/*
int TestRow(void)
{
	int nTemp=0;
	if(COL0==0)		
		nTemp=0x70;		
	else if(COL1==0) 		
    	nTemp=0xb0;		
	else if(COL2==0)			
    	nTemp=0xd0;		
	else if(COL3==0)			
    	nTemp=0xe0;			
    else 
		nTemp=0xf0;

	return nTemp;
}*/
/*
int TestCol(void)
{
	int nTemp=0;
	if(ROW0==0)	
		nTemp=0x07;

	else if(ROW1==0) 		
    	nTemp=0x0b;		
	else if(ROW2==0)			
    	nTemp=0x0d;		
	else if(ROW3==0)		
		nTemp=0x0e;
    else 
		nTemp=0x00;
	return nTemp;
}*/

