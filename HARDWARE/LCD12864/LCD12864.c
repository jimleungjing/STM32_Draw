#include"LCD12864.h"
#include"sys.h"
#include"stm32f4xx_gpio.h"
#include"delay.h"

#define LCD_RCC		RCC_AHB1Periph_GPIOC

void LCD12864_PORT_init(void)
{
	int i=0;
	GPIO_InitTypeDef  LCD_GPIO[3];
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		
	LCD_GPIO[0].GPIO_Pin=LCD_CS_BIT;
	LCD_GPIO[0].GPIO_Mode=GPIO_Mode_OUT;
	LCD_GPIO[0].GPIO_OType=GPIO_OType_PP;
	LCD_GPIO[0].GPIO_Speed=GPIO_Speed_100MHz;
	LCD_GPIO[0].GPIO_PuPd = GPIO_PuPd_UP;
	
	LCD_GPIO[1].GPIO_Pin=LCD_SID_BIT;
	LCD_GPIO[1].GPIO_Mode=GPIO_Mode_OUT;
	LCD_GPIO[1].GPIO_OType=GPIO_OType_PP;
	LCD_GPIO[1].GPIO_Speed=GPIO_Speed_100MHz;
	LCD_GPIO[1].GPIO_PuPd = GPIO_PuPd_UP;
	
	LCD_GPIO[2].GPIO_Pin=LCD_SCLK_BIT;
	LCD_GPIO[2].GPIO_Mode=GPIO_Mode_OUT;
	LCD_GPIO[2].GPIO_OType=GPIO_OType_PP;
	LCD_GPIO[2].GPIO_Speed=GPIO_Speed_100MHz;
	LCD_GPIO[2].GPIO_PuPd = GPIO_PuPd_UP;
	
	for(i=0;i<3;i++)
	{
		  GPIO_Init(GPIOC, &LCD_GPIO[i]);//初始化GPIO
	}
	
}


void LCD_Byte(unsigned char val)
{
/*
#if !defined(__MSP430G2553__) && defined(_LCD_SPIMODE)
	SPIA0_TxRxByte(val);
#else
*/
	unsigned char i;
	LCD_SCLK_L;
	delay_us(1);//__MCU_FREQ
	for(i=0;i<8;i++)
	{
		if(val&0x80)
		{
			LCD_SID_H;
		}
		else
		{
			LCD_SID_L;
		}
		delay_us(1);//__MCU_FREQ
		LCD_SCLK_H;
		delay_us(1);//__MCU_FREQ
		val<<=1;
		LCD_SCLK_L;
	}
//#endif
}

void LCD_Command(unsigned char const com)
{
	unsigned char start=0xF8, higher= com & 0xF0, lower= (com << 4) & 0xF0;
	LCD_CS_H;
	LCD_Byte(start);
	LCD_Byte(higher);
	LCD_Byte(lower);
	delay_us(1);	//50
	LCD_CS_L;
}

void LCD_Data(unsigned char data)
{
	unsigned char start=0xFA, higher= data & 0xF0, lower= (data << 4) & 0xF0;
	LCD_CS_H;
	LCD_Byte(start);
	LCD_Byte(higher);
	LCD_Byte(lower);
	delay_us(1);//50
	LCD_CS_L;
}

//初始化，默认进入文本模式。
void LCD_init()
{
	LCD12864_PORT_init();
	LCD_Command(LCD_RE_0);delay_ms(10);
	LCD_Command(LCD_RE_0);delay_ms(10);
	LCD_Command(LCD_TEXT_CLR);delay_ms(10);
	LCD_Command(LCD_DISPLAY_ON);delay_ms(10);
	LCD_Command(LCD_AC_RST);delay_ms(10);
	LCD_Command(LCD_BIT_MODIFY);delay_ms(10);
}

//文本模式中清屏
void LCD_TClr()
{
	LCD_Command(LCD_TEXT_CLR);
	delay_ms(10);
}

void LCD_Data2(unsigned char *data)
{
	LCD_Data(data[0]);
	LCD_Data(data[1]);
}

