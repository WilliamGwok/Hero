#include "State.h"
#include "rc.h"
#include "Gimbal.h"
#include "Chassis.h"
#include "Shoot_motor.h"
#include "stm32f4xx_hal.h"
#include "judge_sensor.h"
#include "key.h"


char Thumb_Wheel_Ctrl = 0;
char Servo_Wheel_Ctrl = 0;
char fric_prestate = 0;
char fric_nowstate = 0;
char single_nowstate = 0;
char single_prestate = 0;
char Keep_To_Single_Switch = 0;
uint32_t Normal_Tick = 0;
uint32_t Jam_Tick = 0;


uint32_t test = 0;

Move_state_t Move_state;
Dir_state_t Dir_state;
Shoot_state_t Shoot_state;
Gim_Ctrl_t Gim_Ctrl;
Remote_Ctrl_t Remote_Ctrl;

#define Rc_Msg_S1_Up    2
#define Rc_Msg_S1_Mid   3
#define Rc_Msg_S1_Down  1
#define Rc_Msg_S2_Up    1
#define Rc_Msg_S2_Mid   3
#define Rc_Msg_S2_Down  2

void Mode_Init(void)
{
	Move_state.Imu_Mode = FALSE;
	Move_state.Mec_Mode = FALSE;
	Move_state.Top_Mode = FALSE;
	Move_state.Gim_Init_Flg = FALSE;
	Dir_state.Front_Mode = FALSE;
	Dir_state.Back_Mode = FALSE;
	Shoot_state.Friction_Mode = FALSE;
	Shoot_state.Fric_ok_flg = FALSE;
	Shoot_state.Feed_Keep_Mode = FALSE;
	Shoot_state.Feed_Single_Mode = FALSE;
	Shoot_state.Spin_Once = FALSE;
	Shoot_state.Jam_State = FALSE;
	Gim_Ctrl.Com_Ctrl = FALSE;
	Gim_Ctrl.RC_Ctrl  = FALSE;
	Shoot_state.Friction_Heat_OK = FALSE;
	Remote_Ctrl.RC_Mode = TRUE;
	Remote_Ctrl.Key_Mode = FALSE;
	Gim_Ctrl.Head_Flag = FALSE;
	Dir_state.Ang45_Mode = FALSE;
	Move_state.High_Speed = FALSE;
	Move_state.Slow_Turn = FALSE;
	Thumb_Wheel_Ctrl = 0;
	Servo_Wheel_Ctrl = 0;
}


/*.....................遥控/键盘控制.....................*/
void Rc_Key_Ctrl(void)
{
	if(KEY_Z)
	{
		Remote_Ctrl.Key_Mode = TRUE;
		Remote_Ctrl.RC_Mode = FALSE;
	}
	else if(KEY_X)
	{
		Remote_Ctrl.RC_Mode = TRUE;
		Remote_Ctrl.Key_Mode = FALSE;
		Move_state.Top_Mode = FALSE;
		Dir_state.Ang45_Mode = FALSE;
	}
}





/*.....................运动模式.....................*/
void thumb_wheel_judge( void )
{
  if(rc_info.thumbwheel >= 580)
	{
		if(Move_state.Imu_Mode)
		Thumb_Wheel_Ctrl = 0;
		
		else if(Move_state.Mec_Mode)
		Servo_Wheel_Ctrl = 0;
	}
	if(rc_info.thumbwheel <= -550)
	{
		if(Move_state.Imu_Mode)
		Thumb_Wheel_Ctrl = 1;
		
		else if(Move_state.Mec_Mode)
		Servo_Wheel_Ctrl = 1;
	}
}

//ifbaibfuqebfiqebifqbeiufqbibqeui
//char Xkey = 0;
//char top_now = 0;
//char top_pre = 0;

//void Key_Top_Mode_Change(void)
//{
//	if(Move_state.Top_Mode == TRUE)
//	{
//		Move_state.Top_Mode = FALSE;
//		top_pre = 1;
//	}
//	else
//	{
//		Move_state.Top_Mode = TRUE;
//		top_pre = 1;
//	}
//}

