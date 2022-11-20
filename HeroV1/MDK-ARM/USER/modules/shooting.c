#include "shooting.h"
#include "motor.h"
#include "shooting_motor.h"

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/
float feed_motor_speed_pid_param[7]           = {0,0,0,0,0,0,3000};

float feed_motor_angle_pid_param[7]           = {0,0,0,0,0,0,10000};

float position_motor_speed_pid_param[7]       = {0,0,0,0,0,10000,9000};

float friction_left_motor_speed_pid_param[7]  = {0,0,0,0,0,6000,10000};

float friction_right_motor_speed_pid_param[7] = {0,0,0,0,0,6000,10000};

Shoot_t shoot;

int16_t can1_shoot_send_buff[4];
int16_t can2_shoot_send_buff[4];

void Shooting_Init_All(void)
{
	feed_motor.init(&feed_motor);
	position_motor.init(&position_motor);
	friction_left_motor.init(&friction_left_motor);
	friction_right_motor.init(&friction_right_motor);
	
	feed_motor.pid_init(&feed_motor.pid.speed,feed_motor_speed_pid_param);
	feed_motor.pid_init(&feed_motor.pid.angle,feed_motor_angle_pid_param);
	friction_left_motor.pid_init(&friction_left_motor.pid.speed,friction_left_motor_speed_pid_param);
	friction_right_motor.pid_init(&friction_right_motor.pid.speed,friction_right_motor_speed_pid_param);
	
	shoot.feed_shoot = &feed_motor;
	shoot.position_shoot = &position_motor;
	shoot.left_friction_shoot = &friction_left_motor;
	shoot.right_friction_shoot = &friction_right_motor;
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

void Feed_Work(Shoot_t* shoot)
{
	
}

