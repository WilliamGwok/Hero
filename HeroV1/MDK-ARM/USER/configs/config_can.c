#include "config_can.h"
#include "shooting_motor.h"
#include "chassis_motor.h"
#include "config_status.h"

//void CAN1_Get_Data(uint32_t id, uint8_t *data)
//{
//	switch (id)
//	{
//		case feed_motor_id:
//			feed_motor.update(&feed_motor,data);
//			break;
//		
//	}
//}

//void CAN2_Get_Data(uint32_t id, uint8_t *data)
//{
//	switch (id)
//	{
//		case position_motor_id:
//			position_motor.update(&position_motor,data);
//			break;
//		case friction_left_motor_id:
//			friction_left_motor.update(&friction_left_motor,data);
//			break;
//		case friction_right_motor_id:
//			friction_right_motor.update(&friction_right_motor,data);
//			break;
//	}
//}

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	feed_motor.rx(&feed_motor,rxBuf,8);
//	
//	chassis_motor[LF].rx(&chassis_motor[LF],rxBuf,8);
//	
//	chassis_motor[RF].rx(&chassis_motor[RF],rxBuf,8);
//	
//	chassis_motor[LB].rx(&chassis_motor[LB],rxBuf,8);
//	
//	chassis_motor[RB].rx(&chassis_motor[RB],rxBuf,8);
}

void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
  position_motor.rx(&position_motor,rxBuf,8);
	
	friction_left_motor.rx(&friction_left_motor,rxBuf,8);
	
	friction_right_motor.rx(&friction_right_motor,rxBuf,8);
}

