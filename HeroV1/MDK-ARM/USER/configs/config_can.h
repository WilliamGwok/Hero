#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"

#define chassis_motor_LF_id 0x201
#define chassis_motor_RF_id 0x202
#define chassis_motor_LB_id 0x203
#define chassis_motor_RB_id 0x204

#define feed_motor_id             0x207
#define position_motor_id         0x202
#define friction_left_motor_id    0x206
#define friction_right_motor_id   0x207

#define gimbal_motor_pitch_id 0x205
#define gimbal_motor_yaw_id   0x208

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif


