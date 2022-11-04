#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "S_function.h"
#include "rp_config.h"
#include "motor_class.h"
#include "Motor.h"
#include "State.h"
#include "Shoot_motor.h"
#include "tim_drv.h"

void Shooting_Ctrl(void);
void Angle_Process(motor_t * motor);

#endif

