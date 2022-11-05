#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"

#define feed_motor_id             0x207
#define position_motor_id         0x202
#define friction_left_motor_id    0x206
#define friction_right_motor_id   0x207

void CAN1_Get_Data(uint32_t id, uint8_t *data);
void CAN2_Get_Data(uint32_t id, uint8_t *data);

#endif