void Move_Mode_Ctrl(void)
{
	thumb_wheel_judge();
	if(Remote_Ctrl.RC_Mode == TRUE)
	{
		if(rc_info.s1 == Rc_Msg_S1_Mid) 
	  {
		  if(Thumb_Wheel_Ctrl == 0)
		  {
			  Move_state.Imu_Mode = TRUE;
			  Move_state.Top_Mode = FALSE;
		  }
		  else if(Thumb_Wheel_Ctrl == 1)
		  {
		  	Move_state.Top_Mode = TRUE;
		  	Move_state.Imu_Mode = TRUE;
		  }
	  }
	  else 
	  {
	  	Move_state.Imu_Mode = FALSE;
	  }
	
	  if(rc_info.s1 == Rc_Msg_S1_Down) 
	  {
		  Move_state.Mec_Mode = TRUE;
		  Move_state.Top_Mode = FALSE;
		  Move_state.Imu_Mode = FALSE;
		  Thumb_Wheel_Ctrl = 0;
	  }
	  else Move_state.Mec_Mode = FALSE;
	}
	else
	{
		Key_ctrl();
		Key_F();
//		if(KEY_Q)
//		{
//			Move_state.Imu_Mode = TRUE;
//			Move_state.Mec_Mode = FALSE;
//		}
//		if(KEY_E)
//		{
//			Move_state.Mec_Mode = TRUE;
//			Move_state.Imu_Mode = FALSE;
//			Move_state.Top_Mode = FALSE;
//		}
//		if(Move_state.Imu_Mode == TRUE)
//		{
//			Key_X();
//		}
	}
}









/*.....................头尾.....................*/

void Direction_Mode_Ctrl(void)
{
	if(Gimb_Motor[GIMB_Y].info->angle  >= 2752 && Gimb_Motor[GIMB_Y].info->angle <= 6848)
	{
		Dir_state.Front_Mode = TRUE;
		Dir_state.Back_Mode = FALSE;
	}
	else
	{
		Dir_state.Front_Mode = FALSE;
		Dir_state.Back_Mode = TRUE;
	}
}







/*.....................摩擦轮.....................*/

//void Key_R(void)//
//{
//  KEY_State_Judge(&Keyboard.C ,KEY_C, CHANGE_TIM , LONG_CHANGE_TIM_C);
//  switch(Keyboard.C.State)
//  {
// 		case UP:		
//      break;
//    case PRESS:
//			if(Shoot_state.Friction_Mode == TRUE)
//			{
//				Shoot_state.Friction_Mode = FALSE;
//			}
//			else
//			{
//				Shoot_state.Friction_Mode = TRUE;
//			}
//      break;
//    case SHORT_DOWN:
//      break;
//    case DOWN:
//      break;
//    case RELAX:
//      break;
//  }
//}

//摩擦轮模式切换
void Friction_Mode_Change(void)
{
	if(Shoot_state.Friction_Mode == TRUE)
	{
		Shoot_state.Friction_Mode = FALSE;
		fric_prestate = 1;
	}
	else
	{
		Shoot_state.Friction_Mode = TRUE;
		fric_prestate = 1;
	}
}
//检测s2跳变，判断是否需要改变摩擦轮状态
void Friction_Mode_Ctrl(void)
{
	if(Remote_Ctrl.RC_Mode == TRUE)
	{
		if(rc_info.s2 == Rc_Msg_S2_Down)
		{
			fric_nowstate = 1;
			
			if(fric_nowstate != fric_prestate)
			{
				Friction_Mode_Change();
			}
		}
		else
		{
			fric_nowstate = 0;
			fric_prestate = 0;
		}
	}
  else
	{
//		Key_R();
		if(KEY_R)
		{
			fric_nowstate = 1;
			
			if(fric_nowstate != fric_prestate)
			{
				Friction_Mode_Change();
			}
		}
		else
		{
			fric_nowstate = 0;
			fric_prestate = 0;
		}
		
	}
}
//判断摩擦轮是否到达足够转速
void Friction_Speed_Judge(void)
{
	if(abs(Shot_Motor[Left_Fric].info->speed) >= 1000 && abs(Shot_Motor[Right_Fric].info->speed >= 1000))
		Shoot_state.Fric_ok_flg = TRUE;
	else
		Shoot_state.Fric_ok_flg = FALSE;
}



/*.....................枪管热量判断.....................*/


void Gun_Heat_Judge(void)
{
	uint16_t heat = 0,heat_limit = 0;
	
	heat = judge_info.power_heat_data.shooter_id1_17mm_cooling_heat;
	
	heat_limit = judge_info.game_robot_status.shooter_id1_17mm_cooling_limit;
	
	if(heat < (heat_limit - 20))
	{
		Shoot_state.Friction_Heat_OK = TRUE;
	}
	else
	{
		Shoot_state.Friction_Heat_OK = FALSE;
	}
}




/*.....................2006.....................*/

void Feed_Single_Mode_Ctrl(void)//在Single_Shooting_Output函数里有进一步处理
{
	if(Remote_Ctrl.RC_Mode == TRUE)
	{
		if(rc_info.s2 == Rc_Msg_S2_Up)
		{
			single_nowstate = 1;
		}
		else
		{
			single_nowstate = 0;
			single_prestate = 0;
		}
	}
	else
	{
		if(MOUSE_LEFT)
		{
			single_nowstate = 1;
		}
		else
		{
			single_nowstate = 0;
			single_prestate = 0;
		}
	}
}

