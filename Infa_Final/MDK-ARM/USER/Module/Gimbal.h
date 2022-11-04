#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "S_function.h"
#include "rp_config.h"
#include "motor_class.h"
#include "Gimbal_motor.h"
#include "Motor.h"
#include "imu_protocol.h"
#include "State.h"

extern float yaw_mec_final_tar;
extern float yaw_imu_final_tar;

void GIMBAL_YAW_Mec_INIT(void);
void GIMBAL_PITCH_Mec_INIT(void);
void GIMBAL_YAW_Imu_INIT(void);
void GIMBAL_PITCH_Imu_INIT(void);
void Gimbal_Motor_Current(motor_t *gimbal,int16_t *data);
void GIMBAL_CTRL(void);
void Change_To_Imu_Mode_Init(char* target_change);

#endif
