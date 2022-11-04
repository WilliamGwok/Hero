#ifndef __STATE_H
#define __STATE_H

#include "rp_config.h"

//#define Imu_Mode   (rc_info.s1==3)
//#define Mec_Mode   (rc_info.s1==2)

//typedef enum{
//	RAW_CTRL,
//}ctrl_mode_t;

typedef enum{
	FALSE = 0,
	TRUE = 1,
}state_t;

typedef struct Move_State_Struct{
	state_t  Imu_Mode;
	state_t  Mec_Mode;
	state_t  Top_Mode;
	state_t  Gim_Init_Flg;
	state_t  High_Speed;
	state_t  Slow_Turn;
}Move_state_t;

typedef struct Shoot_State_Struct{
	state_t  Friction_Heat_OK;
	state_t  Friction_Mode;
  state_t  Fric_ok_flg;
	state_t  Feed_Keep_Mode;
	state_t  Feed_Single_Mode;
	state_t  Spin_Once;
	state_t  Jam_State;
	state_t  Servo_State;
}Shoot_state_t;

typedef struct Direction_State_Struct{
	state_t  Front_Mode;
	state_t  Back_Mode;
	state_t  Ang45_Mode;
}Dir_state_t;

typedef struct Gim_Ctrl_Struct{
	state_t  RC_Ctrl;
	state_t  Com_Ctrl;
	state_t  Head_Flag;
}Gim_Ctrl_t;

typedef struct Remote_Ctrl_Struct{
	state_t  RC_Mode;
	state_t  Key_Mode;
}Remote_Ctrl_t;



extern char Servo_Wheel_Ctrl;
extern Move_state_t Move_state;
extern Dir_state_t Dir_state;
extern Shoot_state_t Shoot_state;
extern char single_nowstate;
extern char single_prestate;
extern Gim_Ctrl_t Gim_Ctrl;
extern Remote_Ctrl_t Remote_Ctrl;

void Mode_Init(void);
void Move_Mode_Ctrl(void);
void Mode_Ctrl(void);

#endif


