#ifndef __SHOOTING_H
#define __SHOOTING_H

#include "stm32f4xx_hal.h"
#include "motor.h"

typedef struct
{
	uint16_t feed_reload_angle;
	uint16_t fric_speed_init;
	uint16_t fric_10ms_speed;
	uint16_t fric_16ms_speed;
	uint16_t position_work_speed;
}Shoot_config_t;

typedef struct
{
	uint16_t feed_angle_target;
	uint16_t fric_speed_target;
	uint16_t position_speed_target;
}Shoot_info_t;

//typedef struct
//{
//	
//}Shoot_Status_t;

typedef struct Shoot_All_t
{
	Shoot_info_t* info;
	Shoot_config_t* config;
	
	motor_t*  feed_shoot;
	motor_t*  position_shoot;
	motor_t*  left_friction_shoot;
	motor_t*  right_friction_shoot;
	
	void (*feed_ctrl)(struct Shoot_All_t *shoot);
	void (*position_ctrl)(struct Shoot_All_t *shoot);
	void (*l_fric_ctrl)(struct Shoot_All_t *shoot);
	void (*r_fric_ctrl)(struct Shoot_All_t *shoot);
}Shoot_t;


void Feed_Ctrl(Shoot_t* shoot);
void Position_Ctrl(Shoot_t* shoot);
void Right_Friction_Ctrl(Shoot_t* shoot);
void Left_Friction_Ctrl(Shoot_t* shoot);
void Shooting_Init_All(void);
void Shooting_Heartbeat(void);
void Shooting_Test(void);

#endif


