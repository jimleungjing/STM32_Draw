#include "pid.h"
#include "sys.h"
/*
********************************************************************************
*Name		:PID_Calc
*Description:The function process the xxx 	
*Arguments	:pid 			A pointer to the PID_t stuct
*Return		:void
*Notes		:	增量式PID
*Example	:
			#define CHASSIS_SPEED_ATTENUATION   (1.0f)			//稀释因子
			CMRotatePID.ref = -GimbalRef.yaw_angle_dynamic_ref;
			CMRotatePID.fdb = MPU6050_Real_Data.Gyro_Z;
			CMRotatePID.Calc(&CMRotatePID);
			Set_CM_Speed(CAN1, CHASSIS_SPEED_ATTENUATION * CM1SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM2SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM3SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM4SpeedPID.output);
********************************************************************************
*/
void PID_Calc(struct PID_t*	pid)
{
	pid->err[2]=pid->err[1];					//get last_last error
	pid->err[1]=pid->err[0];					//get last_error
	pid->err[0]=pid->ref-pid->fdb;				//get current_error
	/*----Compute all the working error variables----*/
	
	pid->KpComponent=pid->err[0]-pid->err[1];
	pid->KiComponent=(pid->err[0]+pid->err[1])/2;
	pid->KdComponent=pid->err[0]-pid->err[1]*2+pid->err[2];
	
	pid->output+=(pid->kp*pid->KpComponent+pid->ki*pid->KiComponent+pid->kd*pid->KdComponent);
	if(pid->output>pid->outputMax)
		pid->output=pid->outputMax;
	if(pid->output<pid->outputMin)
		pid->output=pid->outputMin;
}

void PID_Reset(struct PID_t* pid)
{
	pid->err[2]=0;
	pid->err[1]=0;
	pid->err[0]=0;
	/*
	pid->KpComponent=0;
	pid->KiComponent=0;
	pid->KdComponent=0;
	*/
	pid->KpOffset=0;
	pid->KiOffset=0;
	pid->KdOffset=0;
}