int LCD_Data2End(unsigned char*data, unsigned char const end)
{
	if(data[0]==end) return 1;
	LCD_Data(data[0]);
	LCD_Data(data[1]==end?' ':data[1]);
	return data[1]==end;
}

void LCD_TInt(int row, int col, unsigned char*data)
{
	LCD_Command(LCD_DDRAM_POS(row, col));
	LCD_Data2(data);
}


void LCD_TSentence(int row,int col,unsigned char *data)
{
	unsigned char a[16];
	//unsigned char*ptr=0;
	int i=0;
//	ptr=data;
//	for(;(ptr!='\0')&&(i<8);ptr=ptr+1,i++)
	for(i=0;i<16;i=i+2)
	{
		//a[i]=*ptr;
		a[i]=data[i];
	}
	for(i=0;((col+i/2)<8)&&(a[i]!='\0');i=i+2)
	{
		LCD_TInt(row,col+i/2,&a[i]);
	}
}

void LCD_TData(int row,int col,double data)
{
	int i=0;
	double temp_double=0;
	long long int temp_int=0;
	unsigned int integer[4],decimal[4];
	unsigned char temp[10];
	const unsigned char NUM_CHAR[10]={'0','1','2','3','4','5','6','7','8','9'};
	temp_double=data;
	temp_int=data;
	for(i=1;i<5;i++)
	{
		integer[4-i]=temp_int%10;
		temp_int=temp_int/10;
	}

	temp_int=temp_double*10000;
	for(i=1;i<5;i++)
	{
		decimal[4-i]=temp_int%10;
		temp_int=temp_int/10;
	}
	for(i=0;i<9;i++)
	{
		if(i==4)
		{
			temp[i]='.';
		}
		else if(i<4)
		{
			temp[i]=NUM_CHAR[integer[i]];
		}
		else
		{
			temp[i]=NUM_CHAR[decimal[i-5]];
		}
	}
	temp[9]='\0';
	while((temp[0]=='0')&&temp[1]!='.')
	{
		array_move_L(temp);
	}
//	LCD_TLine(row,col,(unsigned char*)"              ");
	LCD_TLine(row,col,temp);
}

void array_move_L(unsigned char *temp)
{
	int i=0;
	for(i=0;i<9;i++)
	{
		temp[i]=temp[i+1];
	}
}


void LCD_TRowIncLengthEnd(int row, int col, unsigned char *data, int len, const unsigned char end, int LCD_TRowInc(int))
{
	LCD_Command(LCD_DDRAM_POS(row, col));
	while(len!=0)
	{
		--len;
		if(LCD_Data2End(data, end))
			break;
		data += 2;
		++col;
		if(col>=LCD_COL)
		{
			col=0;
			row = LCD_TRowInc(row);
			LCD_Command(LCD_DDRAM_POS(row, col));
		}
	}
}

int LCD_TMultiLineRowInc(int row) 
{ 
	return (row+1) % LCD_ROW; 
}
int LCD_TLineRowInc(int row) { return row; }

void LCD_TMultiLineLengthEnd(int row, int col, unsigned char  *data, int len, const unsigned char end)
{
	LCD_TRowIncLengthEnd(row, col, data, len, end, LCD_TMultiLineRowInc);
}

void LCD_TLineLengthEnd(int row, int col, unsigned char *data, int len, const unsigned char end)
{
	LCD_TRowIncLengthEnd(row, col, data, len, end, LCD_TLineRowInc);
}


unsigned char LCD_GD[LCD_GROW][LCD_GCOL_CHAR];

//进入文本模式
void LCD_Text()
{
	LCD_Command(LCD_RE_1);
	LCD_Command(LCD_GRAPHIC_OFF);
	LCD_Command(LCD_RE_0);
}

//进入绘图模式
void LCD_Graph()
{
	LCD_Command(LCD_RE_1);
	LCD_Command(LCD_GRAPHIC_ON);
}

void LCD_GPosSet(int row, int col)
{
	LCD_Command(LCD_GRAM_AC(row%32));
	LCD_Command(LCD_GRAM_AC(((row>>5)<<3)+col));
}

