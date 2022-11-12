#include "chassis.h"
#include "motor.h"
#include "drv_can.h"
#include "chassis_motor.h"
#include "config_status.h"
#include "rp_chassis.h"
#include "remote.h"

extern chassis_t Chassis;

Chassis_InitTypeDef chassis_all;

int16_t can1_chassis_send_buff[4];

float chassis_speed_pid_param[7] = {8.0f,0.33f,0,0,0,6000,15000};





void Chassis_Init_All(void)
{
	chassis_motor[LF].init(&chassis_motor[LF]);
	chassis_motor[RF].init(&chassis_motor[RF]);
	chassis_motor[LB].init(&chassis_motor[LB]);
	chassis_motor[RB].init(&chassis_motor[RB]);
	
	chassis_motor[LF].pid_init(&chassis_motor[LF].pid.speed,chassis_speed_pid_param);
	chassis_motor[RF].pid_init(&chassis_motor[RF].pid.speed,chassis_speed_pid_param);
	chassis_motor[LB].pid_init(&chassis_motor[LB].pid.speed,chassis_speed_pid_param);
	chassis_motor[RB].pid_init(&chassis_motor[RB].pid.speed,chassis_speed_pid_param);
	
	chassis_all.motor_LB = &chassis_motor[LB];
	chassis_all.motor_LF = &chassis_motor[LF];
	chassis_all.motor_RF = &chassis_motor[RF];
	chassis_all.motor_RB = &chassis_motor[RB];

	Chassis.init(&Chassis, &chassis_all);
}



void Chassis_Heartbeat(void)
{
	chassis_motor[LF].heartbeat(&chassis_motor[LF]);
	chassis_motor[RF].heartbeat(&chassis_motor[RF]);
	chassis_motor[LB].heartbeat(&chassis_motor[LB]);
	chassis_motor[RB].heartbeat(&chassis_motor[RB]);
}

void Chassis_Ctrl(void)
{
	char buff = 0;
	
	if(1)    //ÅÐ¶ÏÒÀ¾Ý´ý¶¨
	{
		buff = CHAS_MID_BUFF;
	}
	
	Chassis.base_info.target.front_speed = rc.base_info->ch3 * buff;
	Chassis.base_info.target.right_speed = rc.base_info->ch2 * buff;
	Chassis.base_info.target.cycle_speed = rc.base_info->ch0 * buff;
	
	Chassis.work(&Chassis);
}


