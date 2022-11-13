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
}


void Shooting_Heartbeat(void)
{
	feed_motor.heartbeat(&feed_motor);
	position_motor.heartbeat(&position_motor);
	friction_left_motor.heartbeat(&friction_left_motor);
	friction_right_motor.heartbeat(&friction_right_motor);
}

