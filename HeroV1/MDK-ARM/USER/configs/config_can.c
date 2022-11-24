#include "config_can.h"
#include "shooting_motor.h"
#include "chassis_motor.h"
#include "gimbal_motor.h"
#include "config_status.h"

//200为前4个，1FF为后4个
int16_t can1_0x200_send_buff[4];
int16_t can2_0x200_send_buff[4];
int16_t can1_0x1FF_send_buff[4];
int16_t can2_0x1FF_send_buff[4];


void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case feed_motor_id:
			feed_motor.rx(&feed_motor,rxBuf,8);
		  break;
		case chassis_motor_LF_id:
			chassis_motor[LF].rx(&chassis_motor[LF],rxBuf,8);
		  break;
		case chassis_motor_RF_id:
			chassis_motor[RF].rx(&chassis_motor[RF],rxBuf,8);
		  break;
		case chassis_motor_LB_id:
			chassis_motor[LB].rx(&chassis_motor[LB],rxBuf,8);
		  break;
		case chassis_motor_RB_id:
			chassis_motor[RB].rx(&chassis_motor[RB],rxBuf,8);
		  break;
		case gimbal_motor_yaw_id:
			Gim_Yaw.rx(&Gim_Yaw,rxBuf,8);
		  break;
	}
}

void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case position_motor_id:
			position_motor.rx(&position_motor,rxBuf,8);
		  break;
		case friction_left_motor_id:
			friction_left_motor.rx(&friction_left_motor,rxBuf,8);
		  break;
		case friction_right_motor_id:
			friction_right_motor.rx(&friction_right_motor,rxBuf,8);
		  break;
		case gimbal_motor_pitch_id:
			Gim_Pitch.rx(&Gim_Pitch,rxBuf,8);
		  break;
	}
}

