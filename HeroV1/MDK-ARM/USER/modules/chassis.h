#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stm32f4xx_hal.h"

#define CHAS_FAST_BUFF    5
#define CHAS_MID_BUFF     3
#define CHAS_SLOW_BUFF    2

typedef enum
{
	C_M_sleep,
	C_M_stop,
	C_M_normal,
	C_M_special,
}chassis_move_mode_e;

typedef enum
{
	C_S_sleep,
	C_S_stop,
	C_S_normal,
	C_S_follow,
	C_S_top,
}chassis_spin_mode_e;

typedef struct
{
	uint8_t chassis_move_mode;
	
	uint8_t chassis_spin_mode;
}Chassis_Mode_t;

typedef struct
{
	uint16_t top_speed_level_0;
	
	uint16_t top_speed_level_1;
	
	uint16_t top_speed_level_2;
}Chassis_Config_Info_t;

extern Chassis_Mode_t Chassis_Mode;

void Chassis_Init_All(void);
void Chassis_Heartbeat(void);
void Chassis_Ctrl(Chassis_Mode_t* chassis_mode);
void Chassis_Mode_Init(Chassis_Mode_t* chassis_mode);
void Chassis_Command_Init(void);
void Chassis_Process(Chassis_Mode_t* chassis_mode);

#endif

