#include "shooting.h"
#include "motor.h"
#include "shooting_motor.h"
#include "config_shooting.h"

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/
float feed_motor_position_in_pid_param[7]       = {0,0,0,0,0,0,3000};

float feed_motor_position_pid_param[7]          = {0,0,0,0,0,0,10000};

float position_motor_speed_pid_param[7]         = {0,0,0,0,0,10000,9000};

float friction_left_motor_speed_pid_param[7]    = {27,3,0,0,0,6000,10000};

float friction_right_motor_speed_pid_param[7]   = {27,3,0,0,0,6000,10000};

Shoot_config_t shoot_config =
{
	.feed_reload_angle = feed_reload_work_angle,
	.fric_speed_init = friction_work_speed_init,
	.fric_10ms_speed = friction_10ms_work_speed,
	.fric_16ms_speed = friction_16ms_work_speed,
	.position_work_speed = position_work_speed_init,
};

Shoot_info_t Shoot_Info;

Shoot_t Shoot = 
{
	.config = &shoot_config,
	.info = &Shoot_Info,
	
	.feed_ctrl = Feed_Ctrl,
	.position_ctrl = Position_Ctrl,
	.l_fric_ctrl = Left_Friction_Ctrl,
	.r_fric_ctrl = Right_Friction_Ctrl,
};

int16_t can1_shoot_send_buff[4];//只有feed
int16_t can2_shoot_send_buff[4];

void Shooting_Init_All(void)
{
	feed_motor.init(&feed_motor);
	position_motor.init(&position_motor);
	friction_left_motor.init(&friction_left_motor);
	friction_right_motor.init(&friction_right_motor);
	
	feed_motor.pid_init(&feed_motor.pid.speed,feed_motor_position_in_pid_param);
	feed_motor.pid_init(&feed_motor.pid.angle,feed_motor_position_pid_param);
	friction_left_motor.pid_init(&friction_left_motor.pid.speed,friction_left_motor_speed_pid_param);
	friction_right_motor.pid_init(&friction_right_motor.pid.speed,friction_right_motor_speed_pid_param);
	
	Shoot.feed_shoot = &feed_motor;
	Shoot.position_shoot = &position_motor;
	Shoot.left_friction_shoot = &friction_left_motor;
	Shoot.right_friction_shoot = &friction_right_motor;
}

void Shooting_Heartbeat(void)
{
	feed_motor.heartbeat(&feed_motor);
	position_motor.heartbeat(&position_motor);
	friction_left_motor.heartbeat(&friction_left_motor);
	friction_right_motor.heartbeat(&friction_right_motor);
}

//void Gimbal_Yaw_Imu_Ctrl(gimbal_t* gimbal)
//{
//	gimbal_info_t*  info = gimbal->info;
//	
//	motor_t *motor = gimbal->yaw_gimbal;
//	
//	can1_gimbal_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
//	                                                        info->yaw_angle_imu_measure,info->yaw_speed_imu_measure,
//	                                                        gimbal->info->yaw_angle_target,1);
//	
//	Gimbal_Send(motor,can1_gimbal_send_buff);
//}

void Shooting_Send(motor_t* motor,int16_t *buff)
{
  if(motor->id.drive_type == M_CAN1)
	{
		CAN1_Send_With_int16_to_uint8(motor->id.tx_id,buff);
	}
	if(motor->id.drive_type == M_CAN2)
	{
		CAN2_Send_With_int16_to_uint8(motor->id.tx_id,buff);
	}
}

void Shoot_Commmand_React(Shoot_t* shoot)
{
	
}



void Shooting_Ctrl(Shoot_t* shoot)
{
	
}


uint16_t abcdefg = friction_work_speed_init;

void Shooting_Test(void)
{
	Shoot.info->fric_speed_target = friction_work_speed_init;
	Shoot.info->feed_angle_target = feed_reload_work_angle;
	
//	Shoot.l_fric_ctrl(&Shoot);
//	Shoot.r_fric_ctrl(&Shoot);
}











void Feed_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->feed_shoot;
	
	can1_shoot_send_buff[motor->id.buff_p] = motor->c_posit(motor,shoot->info->feed_angle_target);//不能这样写
	
  Shooting_Send(motor,can1_shoot_send_buff);
}

void Position_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->position_shoot;
	
	can2_shoot_send_buff[motor->id.buff_p] = motor->c_speed(motor,shoot->info->position_speed_target);
	
	Shooting_Send(motor,can2_shoot_send_buff);
}

void Right_Friction_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->right_friction_shoot;
	
	can2_shoot_send_buff[motor->id.buff_p] = motor->c_speed(motor,shoot->info->fric_speed_target);
	
	Shooting_Send(motor,can2_shoot_send_buff);
}

void Left_Friction_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->left_friction_shoot;
	
	can2_shoot_send_buff[motor->id.buff_p] = motor->c_speed(motor,(-shoot->info->fric_speed_target));
	
	Shooting_Send(motor,can2_shoot_send_buff);
}
