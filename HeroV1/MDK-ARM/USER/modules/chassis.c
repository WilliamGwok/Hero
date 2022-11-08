#include "chassis.h"
#include "motor.h"
#include "drv_can.h"
#include "chassis_motor.h"
#include "config_status.h"
#include "rp_chassis.h"

Chassis_InitTypeDef chassis_all;
extern chassis_t Chassis;
float chassis_speed_pid_param[7] = {8.0f,0.33f,0,0,0,6000,15000};

int16_t can1_chassis_send_buff[4];
//可以改，不同电机不同速度
float chassis_tar = 0;

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

void Chassis_Ctrl_All(void)
{
	chassis_motor[LF].heartbeat(&chassis_motor[LF]);
	chassis_motor[RF].heartbeat(&chassis_motor[RF]);
	chassis_motor[LB].heartbeat(&chassis_motor[LB]);
	chassis_motor[RB].heartbeat(&chassis_motor[RB]);
		
//	can1_chassis_send_buff[chassis_motor[LF].id.buff_p] = chassis_motor[LF].c_speed(&chassis_motor[LF],chassis_tar);
//	can1_chassis_send_buff[chassis_motor[RF].id.buff_p] = chassis_motor[RF].c_speed(&chassis_motor[RF],chassis_tar);
//	can1_chassis_send_buff[chassis_motor[LB].id.buff_p] = chassis_motor[LB].c_speed(&chassis_motor[LB],chassis_tar);
//	can1_chassis_send_buff[chassis_motor[RB].id.buff_p] = chassis_motor[RB].c_speed(&chassis_motor[RB],chassis_tar);
//		
//	CAN1_Send_With_int16_to_uint8(chassis_motor[LF].id.tx_id,can1_chassis_send_buff);
//	CAN1_Send_With_int16_to_uint8(chassis_motor[RF].id.tx_id,can1_chassis_send_buff);
//	CAN1_Send_With_int16_to_uint8(chassis_motor[LB].id.tx_id,can1_chassis_send_buff);
//	CAN1_Send_With_int16_to_uint8(chassis_motor[RB].id.tx_id,can1_chassis_send_buff);
}


