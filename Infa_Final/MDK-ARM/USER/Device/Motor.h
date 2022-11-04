#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_hal.h"
#include "pid_algo.h"
#include "Chassis_motor.h"
#include "motor_class.h"

typedef struct motor_combination_struct{
	motor_t *chas[CHAS_MOTOR_CNT];
	motor_t *gimb[GIMB_MOTOR_CNT];
	motor_t *shot[SHOT_MOTOR_CNT];
	
	void		(*init)(struct motor_struct *self);
	void		(*update)(struct motor_struct *self, uint8_t *rxBuf);
	void		(*check)(struct motor_struct *self);	
	void		(*heart_beat)(struct motor_struct *self);
	
	char    offline_num;
} motor_combination_t;

void Init_Motor(void);
void Init_Output(pid_ctrl_t *pid);
void motor_init(motor_t *motor);
void motor_update(motor_t *motor, uint8_t *rxBuf);
void motor_check(motor_t *motor);
void motor_heart_beat(motor_t *motor);
char motor_offline_check(void);
void MOTOR_CAN1_RX(uint32_t canId, uint8_t *rxBuf);
void MOTOR_CAN2_RX(uint32_t canId, uint8_t *rxBuf);
uint8_t Send_Current(uint8_t CAN, uint32_t stdId, int16_t *dat);

#endif


