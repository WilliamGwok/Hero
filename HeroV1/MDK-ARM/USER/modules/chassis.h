#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stm32f4xx_hal.h"

#define CHAS_FAST_BUFF    5
#define CHAS_MID_BUFF     3
#define CHAS_SLOW_BUFF    2

void Chassis_Init_All(void);
void Chassis_Heartbeat(void);
void Chassis_Ctrl(void);

#endif

