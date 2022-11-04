#include "main.h"
#include "can_drv.h"
#include "Motor.h"
#include "Chassis_motor.h"
#include "rp_driver.h"
#include "rp_device.h"

motor_info_t 	CHAS_MOTOR_IF[] = {
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
};

drv_can_t		CHAS_MOTOR_DRV[] = {
	[POWER_0] = {
		.type    = DRV_CAN1,
		.can_id  = CAN_ID_POWER_0,
		.tx_data = CAN_SendSingleData,
	},
	[POWER_1] = {
		.type    = DRV_CAN1,
		.can_id  = CAN_ID_POWER_1,
		.tx_data = CAN_SendSingleData,
	},
	[POWER_2] = {
		.type    = DRV_CAN1,
		.can_id  = CAN_ID_POWER_2,
		.tx_data = CAN_SendSingleData,
	},
	[POWER_3] = {
		.type    = DRV_CAN1,
		.can_id  = CAN_ID_POWER_3,
		.tx_data = CAN_SendSingleData,
	},
};

motor_pid_t 	CHAS_MOTOR_PID[CHAS_MOTOR_CNT] = {
	[POWER_0] = {	
		.speed.kp           = 8.f,
		.speed.ki           = 0.6f,
		.speed.kd           = 0.f,
		.speed.integral_max = 5000.f,
		.speed.out_max      = 8000.f,
		.speed.blind_err    = 0.f,
	},
	[POWER_1] = {	
		.speed.kp           = 8.f,
		.speed.ki           = 0.6f,
		.speed.kd           = 0.f,
		.speed.integral_max = 5000.f,
		.speed.out_max      = 8000.f,
		.speed.blind_err    = 0.f,
	},
	[POWER_2] = {	
		.speed.kp           = 8.f,
		.speed.ki           = 0.6f,
		.speed.kd           = 0.f,
		.speed.integral_max = 5000.f,
		.speed.out_max      = 8000.f,
		.speed.blind_err    = 0.f,
	},
	[POWER_3] = {	
		.speed.kp           = 8.f,
		.speed.ki           = 0.6f,
		.speed.kd           = 0.f,
		.speed.integral_max = 5000.f,
		.speed.out_max      = 8000.f,
		.speed.blind_err    = 0.f,
	},
};

motor_t		Chas_Motor[] = {
	[POWER_0] = {
		.info       = &CHAS_MOTOR_IF[POWER_0],
		.driver     = &CHAS_MOTOR_DRV[POWER_0],
		.pid        = &CHAS_MOTOR_PID[POWER_0],
		.id         = DEV_ID_POWER_0,
		.type       = CHASSIS_POWER,
		.work_state = DEV_OFFLINE,
	},
	[POWER_1] = {
		.info       = &CHAS_MOTOR_IF[POWER_1],
		.driver     = &CHAS_MOTOR_DRV[POWER_1],
		.pid        = &CHAS_MOTOR_PID[POWER_1],
		.id         = DEV_ID_POWER_1,
		.type       = CHASSIS_POWER,
		.work_state = DEV_OFFLINE,
	},
	[POWER_2] = {
		.info       = &CHAS_MOTOR_IF[POWER_2],
		.driver     = &CHAS_MOTOR_DRV[POWER_2],
		.pid        = &CHAS_MOTOR_PID[POWER_2],
		.id         = DEV_ID_POWER_2,
		.type       = CHASSIS_POWER,
		.work_state = DEV_OFFLINE,
	},
	[POWER_3] = {
		.info       = &CHAS_MOTOR_IF[POWER_3],
		.driver     = &CHAS_MOTOR_DRV[POWER_3],
		.pid        = &CHAS_MOTOR_PID[POWER_3],
		.id         = DEV_ID_POWER_3,
		.type       = CHASSIS_POWER,
		.work_state = DEV_OFFLINE,
	},
};
