#include "gimbal.h"
#include "gimbal_motor.h"
#include "drv_can.h"

extern motor_t Gim_Yaw;
extern motor_t Gim_Pitch;

gimbal_info_t gimbal_info;

gimbal_t gimbal = 
{
	.info = &gimbal_info,
	.imu_update = Gimbal_Imu_Update,
};

float gimb_y_angle_in_pid_param[7]   = {12,3.5,0,0,0,4000,28000};

float gimb_y_angle_pid_param[7]      = {2.5,0.1,0,0,0,0,700};

float gimb_p_angle_in_pid_param[7]   = {60,1,0,0,0,5000,28000};

float gimb_p_angle_pid_param[7]      = {3,0,0,0,0,0,500};

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
	
	gimbal.yaw_gimbal   = &Gim_Yaw;
	gimbal.pitch_gimbal = &Gim_Pitch;
}

void Gimbal_Heartbeat(void)
{
	Gim_Pitch.heartbeat(&Gim_Pitch);
	Gim_Yaw.heartbeat(&Gim_Yaw);
}

void Gimbal_Send(motor_t* motor,int16_t *buff)
{
  if(motor->id.drive_type == M_CAN1)
	{
		CAN1_Send_With_int16_to_uint8(motor->id.tx_id,can1_gimbal_send_buff);
	}
	if(motor->id.drive_type == M_CAN2)
	{
		CAN2_Send_With_int16_to_uint8(motor->id.tx_id,can2_gimbal_send_buff);
	}
}

void Gimbal_Imu_Update(gimbal_t* gimbal)
{
	gimbal->info->yaw_speed_measure = imu.base_info->yaw_dif_speed_ave;
	
	gimbal->info->pitch_speed_measure = imu.base_info->pitch_dif_speed_ave;
}

void Gimbal_Ctrl(void)
{
	gimbal.imu_update(&gimbal);
	
	
  
}

/*Part 1*/
void Gimbal_Yaw_Mec_Ctrl(gimbal_t* gimbal)
{
	motor_rx_info_t *info = &gimbal->yaw_gimbal->rx_info;
	
	motor_t *motor = gimbal->yaw_gimbal;
	
	can1_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.angle,&motor->pid.angle_in,
	               info->angle,info->speed,gimbal->info->yaw_angle_target,1);
	
	Gimbal_Send(motor,can1_gimbal_send_buff);
}

void Gimbal_Pitch_Mec_Ctrl(gimbal_t* gimbal)
{
	motor_rx_info_t *info = &gimbal->pitch_gimbal->rx_info;
	
	motor_t *motor = gimbal->pitch_gimbal;
	
	can2_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.angle,&motor->pid.angle_in,
	               info->angle,info->speed,gimbal->info->yaw_angle_target,1);
	
	Gimbal_Send(motor,can2_gimbal_send_buff);
}


