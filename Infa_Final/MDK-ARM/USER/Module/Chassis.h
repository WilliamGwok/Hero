#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stm32f4xx_hal.h"
#include "Chassis_motor.h"
#include "S_function.h"
#include "rp_math.h"
#include "Motor.h"
#include "rc.h"
#include "rp_config.h"
#include "Gimbal_motor.h"


typedef struct CHASSIS_struct{
	int16_t V_rate;
	int16_t V_MAX;
	int16_t X;
	int16_t Y;
	int16_t Z;
	int16_t Ch0;
	int16_t Ch1;
	int16_t Ch2;
	int16_t Ch3;
}CHASSIS_t;

void CHASSIS_CTRL(void);
void Chassis_MEC_Input(void);
void Chassis_Motor_Stop(void);

#endif
