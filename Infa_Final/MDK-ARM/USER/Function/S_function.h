#ifndef __RP_CONFIG_H
#define __RP_CONFIG_H

#include "stm32f4xx_hal.h"
#include "pid_algo.h"
#include "rp_math.h"
#include "Gimbal_motor.h"
#include "State.h"

#define FILTER_LEN  5


typedef struct Mean_Filter_Struct{
	char len;          
	char index;         
	float cnt;          
	float SPEED[FILTER_LEN];   
}Mean_Filter_t;

extern Mean_Filter_t Mean_Filter_Yaw_Init;
extern Mean_Filter_t Mean_Filter_Yaw;
extern Mean_Filter_t Mean_Filter_Pitch;
extern Mean_Filter_t Mean_Filter_Yaw_Mec;
extern Mean_Filter_t Mean_Filter_Pitch_Mec;

void Mean_Filter_Init(Mean_Filter_t * filter);
float Mean_Filter_Pro(Mean_Filter_t * filter,float mea_speed);
float PID_CAL(pid_ctrl_t *out, pid_ctrl_t *inn, float meas1, float meas2);
float SF(float t,float *slopeFilter,float res);

#endif

