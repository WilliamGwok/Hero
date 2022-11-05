#include "config_can.h"

void CAN1_Get_Data(uint32_t id, uint8_t *data)
{
	switch (id)
	{
		case feed_motor_id:
			break;
		
	}
}

void CAN2_Get_Data(uint32_t id, uint8_t *data)
{
	switch (id)
	{
		case position_motor_id:
			break;
		case friction_left_motor_id:
			break;
		case friction_right_motor_id:
			break;
	}
}

