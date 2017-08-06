#include "main.h"
#include "PulseMeter.h"
#include <math.h>

double frequency=0;
double duty=0;
double amplitude=0;
double risingedge=0;

u32 ReadPulseData(u32 addr)
{
	u32 DataRead=0;
	u32 temp[40];
	u32 temp_t=0;
	s32 temp_s=0;
		
	unsigned long long temp_d=0;
	int i=0;
	int j=0;
	int count=0;
	SPIWriteRead32Bits(addr);
	delay_us(10);
	for(i=0;i<40;i++)
	{
		temp[i]=SPIWriteRead32Bits(addr);
		delay_us(1);
	}
	for(i=0;i<39;i++)
	{
		for(j=0;j<39-i;j++)
		{
			if(temp[j]>temp[i+1])
			{
				temp_t=temp[j+1];
				temp[j+1]=temp[j];
				temp[j]=temp_t;
			}
		}
	}
	if(addr!=AmpAddr)
	{
		for(i=10;i<30;i++)
		{
			temp_d+=temp[i];
		}
		DataRead=temp_d/20;
	}
	else
	{
		for(i=10;i<30;i++)
		{
			
			if(temp[i]>2048-Threshold&&temp[i]<2048+Threshold)
				i++;
			else
			{
				temp_s=temp[i];
				temp_s=temp_s-2048;
				if(temp_s<0)
					temp_s=-temp_s;
				temp_d+=temp_s;
				count++;
			}
		}
		
		DataRead=temp_d/count;
		//DataRead=Yn;
	}
	return DataRead;
}


double GetFreqData(void)
{
	double Ns;
	double Nt;
	double freq;
	SPIWriteRead32Bits(InSigCNTAddr);
	Ns=SPIWriteRead32Bits(InSigCNTAddr);
	SPIWriteRead32Bits(StdSigCNTAddr);
	Nt=SPIWriteRead32Bits(StdSigCNTAddr);
	
	freq=Ns*1.0f/Nt*StdFreq;
	frequency=freq;
	if(freq>MHz)
	{
		freq/=MHz;
		LCD_ShowString(650,10,200,16,48,(u8*)"MHz",0);
	}
	else if(freq>kHz)
	{
		freq/=kHz;
		LCD_ShowString(650,10,200,16,48,(u8*)"kHz",0);
	}
	else
	{
		LCD_ShowString(650,10,200,16,48,(u8*)" Hz",0);
	}
	LCD_ShowFloat(650,350,Ns,2,16,24);
	LCD_ShowFloat(650,380,Nt,2,16,24);
	return freq;	
}

#define FilterCoef 20
double GetDutyData(void)
{
	uint64_t pulsewidth=0;
	uint64_t period=0;
	double duty=0;
	double dutytemp=0;
	u32 t=0;

	for(t=0;t<FilterCoef;t++)
	{
		SPIWriteRead32Bits(PulseWidthAddr);
		pulsewidth+=SPIWriteRead32Bits(PulseWidthAddr);
		SPIWriteRead32Bits(PeriodAddr);
		period+=SPIWriteRead32Bits(PeriodAddr);
	}
	
	dutytemp=((double)pulsewidth)*1.0f/((double)period);
	if(dutytemp<1 && dutytemp>0.05)
		duty=dutytemp;
	LCD_ShowFloat(650,410,pulsewidth,2,16,24);
	LCD_ShowFloat(650,440,period,2,16,24);
	return duty*100;
}

double GetIntervalTime(void)
{
	u32 IntervalPulsewidth=0;
	double Interval=0;
	SPIWriteRead32Bits(IntervalPulsewidthAddr);
	IntervalPulsewidth=SPIWriteRead32Bits(IntervalPulsewidthAddr);
	
	Interval=IntervalPulsewidth*0.25f/400;				//	*MHz/400MHz		采用4个不同相位的时钟，所以除4
	LCD_ShowFloat(50,350,IntervalPulsewidth,2,10,24);
	return Interval;
}

double GetAmpData(void)
{
/*	int i=0;
	int j=0;
	u32 temp_t=0;*/
	double amp_mv=0;
	uint32_t amp_temp=0;
//	u32 temp_amp[40];
	
/*	
	for(i=0;i<40;i++)
	{
		temp_amp[i]=Amp;
	}
*/	
	// Bubble Sort
/*	for(i=0;i<39;i++)
	{
		for(j=0;j<39-i;j++)
		{
			if(temp_amp[j]>temp_amp[i+1])
			{
				temp_t=temp_amp[j+1];
				temp_amp[j+1]=temp_amp[j];
				temp_amp[j]=temp_t;
			}
		}
	}
*/
/*	for(i=10;i<30;i++)
	{
		amp_mv+=temp_amp[i];
	}
	*/
	SPIWriteRead32Bits(AmpAddr);
	amp_temp=SPIWriteRead32Bits(AmpAddr);
	LCD_ShowFloat(50,400,amp_temp,2,10,24);
	if(amp_temp & ADSSign)
	{
		amp_temp-=2048;
		amp_mv=(double)amp_temp;
	}
	else
	{
		amp_temp=2048-amp_temp;
		amp_mv=-1*(double)amp_temp;
	}

	amp_mv=amp_mv*ADSVREF/ADSBITS;
	//amp_mv=amp_mv*2/20;
	return amp_mv;
}