void Feed_Mode_Ctrl(void)
{
	if(Move_state.Imu_Mode == TRUE)
	{
		Keep_To_Single_Switch = 0;
		
		if(Remote_Ctrl.RC_Mode == TRUE)
		{
			if(rc_info.s2 == Rc_Msg_S2_Up)
			{
				Shoot_state.Feed_Keep_Mode = TRUE;
				Shoot_state.Feed_Single_Mode = FALSE;
			}
			else
			{
				Shoot_state.Feed_Keep_Mode = FALSE;
				Shot_Motor[Feed].pid->Feed_Speed_Pid.target = 0;
			}
		}
		else
		{
			if(MOUSE_LEFT)
			{
				Shoot_state.Feed_Keep_Mode = TRUE;
				Shoot_state.Feed_Single_Mode = FALSE;
			}
			else
			{
				Shoot_state.Feed_Keep_Mode = FALSE;
				Shot_Motor[Feed].pid->Feed_Speed_Pid.target = 0;
			}
		}
	}
	else if(Move_state.Mec_Mode == TRUE)
	{
		if(Keep_To_Single_Switch == 0)//这是一次性的，是对模式切换的处理
		{
			Shot_Motor[Feed].info->total_angle = 0;
			Keep_To_Single_Switch = 1;
		}
		Shoot_state.Feed_Single_Mode = TRUE;
		Shoot_state.Feed_Keep_Mode = FALSE;
	}
}




/*.....................2006堵转判断.....................*/
char Jam_Judge(motor_t *feed)
{
	char state = 0;
	
	if(abs(feed->pid->Feed_Speed_Pid.target) > 0 && abs(feed->info->speed) < 50)
	{
		state = 1;
	}
	if(abs(feed->pid->Feed_Single_Angle_Pid.target) > 0 && abs(feed->info->speed) < 50)
	{
		state = 1;
	}
	
	return state;
}

void Jam_Confirm(motor_t *feed)
{
	char judge = 0;
	
	judge = Jam_Judge(feed);
	
	if(judge)
	{
		Jam_Tick = HAL_GetTick();
	}
	else
	{
		Normal_Tick = HAL_GetTick();
		Jam_Tick = HAL_GetTick();
	}
	
	test = Jam_Tick - Normal_Tick;
	
	if(Jam_Tick - Normal_Tick > 300)
	{
		Shoot_state.Jam_State = TRUE;//由堵转操作后置假
	}
}

void Jam_Ctrl(motor_t *feed)
{
	Jam_Confirm(feed);
}




/*.....................舵机模式判断.....................*/
char servo_nowstate = 0;
char servo_prestate = 0;

void Servo_Mode_Change(void)
{
	if(Shoot_state.Servo_State == TRUE)
	{
		Shoot_state.Servo_State = FALSE;
		servo_prestate = 1;
	}
	else
	{
		Shoot_state.Servo_State = TRUE;
		servo_prestate = 1;
	}
}

void Servo_Judge(void)
{
	thumb_wheel_judge();
	if(Remote_Ctrl.RC_Mode == TRUE)
	{
		if(Servo_Wheel_Ctrl == 1)
		  Shoot_state.Servo_State = TRUE;
	  else
		  Shoot_state.Servo_State = FALSE;
	}
	else
	{
		if(MOUSE_RIGH)
		{
			servo_nowstate = 1;
			
			if(servo_nowstate != servo_prestate)
			{
				Servo_Mode_Change();
			}
		}
		else
		{
			servo_nowstate = 0;
			servo_prestate = 0;
		}
	}
}




/*.....................云台模式控制.....................*/
void Gimbal_Ctrl(void)
{
	if(rc_info.s1 == Rc_Msg_S1_Up)
	{
		Gim_Ctrl.Com_Ctrl = TRUE;
		Gim_Ctrl.RC_Ctrl  = FALSE;
	}
	else
	{
		Gim_Ctrl.RC_Ctrl  = TRUE;
		Gim_Ctrl.Com_Ctrl = FALSE;
	}
}





/*.....................标志位总控制.....................*/

void Mode_Ctrl(void)
{
	Rc_Key_Ctrl();
	Move_Mode_Ctrl();
	Direction_Mode_Ctrl();
	Friction_Mode_Ctrl();
	Friction_Speed_Judge();
	Feed_Single_Mode_Ctrl();
	Feed_Mode_Ctrl();
	Servo_Judge();
	Gimbal_Ctrl();
	Gun_Heat_Judge();
	
	if(Remote_Ctrl.Key_Mode == TRUE)
	{
		Key_Q();
	  Key_E();
	  Key_C();
	  Key_V();
	  Key_SHIFT();
	}
	
	
	if(Shoot_state.Fric_ok_flg == TRUE)
	{
		Jam_Ctrl( &Shot_Motor[Feed] );
	}
}