//绘图模式中清除绘图层
void LCD_GClr()
{
	int row, col;
	for(row=0; row<LCD_GROW; row++)
	{
		LCD_GPosSet(row, 0);
		for(col=0; col<LCD_GCOL_CHAR; col++)
		{
			LCD_GD[row][col]=0x00;
			LCD_Data(0x00);
		}
	}
}

void LCD_GInt(int row, int col)
{
	LCD_GPosSet(row, col);
	LCD_Data2(&LCD_GD[row][col<<1]);
}

unsigned char LCD_GGet(int x, int y)
{
	return (LCD_GD[x][y>>3]&(1<<(7-(y%8))));
}

void LCD_GSet(unsigned char *i, unsigned char j) { *i |=  j; }
void LCD_GRes(unsigned char *i, unsigned char j) { *i &= ~j; }
void LCD_GNot(unsigned char *i, unsigned char j) { *i ^=  j; }

void LCD_GDot(int x, int y, void LCD_G(unsigned char*, unsigned char))
{
	LCD_G(&LCD_GD[x][y>>3], 1<<(7-(y%8)));
	LCD_GInt(x, y>>4);
}

void LCD_GSetDot(int x, int y)
{
	LCD_GDot(x, y, LCD_GSet);
}

void LCD_GResDot(int x, int y)
{
	LCD_GDot(x, y, LCD_GRes);
}

void LCD_GNotDot(int x, int y)
{
	LCD_GDot(x, y, LCD_GNot);
}

void LCD_GLine(int x1, int y1, int x2, int y2, void LCD_GDot(int, int))
{
	int x=x2-x1, y=y2-y1, i;
	float a;
	if(x<0) x=-x;
	if(y<0) y=-y;
	if(x>y)
	{
		if(x1>x2)
		{
			i=x1;x1=x2;x2=i;
			i=y1;y1=y2;y2=i;
		}
		a=((y1-y2)*1.0)/(x1-x2);
		for(i=0;i<=x;i++)
		{
			LCD_GDot(i+x1, i*a+y1+0.5);
		}
	}
	else
	{
		if(y1>y2)
		{
			i=x1;x1=x2;x2=i;
			i=y1;y1=y2;y2=i;
		}
		a=((x1-x2)*1.0)/(y1-y2);
		for(i=0;i<=y;i++)
		{
			LCD_GDot(i*a+x1+0.5, i+y1);
		}
	}
}

void LCD_GSetLine(int x1, int y1, int x2, int y2)
{
	LCD_GLine(x1, y1, x2, y2, LCD_GSetDot);
}

void LCD_GResLine(int x1, int y1, int x2, int y2)
{
	LCD_GLine(x1, y1, x2, y2, LCD_GResDot);
}

void LCD_GNotLine(int x1, int y1, int x2, int y2)
{
	LCD_GLine(x1, y1, x2, y2, LCD_GNotDot);
}

void LCD_GRect(int x1, int y1, int x2, int y2, void LCD_G(unsigned char*, unsigned char))
{
	int i, j;
	if(x1>x2) { i=x1;x1=x2;x2=i; }
	if(y1>y2) { i=y1;y1=y2;y2=i; }

	if((y1>>3)<(y2>>3))
	{
		for(i=x1;i<=x2;i++)
		{
			LCD_G(&LCD_GD[i][y1>>3], (1<<(8-(y1%8)))-1);
			LCD_G(&LCD_GD[i][y2>>3], ~(1<<(7-(y2%8)))+1);
		}
	}
	else
	{
		for(i=x1;i<=x2;i++)
		{
			LCD_G(&LCD_GD[i][y1>>3], ((1<<(8-(y1%8)))-1)&(~(1<<(7-(y2%8)))+1));
		}
	}

	for(i=x1;i<=x2;i++)
	{
		for(j=(y1>>3)+1;j<=(y2>>3)-1;j++)
		{
			LCD_G(&LCD_GD[i][j], 0xFF);
		}
	}

	for(i=x1;i<=x2;i++)
	{
		LCD_GPosSet(i, y1>>4);
		for(j=(y1>>4);j<=(y2>>4);j++)
		{
			LCD_Data2(&LCD_GD[i][j<<1]);
		}
	}
}
