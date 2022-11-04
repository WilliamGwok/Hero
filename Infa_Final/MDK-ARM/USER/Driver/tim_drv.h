#ifndef __DRV_TIM_H
#define __DRV_TIM_H

#include "stm32f4xx_hal.h"

#define SERVO_PWM	  TIM1->CCR2

void PWM_Init(void);
void Servo_Out(int16_t pwm);
void Servo_Stop(void);
void Servo_weak(void);

#endif

