#include "config_can.h"


#define feed_motor_id             0x207
#define position_motor_id         0x202
#define friction_left_motor_id    0x206
#define friction_right_motor_id   0x207

void CAN1_Get_Data(uint32_t id, uint8_t *data)
{
	switch (id)
	{
		case feed_motor_id:
			break;
	}
}



