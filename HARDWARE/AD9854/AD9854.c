
#include "AD9854.h"
#include "sys.h"
#include "delay.h"

const	uint64_t AD9854_MaxFreqWord=0x0000FFFFFFFFFFFF;

u8 FreqSingle[6]={0xA,0xAA,0xAB,0x0,0x0,0x0};


//******************************************************************


//******************************************************************

//函数功能:扫频函数
//输入参数:无
//说明：头文件中可修改起始频率，步进值，和扫描速率

//******************************************************************
void Freq_SW(void)
{
	AD9854WriteCtR(ctrsw);
	AD9854WriteFreqWord1(freqbg);		//起始频率
	AD9854WriteFerqWord2(freqend);
	AD9854WriteFreqStep(freqstep);	//步进频率
	AD9854WriteFreqStay(freqstay);	//驻足时间
	Update_AD9854();
}


//函数功能：这个函数利用宏定义中的控制字表来控制单音模式的输出频率
//输入参数: freq1,1~600
//Serial Register Address :2
//写入地址字节数:1 Byte
//写入数据字节数:6 Bytes 
//******************************************************************
void AD9854WriteFreqSingle(double freq)
{
	CalculateFreqWord(freq);
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE1);              				// 0x02对应写Frq1
	AD9854WriteByte(FreqSingle);            	// 写6个字节的数据,先发高位
	AD9854WriteByte(FreqSingle+1);
	AD9854WriteByte(FreqSingle+2);
	AD9854WriteByte(FreqSingle+3);
	AD9854WriteByte(FreqSingle+4);
	AD9854WriteByte(FreqSingle+5);
	delay_us(1);
	AD9854_CS_H;
	delay_us(1);
	Update_AD9854();
}

double AD9854ReadFreqSingle(void)
{
	double FreqRead=0;
	uint64_t temp;
	temp=AD9854ReadFreqWord1();
	FreqRead=temp;
	FreqRead=FreqRead/AD9854_MaxFreqWord*AD9854_FREQ_REF;
	return FreqRead;
}

void CalculateFreqWord(double freq)
{
	double temp=AD9854_FREQ_REF;
	double FreqWord=freq*AD9854_MaxFreqWord/temp;
	uint64_t FreqWord_i=FreqWord;
	int i=0;
	for(i=0;i<6;i++)
	{
		FreqSingle[5-i]=FreqWord_i&0x00000000000000FF;
		FreqWord_i>>=8;
	}	
}
//******************************************************************

//函数功能:写入自定义频率控制字，可利用频率控制字计算器计算
//输入参数:char *freq
//******************************************************************
void AD9854WriteFreqWord1(unsigned char *freq)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE1) ;              // 0x02对应写Frq1
	AD9854WriteByte(freq);            					// 写6个字节的数据,先发高位
	AD9854WriteByte(freq+1);
	AD9854WriteByte(freq+2);
	AD9854WriteByte(freq+3);
	AD9854WriteByte(freq+4);
	AD9854WriteByte(freq+5);
	delay_us(1);
	AD9854_CS_H;
	
}

void AD9854WriteFerqWord2(unsigned char *freq)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE2);              // 0x03对应写Frq2
	AD9854WriteByte(freq);            					// 写6个字节的数据,先发高位
	AD9854WriteByte(freq+1);
	AD9854WriteByte(freq+2);
	AD9854WriteByte(freq+3);
	AD9854WriteByte(freq+4);
	AD9854WriteByte(freq+5);
	delay_us(1);
	AD9854_CS_H;
}

uint64_t AD9854ReadFreqWord1(void )
{
	uint64_t temp=0;
	uint64_t DataRead=0;
	int i=0;
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addrRead+AD9854_FRE1);
	for(i=0;i<6;i++)
	{
		DataRead>>=8;
		temp=AD9854ReadByte();
		DataRead=(temp<<40)&0x0000FF0000000000;
	}
	return DataRead;
}

uint64_t AD9854ReadFreqWord2(void )
{
	uint64_t temp=0;
	int i=0;
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_FRE2);
	for(i=0;i<6;i++)
	{
		temp<<=8;
		temp=AD9854ReadByte();
	}
	return temp;
}

void AD9854WritePhaseWord1(unsigned char *phase)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_PHA1); 
	AD9854WriteByte(phase); 
	AD9854WriteByte(phase+1);
	delay_us(1);
	AD9854_CS_H;
}

void AD9854WritePhaseWord2(unsigned char *phase)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_PHA2);              
	AD9854WriteByte(phase);
	AD9854WriteByte(phase+1);
	delay_us(1);
	AD9854_CS_H;
}
 //******************************************************************

 //函数功能:扫频模式下写步进频率
 //输入参数:char *freqst
 //******************************************************************
void AD9854WriteFreqStep(unsigned char *freqstep)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_DELTA);
	AD9854WriteByte(freqstep);
	AD9854WriteByte(freqstep+1);
	AD9854WriteByte(freqstep+2);
	AD9854WriteByte(freqstep+3);
	AD9854WriteByte(freqstep+4);
	AD9854WriteByte(freqstep+5);
	delay_us(1);
	AD9854_CS_H;
}
//******************************************************************

  //******************************************************************

  //函数功能:扫频模式下写每个频率的驻足时间
  //输入参数:char *freqstay
  //******************************************************************
void AD9854WriteFreqStay(unsigned char *freqstay)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_RAMP_CLK);
	AD9854WriteByte(freqstay);
	AD9854WriteByte(freqstay+1);
	AD9854WriteByte(freqstay+2);	
	delay_us(1);
	AD9854_CS_H;
}
 //******************************************************************

