#ifndef __SHOOTING_H
#define __SHOOTING_H

#include "stm32f4xx_hal.h"
#include "motor.h"

typedef struct 
{
	motor_t*  feed_shoot;
	
	motor_t*  position_shoot;
	
	motor_t*  left_friction_shoot;
	
	motor_t*  right_friction_shoot;
	
	
}Shoot_t;

void Shooting_Init_All(void);
void Shooting_Heartbeat(void);

#endif


