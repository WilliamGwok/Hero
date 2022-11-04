#include "3508_motor.h"
#include "string.h"
#include "config_status.h"
#include "pid.h"


extern uint8_t can1_tx_buf[16];
extern uint8_t can2_tx_buf[16];
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


void motor_3508_info_init(motor_3508_info_t *info)
{
	info->offline_cnt = 5;
	info->offline_cnt_max = 5;
	info->target_angle_sum = 0;
	info->status = DEV_OFFLINE;
}

void motor_3508_init(motor_3508_t *motor)
{
	memset(motor->base_info,0,sizeof(motor_3508_base_info_t));
	
	motor_3508_info_init(motor->info);
	
	if(motor->pid_angle != NULL) pid_init(motor->pid_angle);
	if(motor->pid_speed != NULL) pid_init(motor->pid_speed);
}

