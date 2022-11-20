#include "gimbal.h"
#include "gimbal_motor.h"
#include "config_gimbal.h"
#include "drv_can.h"
#include "mathematics.h"
#include "car.h"
#include "stdbool.h"

extern motor_t Gim_Yaw;
extern motor_t Gim_Pitch;

ave_filter_t mec_yaw_angle_fil;
ave_filter_t mec_pitch_angle_fil;

bool gimbal_init_ok = false;
/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/

gimbal_info_t gimbal_info;

gimbal_t Gimbal = 
{
	.info = &gimbal_info,
	
	.imu_update = Gimbal_Imu_Update,
	.yaw_mec_ctrl = Gimbal_Yaw_Mec_Ctrl,
	.pitch_mec_ctrl = Gimbal_Pitch_Mec_Ctrl,
	.yaw_imu_ctrl = Gimbal_Yaw_Imu_Ctrl,
	.pitch_imu_ctrl = Gimbal_Pitch_Imu_Ctrl,
};

float gimb_y_mec_angle_in_pid_param[7]   = {-500,-7,0,0,4000,4000,28000};

float gimb_y_mec_angle_pid_param[7]      = {0.75,0,0,0,0,0,700};

float gimb_p_mec_angle_in_pid_param[7]   = {-500,-5,0,0,5000,5000,28000};

float gimb_p_mec_angle_pid_param[7]      = {1,0,0,0,0,0,500};

float gimb_y_imu_angle_in_pid_param[7]   = {-500,-7,0,0,4000,4000,28000};

float gimb_y_imu_angle_pid_param[7]      = {0.55,0,0,0,0,0,700};

float gimb_p_imu_angle_in_pid_param[7]   = {-500,-7,0,0,5000,5000,28000};

float gimb_p_imu_angle_pid_param[7]      = {0.75,0,0,0,0,0,500};

int16_t can1_gimbal_send_buff[4];
int16_t can2_gimbal_send_buff[4];

void Gimbal_Init_All(void)
{
  Gim_Pitch.init(&Gim_Pitch);
	Gim_Yaw.init(&Gim_Yaw);

	Gim_Yaw.pid_init(&Gim_Yaw.pid.angle_in,gimb_y_mec_angle_in_pid_param);
	Gim_Yaw.pid_init(&Gim_Yaw.pid.angle,gimb_y_mec_angle_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.angle_in,gimb_p_mec_angle_in_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.angle,gimb_p_mec_angle_pid_param);
	
	Gim_Yaw.pid_init(&Gim_Yaw.pid.imu_angle_in,gimb_y_imu_angle_in_pid_param);
	Gim_Yaw.pid_init(&Gim_Yaw.pid.imu_angle,gimb_y_imu_angle_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.imu_angle_in,gimb_p_imu_angle_in_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.imu_angle,gimb_p_imu_angle_pid_param);
	
	Gimbal.yaw_gimbal   = &Gim_Yaw;
	Gimbal.pitch_gimbal = &Gim_Pitch;
	
	Gimbal_Mode_Init(&Gimbal);
	Gimbal_Command_Init(&Gimbal);
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
	gimbal->info->yaw_speed_imu_measure = imu.base_info->yaw_dif_speed_ave;
	
	gimbal->info->pitch_speed_imu_measure = imu.base_info->pitch_dif_speed_ave;
	
	gimbal->info->yaw_angle_imu_measure = -itm(imu.base_info->yaw);
	Gimbal_Yaw_Angle_Check(gimbal);
	
	gimbal->info->pitch_angle_imu_measure = -itm(imu.base_info->pitch);
  Gimbal_Pitch_Angle_Check(gimbal);
	
	gimbal->info->yaw_angle_mec_measure = gim_yaw_middle_angle - gimbal->yaw_gimbal->rx_info.angle;
	Gimbal_Yaw_Angle_Check(gimbal);
	gimbal->info->yaw_angle_mec_measure = ave_fil_update(&mec_yaw_angle_fil,gimbal->info->yaw_angle_mec_measure,10);
	
	gimbal->info->pitch_angle_mec_measure = gim_pitch_middle_angle - gimbal->pitch_gimbal->rx_info.angle;
	gimbal->info->pitch_angle_mec_measure = ave_fil_update(&mec_pitch_angle_fil,gimbal->info->pitch_angle_mec_measure,10);
}

void Gimbal_Mode_Init(gimbal_t* gimbal)
{
	gimbal->info->yaw_mode = G_Y_sleep;
	gimbal->info->pitch_mode = G_P_sleep;
	
}

void Gimbal_Command_Init(gimbal_t* gimbal)
{
	
}


void Gimbal_Mode_Update(gimbal_t* gimbal)
{
	if(car_mode_change == true)
	{
		switch(Car.car_move_status)
		{
			case sleep_car:
				gimbal->info->yaw_mode = G_Y_sleep;
			  gimbal->info->pitch_mode = G_P_sleep;
				break;
			case init_car:
				gimbal->info->yaw_mode = G_Y_init;
			  gimbal->info->pitch_mode = G_P_mec;
			  if(abs(gimbal->info->yaw_angle_mec_measure) < 3)
				{
					gimbal_init_ok = true;
				}
			  break;
			case mec_car:
				gimbal->info->yaw_mode = G_Y_mec;
			  gimbal->info->pitch_mode = G_P_mec;
			  break;
			case imu_car:
				gimbal->info->yaw_angle_target = gimbal->info->yaw_angle_imu_measure;
				gimbal->info->yaw_mode = G_Y_imu;
			  gimbal->info->pitch_mode = G_P_imu;
			default:
				break;
		}
	}
}

