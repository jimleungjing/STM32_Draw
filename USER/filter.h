/**
  ******************************************************************************
  * @file    pid.h 
  * @author  Jim Liang
  * @version V1.0
  * @date    2017/7/12
  ******************************************************************************
 **/
#ifndef __FILTER__
#define __FILTER__

/*
*Name:卡尔曼滤波
*/
typedef struct Kalman_t 
{
	float Xk;				// Estimate 	Matrix
	float Fk;				// Prediction	Matrix
	float Bk;				// Control		Matrix
	float Uk;				// Control		Vector
	float Wk;				// Uncertainty	Factor
	/*-- Xk=Fk*Xk + Bk*Uk + wk;	--*/
	
	float Pk;				// Covariance 	Maxtrix
	float Qk;				// Nosie covariance
	/*-- Pk=Fk*Pk*(Fk')	+ Qk; --*/

	float Hk;				// Transform 	Matrix	:transform Xk to sensor data: Zk	
	float Rk;				// Covariance of uncertainty (sensor noise)
	float Kk;				// Kalman Gain
	/*-- Kk=Pk*(Hk')/(Hk*Pk*(Hk') + Rk) --*/
	
	float Zk;				// Observed measurement 
	/*-- Xk=Xk + Kk*(Zk - Hk*Xk); --*/
	/*-- Pk=(1-Kk*Hk)*Pk; --*/	
	void (*Calc)(struct Kalman_t* KM);
	void (*Reset)(struct Kalman_t* KM);
}Kalman_t;

void KalmanFilter_Calc(struct Kalman_t* KM);
void KalmanFilter_Reset(struct Kalman_t* KM);

/*
*Name:中位值滤波
*/
#define MedianSize 8
extern float MedianArr[MedianSize];
float MedianValue(float* MedianArr,int size);

/*
*Name:平均值滤波
*/
#define AverageSize 8
extern float AverageArr[AverageSize];
float Average(float *AverageArr,int size);

/*
*Name:滑动平均滤波法
*/
#define MovingAverageSize 8
typedef struct MovAverage_t
{
	float MovingAverageArr[MovingAverageSize];
	float (*Calc)(struct MovAverage_t* Mov);
}MovAverage_t;
float MovingAverage(float* MovArr,int size,float data);

/*
*Name:IIR滤波法
*/
#define IIRNum	7
typedef struct IIR_t
{
	double BkXn[IIRNum];					//Numerator
	double AkYn[IIRNum];					//Denominator
	double IIRXn[IIRNum];					//Store X(n) data
	double IIRYn[IIRNum];					//Store Y(n) data
	int pXn;						//point to Xn
	int pYn;						//point to Yn
	double newdata;							//input a new data
	double output;
	double (*Calc)(struct IIR_t* IIR);
	void  (*Reset)(struct IIR_t* IIR);
}IIR_t;

double IIR_Calc(struct IIR_t* IIR);
void IIR_Reset(struct IIR_t* IIR);

#define	IIR_DEFAULT \
{ \
	{9.407519222764e-06,5.644511533659e-05,0.0001411127883415,0.0001881503844553, \
	 0.0001411127883415,5.644511533659e-05,9.407519222764e-06}, \
	{1,   -4.766525407832,    9.570559630012,   -10.34641013039, \
     6.344706779906,   -2.090769640143,   0.2890408496811}, \
	{0}, \
	{0}, \
	0, \
	0, \
	0, \
	0, \
	&IIR_Calc, \
	&IIR_Reset \
} \
//Fs:10kHz;	Fpass:500;	Fstop:1500;	Apass:3dB;	Astop:60dB

/*
*Name:FIR滤波法
*/
#define FIRNum	30
typedef struct FIR_t
{
	double FIRCoef[FIRNum];					//Store h(n)
	double FIRData[FIRNum];					//Store X(k)
	double newdata;							//input a new data
	int front;						//point to X(n)
	double output;							//output Y(n)
	double (*Calc)(struct FIR_t* FIR);
	void   (*Reset)(struct FIR_t* FIR);
}FIR_t;

double FIR_Calc(struct FIR_t* FIR);
void FIR_Reset(struct FIR_t* FIR);

#define	FIR_DEFAULT \
{ \
	{0.004151824495103,  0.01283396113059,  0.02870822095464,  0.05193181342586, \
     0.0804260569483,    0.109737367826,    0.134036062479,   0.1478439759098,	 \
     0.1478439759098,    0.134036062479,    0.109737367826,   0.0804260569483,	 \
     0.05193181342586,  0.02870822095464,  0.01283396113059, 0.004151824495103}, \
	{0}, \
	0, \
	0, \
	0, \
	&FIR_Calc, \
	&FIR_Reset \
} \
//Fs:10kHz;	Fpass:500;	Fstop:1500;	Apass:3dB;	Astop:60dB

void BubbleSort(float *arr, int size);
void QuickSort (float *arr, int size);

#endif
