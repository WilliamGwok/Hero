#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "imu.h"

typedef struct
{
	float yaw_angle_target;
	
	float yaw_speed_measure;
	
	float pitch_angle_target;
	
	float pitch_speed_measure;
	
}gimbal_info_t;

typedef struct gimbal_class_t
{
	gimbal_info_t*  info;
	
	motor_t*  yaw_gimbal;
	
	motor_t*  pitch_gimbal;
	
	
	void (*imu_update)(struct gimbal_class_t *gimbal);
	void (*yaw_mec_ctrl)(struct gimbal_class_t *gimbal);
	void (*pitch_mec_ctrl)(struct gimbal_class_t *gimbal);
	
}gimbal_t;

void Gimbal_Imu_Update(gimbal_t* gimbal);
void Gimbal_Init_All(void);
void Gimbal_Heartbeat(void);
void Gimbal_Ctrl(void);

#endif


