/**
  ******************************************************************************
  * @file    pid.h 
  * @author  Jim Liang
  * @version V1.0
  * @date    2017/7/10
  ******************************************************************************
  */

#ifndef __PID_H__
#define __PID_H__

#include "main.h"
#include "sys.h"

typedef struct PID_t
{
	float ref;
	float fdb;
	float output;
	float outputMax;
	float outputMin;
	float err[3];					//err[0]:current_err;	err[1]:last_err;	err[2]:last_last_err;
	float kp;
	float ki;
	float kd;
	float KpOffset;					//kp=kp_default+kp_offset
	float KiOffset;					//ki=ki_default+ki_offset
	float KdOffset;					//kd=kd_default+kd_offset
	float KpComponent;
	float KiComponent;
	float KdComponent;
	float KpComponentMax;
	float KiComponentMax;
	float KdComponentMax;
	void (*Calc)(struct PID_t* pid);
	void (*Reset)(struct PID_t* pid);
}PID_t;

void PID_Calc(struct PID_t*	pid);
void PID_Reset(struct PID_t* pid);
/*
#define	PWM_PID_DEFAULT\
{\
	0,\
	0,\
	0,\
	PWM_OUTPUTMAX,\
	PWM_OUTPUTMIN,\
	{0,0,0},\
	1.0f,\
	0,\
	0,\

	0,\
	0,\
	0,\
	
	0,\
	0,\
	0,\
	KpComponentMax_coef,\
	KiComponentMax_coef,\
	KdComponentMax_coef,\
	&PID_Calc,\
	&PID_Reset,\
}\
*/
#endif