void Gimbal_Command_React(gimbal_t* gimbal)
{
	
}

void Gimbal_Work(gimbal_t* gimbal)
{
	switch(gimbal->info->yaw_mode)
	{
		case G_Y_sleep:
			break;
		case G_Y_init:
			gimbal->info->yaw_angle_target = 0;
		  gimbal->yaw_mec_ctrl(gimbal);
			break;
		case G_Y_imu:
			switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					gimbal->info->yaw_angle_target += rc.base_info->ch0 / 100.f;
				  Gimbal_Yaw_Angle_Check(gimbal);
					gimbal->yaw_imu_ctrl(gimbal);
				  break;
				case KEY_CTRL:
			    break;
				default:
					break;
			}
			break;
		case G_Y_mec:
			  gimbal->info->yaw_angle_target = 0;
			  gimbal->yaw_mec_ctrl(gimbal);
			break;
	}
	
	switch(gimbal->info->pitch_mode)
	{
		case G_P_sleep:
			break;
		case G_P_init:
			break;
		case G_P_imu:
			switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					gimbal->info->pitch_angle_target += rc.base_info->ch1 / 200.f;
				  Gimbal_Pitch_Angle_Check(gimbal);
					gimbal->pitch_imu_ctrl(gimbal);
				  break;
				case KEY_CTRL:
			    break;
				default:
					break;
			}
			break;
		case G_P_mec:
			  switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					gimbal->info->pitch_angle_target += rc.base_info->ch1 / 180.f;
				  Gimbal_Pitch_Angle_Check(gimbal);
					gimbal->pitch_mec_ctrl(gimbal);
				  break;
				case KEY_CTRL:
			    break;
				default:
					break;
			}
			break;
	}
}


void Gimbal_Ctrl(gimbal_t* gimbal)
{
	Gimbal_Mode_Update(gimbal);
	
	Gimbal_Command_React(gimbal);
	
	Gimbal_Work(gimbal);
	
	Gimbal_Command_Init(gimbal);
}

void Gimbal_Pitch_Angle_Check(gimbal_t* gimbal)
{
	float angle = 0.0f;
	
	gimbal_info_t* info = gimbal->info;
	
	angle = info->pitch_angle_target;
	if(angle > 900)
	{
		angle = 900;
	}
	if(angle < -550)
	{
		angle = -550;
	}
	info->pitch_angle_target = angle;
	
	angle = info->pitch_angle_imu_measure;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->pitch_angle_imu_measure = angle;
}

void Gimbal_Yaw_Angle_Check(gimbal_t* gimbal)
{
	float angle = 0.0f;
	
	gimbal_info_t* info = gimbal->info;
	
	angle = info->yaw_angle_target;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->yaw_angle_target = angle;
	
	angle = info->yaw_angle_mec_measure;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->yaw_angle_mec_measure = angle;
	
	angle = info->yaw_angle_imu_measure;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->yaw_angle_imu_measure = angle;
}




/*Part 2*/
//int yes = 1;
//void Gimbal_Ctrl(gimbal_t* gimbal)
//{
//	Gimbal_Mode_Update(gimbal);
//	if(yes == 1)
//	{
//		Gimbal.info->pitch_angle_target = gim_pitch_middle_angle;
////		Gimbal_Work(gimbal);
//	}
//	else if(yes == -1)
//	{
//		Gimbal.info->pitch_angle_target = gim_pitch_middle_angle;
//	}
//	
////	if(abs(Gimbal.yaw_gimbal->pid.angle.info.err) <= 1)
////	{
////		yes = -yes;
////	}
//	
//  Gimbal.pitch_mec_ctrl(&Gimbal);
//}

/*Part 1*/
void Gimbal_Yaw_Mec_Ctrl(gimbal_t* gim)
{
	motor_t *motor = gim->yaw_gimbal;
	
	can1_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.angle,&motor->pid.angle_in,
	                                                        gim->info->yaw_angle_mec_measure,gim->info->yaw_speed_imu_measure,
	                                                        gim->info->yaw_angle_target,1);
	
	Gimbal_Send(motor,can1_gimbal_send_buff);
}

void Gimbal_Pitch_Mec_Ctrl(gimbal_t* gim)
{
	motor_t *motor = gim->pitch_gimbal;
	
	can2_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.angle,&motor->pid.angle_in,
	                                                        gim->info->pitch_angle_mec_measure,gim->info->pitch_speed_imu_measure,
	                                                        gim->info->pitch_angle_target,1);
	
	Gimbal_Send(motor,can2_gimbal_send_buff);
}


void Gimbal_Yaw_Imu_Ctrl(gimbal_t* gimbal)
{
	gimbal_info_t*  info = gimbal->info;
	
	motor_t *motor = gimbal->yaw_gimbal;
	
	can1_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
	                                                        info->yaw_angle_imu_measure,info->yaw_speed_imu_measure,
	                                                        gimbal->info->yaw_angle_target,1);
	
	Gimbal_Send(motor,can1_gimbal_send_buff);
}

void Gimbal_Pitch_Imu_Ctrl(gimbal_t* gimbal)
{
	gimbal_info_t*  info = gimbal->info;
	
	motor_t *motor = gimbal->pitch_gimbal;
	
	can2_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
	                                                        info->pitch_angle_imu_measure,info->pitch_speed_imu_measure,
	                                                        gimbal->info->pitch_angle_target,1);
	
	Gimbal_Send(motor,can2_gimbal_send_buff);
}

