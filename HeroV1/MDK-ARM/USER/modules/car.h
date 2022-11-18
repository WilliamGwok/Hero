#ifndef __CAR_H
#define __CAR_H

#include "stm32f4xx_hal.h"
#include "remote.h"

typedef enum 
{
	sleep_car,
	mec_car,
	imu_car,
}car_move_mode_t;

typedef enum
{
	RC_CTRL,
	KEY_CTRL,
}car_ctrl_mode_t;

typedef struct
{
	uint8_t move_command;
	uint8_t ctrl_command;
	uint8_t car_move_status;
}car_t;

void Rc_S2_Status_Check(car_t* car);
void Rc_S1_Status_Check(void);
void Rc_Thumbwheel_Check(car_t* car);
void car_command_init(void);
void Rc_Status_Scan(car_t* car);
void Key_Status_Scan(car_t* car);
void car_ctrl_mode_update(car_t* car);
void car_status_update(car_t* car);

#endif

