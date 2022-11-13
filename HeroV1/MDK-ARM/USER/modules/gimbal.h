#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "stm32f4xx_hal.h"
#include "motor.h"

void Gimbal_Init_All(void);
void Gimbal_Heartbeat(void);
void Gimbal_Ctrl(void);

#endif


