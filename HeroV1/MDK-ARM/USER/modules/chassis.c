#include "chassis.h"
#include "motor.h"
#include "drv_can.h"
#include "chassis_motor.h"
#include "config_status.h"
//#include "rp_chassis.h"

//extern chassis_t Chassis;

//float chassis_speed_pid_param[7] = {8.0f,0.33f,0,0,0,6000,15000};

//void Chassis_Init(void)
//{
//	chassis_motor[LF].init(&chassis_motor[LF]);
//	chassis_motor[RF].init(&chassis_motor[RF]);
//	chassis_motor[LB].init(&chassis_motor[LB]);
//	chassis_motor[RB].init(&chassis_motor[RB]);
//	
//	chassis_motor[LF].pid_init(&chassis_motor[LF].pid.speed,chassis_speed_pid_param);
//	chassis_motor[RF].pid_init(&chassis_motor[RF].pid.speed,chassis_speed_pid_param);
//	chassis_motor[LB].pid_init(&chassis_motor[LB].pid.speed,chassis_speed_pid_param);
//	chassis_motor[RB].pid_init(&chassis_motor[RB].pid.speed,chassis_speed_pid_param);
//}

//struct Chassis_Init_structure chassis_all = 
//{
//	motor_LF = &chassis_motor[LF];
//};

//void Chassis_All_Init(void)
//{
//	Chassis_InitTypeDef chassis_all;
//	
//	chassis_all.motor_LB = &chassis_motor[LB];
//	chassis_all.motor_LF = &chassis_motor[LF];
//	chassis_all.motor_RB = &chassis_motor[RB];
//	chassis_all.motor_RF = &chassis_motor[RF];
////	chassis_all.power_limit_buffer = NULL;
//	
//	Chassis_Init(&Chassis,&chassis_all);
//}


