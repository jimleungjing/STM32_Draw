#include "filter.h"
#include "string.h"
/*
*******************************************************
*Name		:KalmanFilter_Calc
*Description:
*Arguments	:KM			a pointer to the Kalman_t struct
*Return		:none
*Note		:
*Example	:
*******************************************************
*/
void KalmanFilter_Calc(struct Kalman_t* KM)					//	 1D
{
	KM->Xk=KM->Fk*KM->Xk + KM->Bk*KM->Uk + KM->Wk;			//	 Xk=Fk*Xk + Bk*Uk + wk;	
	KM->Pk=KM->Fk*KM->Pk*KM->Fk + KM->Qk;					//   Pk=Fk*Pk*(Fk')	+ Qk; 
	KM->Kk=KM->Pk*KM->Hk/(KM->Hk*KM->Pk*KM->Hk + KM->Rk);	//   Kk=Pk*(Hk')/(Hk*Pk*(Hk') + Rk) 
	KM->Xk=KM->Xk + KM->Kk*(KM->Zk-KM->Hk*KM->Xk);			//	 Xk=Xk + Kk*(Zk - Hk*Xk); 
	KM->Pk=(1-KM->Kk*KM->Hk)*KM->Pk;						//	 Pk=(1-Kk*Hk)*Pk; 
}

void KalmanFilter_Reset(struct Kalman_t* KM)
{
	KM->Qk=0;
	KM->Rk=0;
	KM->Wk=0;
}

/*
*********************
*Name:MedianValue
*Note:中位值滤波法
*********************
*/
float MedianArr[MedianSize]={0};
float MedianValue(float* MedianArr,int size)
{
	BubbleSort(MedianArr,size);
	return MedianArr[size>>1];
}

/*
*********************
*Name:Average
*Note:平均值滤波法
*********************
*/
float AverageArr[AverageSize];
float Average(float* AverageArr,int size)
{
	int i;
	float sum=0;
	for(i=0;i<size;i++)
	{
		sum+=AverageArr[i];
	}
	sum/=size;
	return sum;
}

/*
*********************
*Name:MovingAverage
*Note:滑动平均值滤波法
*********************
*/
float MovingAverageArr[MovingAverageSize]={0};
float MovingAverage(float* MovArr,int size,float data)
{
	static int front=0;
	int i;
	float sum=0;
	MovArr[front]=data;
	for(i=0;i<size;i++)
	{
		sum+=MovArr[i];
	}
	sum/=size;
	front++;
	if(front==size)
		front=0;
	return sum;
}

/*
********************************************************************
*Name		:FIR
*Description:
*Arguments	:FIR			a pointer to the FIR_t struct
*Return		:FIR->output
*Note		:有限长单位冲击响应滤波法
*Example	:
			:FIR.newdata=SampleADS();
			:FIR.Calc(&FIR);
********************************************************************
*/
double FIR_Calc(struct FIR_t* FIR)
{
	int j=0;
	int i=0;
	FIR->front=(FIR->front+1)%FIRNum;
	FIR->FIRData[FIR->front]=FIR->newdata;

	for(;j<FIRNum;j++)
	{
		if((FIR->front-j)<0)
			i=FIRNum+FIR->front-j;
		else
			i=FIR->front-j;
		FIR->output+=(FIR->FIRCoef[j]*FIR->FIRData[i]);
	}
	return FIR->output;
}

void FIR_Reset(struct FIR_t* FIR)
{
	memset(FIR->FIRData,0,sizeof(FIR->FIRData));
	FIR->newdata=0;
	FIR->front=0;
	FIR->output=0;
}

/*
********************************************************************
*Name		:IIR
*Description:
*Arguments	:IIR			a pointer to the IIR_t struct
*Return		:Yn
*Note		:无限长单位冲击响应滤波法
*Example	:IIR.newdata=SampleADs();
			:IIR.Calc(&IIR);	
********************************************************************
*/
double IIR_Calc(struct IIR_t* IIR)
{
	int i=0;
	int j=0;
	double Numerator=0;
	double Denominator=0;
	IIR->pXn=(IIR->pXn+1)%IIRNum;
	IIR->pYn=(IIR->pYn+1)%IIRNum;
	IIR->IIRXn[IIR->pXn]=IIR->newdata;

	for(;i<IIRNum;i++)
	{
		int cnt=0;
		if((IIR->pXn-i)<0)
			cnt=IIRNum+IIR->pXn-i;
		else
			cnt=IIR->pXn-i;
		Numerator+=(IIR->BkXn[i]*IIR->IIRXn[cnt]);			// b(k)*X(Z-k)
	}
	for(j=1;j<IIRNum;j++)
	{
		int cnt=0;
		if((IIR->pYn-j)<0)
			cnt=IIRNum+IIR->pYn-j;
		else
			cnt=IIR->pYn-j;
		Denominator+=(IIR->AkYn[j]*IIR->IIRYn[cnt]);		// a(k)*Y(Z-k)
	}
	IIR->output=IIR->IIRYn[IIR->pYn]=Numerator-Denominator;// or Numeraotr+Denominator?
	return IIR->output;
}	

void IIR_Reset(struct IIR_t* IIR)
{
	memset(IIR->IIRXn,0,sizeof(IIR->IIRXn));
	memset(IIR->IIRYn,0,sizeof(IIR->IIRYn));
	IIR->output=0;
}

/*
********************************************************************
*Name		:BubbleSort
*Description:
*Arguments	:arr , size
*Return		:
*Note		:冒泡排序
*Example	:
********************************************************************
*/
void BubbleSort(float *arr, int size)
{
	int i,j;
	for(i=0;i<size-1;i++)
	{
		for(j=0;j<size-1-i;j++)
		{
			if(arr[j]>arr[j+1])
			{
				arr[j]=arr[j]+arr[j+1];
				arr[j+1]=arr[j]-arr[j+1];
				arr[j]=arr[j]-arr[j+1];
			}
		}
	}
}
void QuickSort (float *arr, int size)
{
	
}


