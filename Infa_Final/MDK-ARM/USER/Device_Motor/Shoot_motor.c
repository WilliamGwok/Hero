#include "Shoot_motor.h"
#include "can_drv.h"
#include "Motor.h"



/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
motor_info_t 	SHOT_MOTOR_IF[] = {
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


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
drv_can_t		SHOT_MOTOR_DRV[] = {
	[Left_Fric] = {
		.type    = DRV_CAN2,
		.can_id  = CAN_ID_FRIC_L,
		.tx_data = CAN_SendSingleData,
	},
	[Right_Fric] = {
		.type    = DRV_CAN2,
		.can_id  = CAN_ID_FRIC_R,
		.tx_data = CAN_SendSingleData,
	},
	[Feed] = {
		.type    = DRV_CAN2,
		.can_id  = CAN_ID_BOX,
		.tx_data = CAN_SendSingleData,
	},	
	[Cover] = {
		.type    = DRV_CAN2,
		.can_id  = CAN_ID_BARREL,
		.tx_data = CAN_SendSingleData,
	},	
};


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
motor_pid_t 	SHOT_MOTOR_PID[SHOT_MOTOR_CNT] = {

	[Left_Fric] = {	
		.Friction_Pid.kp = 1.7f,
		.Friction_Pid.ki = 0.16f,
		.Friction_Pid.kd = 0.f,
		.Friction_Pid.integral_max = 1000.f,
		.Friction_Pid.out_max = 16500.f,
		.Friction_Pid.blind_err = 0.f,
	},
	[Right_Fric] = {	
		.Friction_Pid.kp = 1.7f,
		.Friction_Pid.ki = 0.16f,
		.Friction_Pid.kd = 0.f,
		.Friction_Pid.integral_max = 1000.f,
		.Friction_Pid.out_max = 16500.f,
		.Friction_Pid.blind_err = 0.f,
	},	
	
	[Feed] = {	
		.Feed_Speed_Pid.kp = 1.4f,
		.Feed_Speed_Pid.ki = 0.05f,
		.Feed_Speed_Pid.kd = 0.f,
		.Feed_Speed_Pid.integral_max = 8000.f,
		.Feed_Speed_Pid.out_max = 10000.f,
		.Feed_Speed_Pid.blind_err = 0.f,
		
		.Feed_Single_Speed_Pid.kp = 4.0f,
		.Feed_Single_Speed_Pid.ki = 0.18f,
		.Feed_Single_Speed_Pid.kd = 0.f,
		.Feed_Single_Speed_Pid.integral_max = 5000.f,
		.Feed_Single_Speed_Pid.out_max = 10000.f,
		.Feed_Single_Speed_Pid.blind_err = 0.f,
		
		.Feed_Single_Angle_Pid.kp = 0.3f,
		.Feed_Single_Angle_Pid.ki = 0.f,
		.Feed_Single_Angle_Pid.kd = 0.f,
		.Feed_Single_Angle_Pid.integral_max = 1000.f,
		.Feed_Single_Angle_Pid.out_max = 12500.f,
		.Feed_Single_Angle_Pid.blind_err = 0.f,	
	},
};

motor_t		Shot_Motor[] = {
	[Left_Fric] = {
		.info       = &SHOT_MOTOR_IF[Left_Fric],
		.driver     = &SHOT_MOTOR_DRV[Left_Fric],
		.pid        = &SHOT_MOTOR_PID[Left_Fric],
		.id         = DEV_ID_FRIC_L,
		.type       = SHOOT_FRIC,
		.work_state = DEV_OFFLINE,
	},
	[Right_Fric] = {
		.info       = &SHOT_MOTOR_IF[Right_Fric],
		.driver     = &SHOT_MOTOR_DRV[Right_Fric],
		.pid        = &SHOT_MOTOR_PID[Right_Fric],
		.id         = DEV_ID_FRIC_R,
		.type       = SHOOT_FRIC,
		.work_state = DEV_OFFLINE,
	},
	[Feed] = {
		.info       = &SHOT_MOTOR_IF[Feed],
		.driver     = &SHOT_MOTOR_DRV[Feed],
		.pid        = &SHOT_MOTOR_PID[Feed],
		.id         = DEV_ID_BOX,
		.type       = SHOOT_FEED,
		.work_state = DEV_OFFLINE,
	},
};