//函数功能:更新设置
//输入参数:无
//******************************************************************
void Update_AD9854(void)
{
	AD9854_UPDATE_H;
	delay_us(2);
	AD9854_UPDATE_L;
}
//******************************************************************


//函数功能:IO复位，用以通信同步
//输入参数:无
//******************************************************************
void AD9854IOReset(void)
{
	AD9854_IO_RESET_H;
	delay_us(20);
	AD9854_IO_RESET_L;
}
//******************************************************************



//函数功能:设置控制寄存器 CTR_REG (control function register)
//输入参数:ctr(指针,指向unsigned char型数组的首地址,数组长度为32位,4个字节)
//******************************************************************
void AD9854WriteCtR(unsigned char *ctr)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_CTR_REG);//先写地址
	AD9854WriteByte(ctr);         // 写4个字节的数据,先发高位
	AD9854WriteByte(ctr+1);
	AD9854WriteByte(ctr+2);
	AD9854WriteByte(ctr+3);	
	delay_us(1);
	AD9854_CS_H;
}
//******************************************************************


//函数功能:AD9854初始化
//输入参数:无
//硬件说明：
//******************************************************************
void AD9854Init(void)
{
	AD9854PortInit();
	AD9854Reset();
	AD9854WriteCtR(ctr);
   delay_ms(50);
}

//******************************************************************

//函数功能:引脚初始化
//输入参数:无
//硬件说明：可通过宏定义修改，随便哪个IO口都可以

//******************************************************************
void AD9854PortInit(void)
{
	GPIO_InitTypeDef GPIO_AD9854;
	RCC_AHB1PeriphClockCmd(RCC_AD9854_CS, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AD9854_SCLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AD9854_UPDATE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AD9854_IO_RESET, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AD9854_SDIO, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AD9854_RST, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AS9854_SDO, ENABLE);

	GPIO_AD9854.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_AD9854.GPIO_OType=GPIO_OType_PP;
	GPIO_AD9854.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_AD9854.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_AD9854.GPIO_Pin=AD9854_CS_Pin;
	GPIO_Init(AD9854_CS_GPIO,&GPIO_AD9854);

	GPIO_AD9854.GPIO_Pin=AD9854_SCLK_Pin;
	GPIO_Init(AD9854_SCLK_GPIO,&GPIO_AD9854);

	GPIO_AD9854.GPIO_Pin=AD9854_SDIO_Pin;
	GPIO_Init(AD9854_SDIO_GPIO,&GPIO_AD9854);

	GPIO_AD9854.GPIO_Pin=AD9854_UPDATE_Pin;
	GPIO_Init(AD9854_UPDATE_GPIO,&GPIO_AD9854);

	GPIO_AD9854.GPIO_Pin=AD9854_IO_RESET_Pin;
	GPIO_Init(AD9854_IO_RESET_GPIO,&GPIO_AD9854);
	
	GPIO_AD9854.GPIO_Pin=AD9854_RST_Pin;
	GPIO_Init(AD9854_RST_GPIO,&GPIO_AD9854);

	GPIO_AD9854.GPIO_Pin=AD9854_SDO_Pin;
	GPIO_AD9854.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(AD9854_SDO_GPIO,&GPIO_AD9854);
	
	AD9854_CS_L;
	AD9854_SDIO_L;
	AD9854_SCLK_L;
	AD9854_UPDATE_L;
	AD9854_IO_RESET_L;
	AD9854_RST_H;
}
//******************************************************************

//函数功能:单音模式下设置I、Q通道的输出幅度，此函数必须要控制寄存器中开启波形键控才生效
//输入参数:char *IA,char *QA
//说明：分别为12位，分两次写入，先写高位

//******************************************************************
void AMP_SETUP(unsigned char *IA,unsigned char *QA)
{
	AD9854_CS_L;
	delay_us(1);
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_I_MUL);
	AD9854WriteByte(IA);
	AD9854WriteByte(IA+1);
	Update_AD9854();
	AD9854IOReset();
	AD9854WriteByte(addr+AD9854_Q_MUL);
	AD9854WriteByte(QA);
	AD9854WriteByte(QA+1);
	delay_us(1);
	AD9854_CS_H;
	Update_AD9854();
}


void AD9854Reset(void)
{
	AD9854_RST_L;
	delay_ms(2);
	AD9854_RST_H;
	delay_ms(10);
	AD9854_RST_L;
	delay_ms(50);
}


//函数功能:写8位控制字或者数据
//MSB 最高位优先(9854默认)
//SDIO为双向数据线(9854默认)
//******************************************************************
void AD9854WriteByte(unsigned char *data)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		if((*data<<i)&0x80)
			AD9854_SDIO_H;
		else   
			AD9854_SDIO_L;
		delay_us(1);
		AD9854_SCLK_H;
		delay_us(1);
		AD9854_SCLK_L;
	}
}

u8 AD9854ReadByte(void)
{
	u8 i=0;
	u8 ByteRead=0;
	for(i=0;i<8;i++)
	{
		ByteRead=ByteRead<<1;
		delay_us(1);
		AD9854_SCLK_H;
		delay_us(1);
		if(AD9854_SDO_INPUT)
			ByteRead|=0x01;
		else
			ByteRead|=0x00;
		AD9854_SCLK_L;
	}
	return ByteRead;
}

//******************************************************************

