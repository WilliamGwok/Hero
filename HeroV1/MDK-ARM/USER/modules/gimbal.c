#include "gimbal.h"
#include "gimbal_motor.h"
#include "drv_can.h"

extern motor_t Gim_Yaw;
extern motor_t Gim_Pitch;

float gimb_y_angle_in_pid_param[7] = {12,3.5,0,0,0,4000,28000};

float gimb_y_angle_pid_param[7] = {2.5,0.1,0,0,0,0,700};

float gimb_p_angle_in_pid_param[7] = {60,1,0,0,0,5000,28000};

float gimb_p_angle_pid_param[7] = {3,0,0,0,0,0,500};

int16_t can1_gimbal_send_buff[4];
int16_t can2_gimbal_send_buff[4];

void Gimbal_Init_All(void)
{
  Gim_Pitch.init(&Gim_Pitch);
	Gim_Yaw.init(&Gim_Yaw);

	Gim_Yaw.pid_init(&Gim_Yaw.pid.angle_in,gimb_y_angle_in_pid_param);
	Gim_Yaw.pid_init(&Gim_Yaw.pid.angle,gimb_y_angle_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.angle_in,gimb_p_angle_in_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.angle,gimb_p_angle_pid_param);
}

void Gimbal_Heartbeat(void)
{
	Gim_Pitch.heartbeat(&Gim_Pitch);
	Gim_Yaw.heartbeat(&Gim_Yaw);
}

void Gimbal_Ctrl(void)
{
  
}

