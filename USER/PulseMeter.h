#ifndef PULSEMETER_H_
#define PULSEMETER_H_

#include "sys.h"
	
#define InSigCNTAddr				0x00000000
#define StdSigCNTAddr				0x00000001
#define PulseWidthAddr				0x00000002
#define PeriodAddr					0x00000003
#define IntervalPulsewidthAddr		0x00000004
#define AmpAddr						0x00000005
#define DAC904Addr					0x00000006


#define Threshold					100

#ifndef kHz
#define kHz							(1000)
#endif
#ifndef MHz
#define MHz							(1000*kHz)
#endif
#define StdFreq						(400*MHz)		//400MHz

#define ADSBITS						2048
#define ADSVREF						1000			//mV
#define ADSSign						0x00000800

extern double frequency;
extern double amplitude;
extern double risingedge;

u32 ReadPulseData(u32 addr);
double GetFreqData(void);
double GetDutyData(void);
double GetIntervalTime(void);
double GetAmpData(void);

#define EdgeTime				risingedge=SPIWiteRead32Bits(EdgeTimeAddr)
#define InSigCNT				SPIWiteRead32Bits(InSigCNTAddr)
#define StdSigCNT				SPIWiteRead32Bits(StdSigCNTAddr)
#define PulseWidth				SPIWiteRead32Bits(PulseWidthAddr)
#define Period					SPIWiteRead32Bits(PeriodAddr)
//#define Amp						amplitude=SPIWiteRead32Bits(AmpAddr)

#define Freq					GetFreqData()
#define Duty					GetDutyData()
#define IntervalTime			GetIntervalTime()
#define Amp						GetAmpData()

#endif

