#include "tim_drv.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

void PWM_Init(void)
{
	HAL_TIM_Base_Start_IT(&htim4);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	
	Servo_Out(0);
}

void Servo_Out(int16_t pwm)
{
	TIM1->CCR2 = pwm;
}

void Servo_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}	

void Servo_weak(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}


