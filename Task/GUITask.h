#ifndef __GUITask_H
#define  __GUITask_H
#include "stm32f4xx.h"

#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9
#define KEY_A 10
#define KEY_B 11
#define KEY_C 12 
#define KEY_D 15
#define KEY_x 13
#define KEY_E 14

#define KEY_UP KEY_B
#define KEY_DOWN KEY_C
#define KEY_MENU KEY_A
#define KEY_OK KEY_D

typedef enum{
	MainMenu=0,
	SweepFrequency=1,
	AmplitudeFrequency=2,
	PhaseFrequency=3,
	SingleToneMode=4,
	SweepMode=5,
	SetFrequency=6,
	SetStep=7,
	SetRangeMin=8,
	SetRangeMax=9,
	None=10
}SubInterfaceState_e;

typedef enum{
	CosCorrection=0,
	CuttingOut=1,
	Normal=2,
}CoordinateState_e;

/*typedef enum{
	CuttingLoading=0,
	CuttingFinished=1,
	NormalLoading=2,
	NormalFinished=3
}CoordinateWorkState_e;
*/
typedef struct{
	SubInterfaceState_e Lable;
	uint8_t flag;
}Message_t;


#define ChildNum 4
typedef struct GUIHandle_t GUIHandle_t;
struct GUIHandle_t{
	Message_t* Parent;
	Message_t* Data;
	Message_t* Children[ChildNum];
};

SubInterfaceState_e NextSibling(struct GUIHandle_t* GUI);
SubInterfaceState_e LastSibling(struct GUIHandle_t* GUI);

typedef struct HandleStack_t{
	Message_t* _elem;
	uint16_t size;
	uint16_t top;
	SubInterfaceState_e (*pop)(struct HandleStack_t* stack);
	uint8_t (*push)(struct HandleStack_t* stack,SubInterfaceState_e);
}HandleStack_t;

SubInterfaceState_e Stack_Pop(struct HandleStack_t* stack);
uint8_t Stack_Push(struct HandleStack_t* stack,SubInterfaceState_e data);	

extern HandleStack_t HandleStack;
#define StackDefaultSize 10
extern Message_t elem[StackDefaultSize];
#define HandleStack_Default \
{ \
	elem, \
	0, \
	0, \
	&Stack_Pop, \
	&Stack_Push \
} \

// KeyMessageDataProcess
typedef struct{
	double SetADS9854Freq;
	double SetSingleToneStep;
	double SetSweepMin;
	double SetSweepMax;
	double SetSweepStep;
	double SweepInterval;
}FrequencyTester_t;

#define FrequencyTester_Default \
{ \
	1, \
	100, \
	1, \
	40, \
	100, \
	390, \
} \


#define TestDataSize 400
typedef struct FrequencyResponse_t{
	double I_Amp[TestDataSize];
	double CentralFreq;
	uint16_t CentralCnt;
	double Left_3dB;
	double Right_3dB;
	double BandWidth;
	double I_Noise[TestDataSize];
	double Q_Noise[TestDataSize];
	double AmplitudeFreqData[TestDataSize];
	double PhaseFreqData[TestDataSize];
	uint16_t AmpCoordinate[TestDataSize];
	uint16_t PhaseCoordinate[TestDataSize];
	void (*INoiseTest)(struct FrequencyResponse_t* FreqRes,uint16_t pData,double I_Data);
	void (*preTest)(struct FrequencyResponse_t* FreqRes,uint16_t pData,double I_Data,double Q_Data);
	void (*AmpFreq)(struct FrequencyResponse_t* FreqRes,uint16_t pData,double I,double Q);
	void (*PhaseFreq)(struct FrequencyResponse_t* FreqRes,uint16_t pData, double I,double Q);
}FrequencyResponse_t;

void INoiseTest(struct FrequencyResponse_t* FreqRes,uint16_t pData,double I_Data);
void preTest (struct FrequencyResponse_t* FreqRes,uint16_t pData,double I_Data,double Q_Data);
void AmpFreq (struct FrequencyResponse_t* FreqRes,uint16_t pData,double I,double Q);
void PhaseFreq(struct FrequencyResponse_t* FreqRes,uint16_t pData, double I,double Q);
void I_Noise2Coordinate(double I_Noise,uint16_t* Coordinate);

#define FrequencyResponse_Default \
{ \
	{0}, \
	0, \
	0, \
	0, \
	0, \
	0, \
	{0}, \
	{0}, \
	{0}, \
	{0}, \
	{0}, \
	{0}, \
	&INoiseTest, \
	&preTest, \
	&AmpFreq, \
	&PhaseFreq, \
} \

void GUI_Init(void);
void GUIHandleTree_Init(void);
void KeyProcess(void);
void KeyMessageGUIProcess(void);
void KeyMessageDataProcess(void);

void Key0_9Process(void);
void KeyBackwardProcess(void);
void KeyUpProcess(void);
void KeyDownProcess(void);
void KeyOKProcess(void);
void TouchMessageProcess(void);

#endif
