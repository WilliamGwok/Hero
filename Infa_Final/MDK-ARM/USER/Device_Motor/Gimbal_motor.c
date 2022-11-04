#include "Gimbal_motor.h"
#include "can_drv.h"
#include "Motor.h"

motor_info_t 	GIMB_MOTOR_IF[GIMB_MOTOR_CNT] = {
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
};

drv_can_t		GIMB_MOTOR_DRV[] = {
	[GIMB_Y] = {
		.type    = DRV_CAN1,
		.can_id  = CAN_ID_GIMB_Y,
		.tx_data = CAN_SendSingleData,
		.drv_id = 0,
	},
	[GIMB_P] = {
		.type    = DRV_CAN1,
		.can_id  = CAN_ID_GIMB_P,
		.tx_data = CAN_SendSingleData,
		.drv_id = 1,
	},
};

motor_pid_t 	GIMB_MOTOR_PID[GIMB_MOTOR_CNT] = {
	[GIMB_Y] = {		
		.Mec_Speed_Pid.kp = 20.0f,//40.0f,
		.Mec_Speed_Pid.ki = 0.2f,//1.85f,
		.Mec_Speed_Pid.kd = 0.01f,
		.Mec_Speed_Pid.integral_max = 10000.f,
		.Mec_Speed_Pid.out_max = 25000.f,
		.Mec_Speed_Pid.blind_err = 6.f,
		
		.Mec_Angle_Pid.kp = 15.0f,//20.0f,
		.Mec_Angle_Pid.ki = 0.f,
		.Mec_Angle_Pid.kd = 0.f,
		.Mec_Angle_Pid.integral_max = 1000.f,
		.Mec_Angle_Pid.out_max = 25000.f,
		.Mec_Angle_Pid.blind_err = 0.f,
		
		.Imu_Speed_Pid.kp = 15.0f,//40.0f,
		.Imu_Speed_Pid.ki = 1.5f,//1.6f,
		.Imu_Speed_Pid.kd = 0.001f,
		.Imu_Speed_Pid.integral_max = 10000.f,
		.Imu_Speed_Pid.out_max = 25000.f,
		.Imu_Speed_Pid.blind_err = 0.f,
		
		.Imu_Angle_Pid.kp = 120.0f,//250.0f,
		.Imu_Angle_Pid.ki = 0.f,
		.Imu_Angle_Pid.kd = 0.f,
		.Imu_Angle_Pid.integral_max = 1000.f,
		.Imu_Angle_Pid.out_max = 12500.f,
		.Imu_Angle_Pid.blind_err = 0.f,
		.Imu_Angle_Pid.pid_flag = YAW_PID_FLAG,
		
		.For_Chas_Speed_Pid.kp = 0.3f,
		.For_Chas_Speed_Pid.ki = 0.001f,
		.For_Chas_Speed_Pid.kd = 0.0f,
		.For_Chas_Speed_Pid.integral_max = 10000.f,
		.For_Chas_Speed_Pid.out_max = 25000.f,
		.For_Chas_Speed_Pid.blind_err = 10.f,
		
		.For_Chas_Angle_Pid.kp = 4.32f,
		.For_Chas_Angle_Pid.ki = 0.f,
		.For_Chas_Angle_Pid.kd = 0.f,
		.For_Chas_Angle_Pid.integral_max = 1000.f,
		.For_Chas_Angle_Pid.out_max = 12500.f,
		.For_Chas_Angle_Pid.blind_err = 0.f,
		.For_Chas_Angle_Pid.pid_flag = FOR_CHAS_PID_FLAG,
	},
	[GIMB_P] = {
		.Mec_Speed_Pid.kp = 15.0f,//25.0f,
		.Mec_Speed_Pid.ki = 0.2f,//1.50f,
		.Mec_Speed_Pid.kd = 0.f,
		.Mec_Speed_Pid.integral_max = 20000.f,
		.Mec_Speed_Pid.out_max = 25000.f,
		.Mec_Speed_Pid.blind_err = 6.f,
		
		.Mec_Angle_Pid.kp = 20.0f,//10.f,
		.Mec_Angle_Pid.ki = 0.f,
		.Mec_Angle_Pid.kd = 0.f,
		.Mec_Angle_Pid.integral_max = 500.f,
		.Mec_Angle_Pid.out_max = 25000.f,
		.Mec_Angle_Pid.blind_err = 0.f,
		
		.Imu_Angle_Pid.kp = 200.f,
		.Imu_Angle_Pid.ki = 0.f,
		.Imu_Angle_Pid.kd = 0.f,
		.Imu_Angle_Pid.integral_max = 500.f,
		.Imu_Angle_Pid.out_max = 25000.f,
		.Imu_Angle_Pid.blind_err = 0.f,
		.Imu_Angle_Pid.pid_flag = Pitch_PID_FLAG,
		
		.Imu_Speed_Pid.kp = 25.0f,
		.Imu_Speed_Pid.ki = 1.5f,
		.Imu_Speed_Pid.kd = 0.f,
		.Imu_Speed_Pid.integral_max = 25000.f,
		.Imu_Speed_Pid.out_max = 25000.f,
		.Imu_Speed_Pid.blind_err = 6.f,
	},
};

motor_t		Gimb_Motor[] = {
	[GIMB_Y] = {
		.info       = &GIMB_MOTOR_IF[GIMB_Y],
		.driver     = &GIMB_MOTOR_DRV[GIMB_Y],
		.pid        = &GIMB_MOTOR_PID[GIMB_Y],
		.id         = DEV_ID_GIMB_Y,
		.type       = GIMBAL,
		.work_state = DEV_OFFLINE,
	},
	[GIMB_P] = {
		.info       = &GIMB_MOTOR_IF[GIMB_P],
		.driver     = &GIMB_MOTOR_DRV[GIMB_P],
		.pid        = &GIMB_MOTOR_PID[GIMB_P],
		.id         = DEV_ID_GIMB_P,
		.type       = GIMBAL,
		.work_state = DEV_OFFLINE,
	},
};

