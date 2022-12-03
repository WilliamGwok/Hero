#include "chassis.h"
#include "motor.h"
#include "drv_can.h"
#include "chassis_motor.h"
#include "config_status.h"
#include "rp_chassis.h"
#include "remote.h"
#include "math.h"
#include "mathematics.h"
#include "car.h"
#include "config_chassis.h"
#include "config_can.h"

extern bool gimbal_init_ok;
extern chassis_t Chassis;

Chassis_InitTypeDef chassis_all;

Chassis_Mode_t Chassis_Mode;

int16_t can1_chassis_send_buff[4];

float chassis_speed_pid_param[7] = {8.0f,0.33f,0,0,0,6000,15000};

void Chassis_Init_All(void)
{
	chassis_motor[LF].init(&chassis_motor[LF]);
	chassis_motor[RF].init(&chassis_motor[RF]);
	chassis_motor[LB].init(&chassis_motor[LB]);
	chassis_motor[RB].init(&chassis_motor[RB]);
	
	chassis_motor[LF].pid_init(&chassis_motor[LF].pid.speed,chassis_speed_pid_param);
	chassis_motor[RF].pid_init(&chassis_motor[RF].pid.speed,chassis_speed_pid_param);
	chassis_motor[LB].pid_init(&chassis_motor[LB].pid.speed,chassis_speed_pid_param);
	chassis_motor[RB].pid_init(&chassis_motor[RB].pid.speed,chassis_speed_pid_param);
	
	chassis_all.motor_LB = &chassis_motor[LB];
	chassis_all.motor_LF = &chassis_motor[LF];
	chassis_all.motor_RF = &chassis_motor[RF];
	chassis_all.motor_RB = &chassis_motor[RB];
  
	Chassis.init(&Chassis, &chassis_all);
	
	Chassis_Mode_Init(&Chassis_Mode);
	Chassis_Command_Init();
}

void Chassis_Heartbeat(void)
{
	chassis_motor[LF].heartbeat(&chassis_motor[LF]);
	chassis_motor[RF].heartbeat(&chassis_motor[RF]);
	chassis_motor[LB].heartbeat(&chassis_motor[LB]);
	chassis_motor[RB].heartbeat(&chassis_motor[RB]);
}

void Chassis_Mode_Init(Chassis_Mode_t* chassis_mode)
{
	chassis_mode->chassis_move_mode = C_M_sleep;
	chassis_mode->chassis_spin_mode = C_S_sleep;
}

void Chassis_Command_Init(void)
{
	top_car_on = false;
  top_car_off = false;
}

void Chassis_Mode_Update(Chassis_Mode_t* chassis_mode)
{
	if(car_mode_change == true)
	{
		switch(Car.car_move_status)
		{
			case sleep_car:
				break;
			case init_car:
				chassis_mode->chassis_move_mode = C_M_stop;
			  chassis_mode->chassis_spin_mode = C_S_stop;
		    break;
			case imu_car:
				chassis_mode->chassis_move_mode = C_M_special;
			  chassis_mode->chassis_spin_mode = C_S_follow;
			  break;
			case mec_car:
				chassis_mode->chassis_move_mode = C_M_normal;
			  chassis_mode->chassis_spin_mode = C_S_normal;
			default:
				break;
		}
	}
	/*机械模式特殊处理？*/
}

void Chassis_Command_React(Chassis_Mode_t* chassis_mode)
{
	if(Car.ctrl_mode == RC_CTRL)
	{
		if(top_car_off == true)
	  {
		  if(chassis_mode->chassis_spin_mode == C_S_top)
		  {
		  	chassis_mode->chassis_spin_mode = C_S_follow;
		  }
	  }
	  if(top_car_on == true)
	  {
		  if(chassis_mode->chassis_move_mode == C_M_special)
		  {
		  	chassis_mode->chassis_spin_mode = C_S_top;
		  }
		  else
		  {
			  
		  }
	  }
	}
	else
	{
		if(top_switch == true)
		{
			if(chassis_mode->chassis_move_mode == C_M_special)
		  {
				if(chassis_mode->chassis_spin_mode == C_S_top)
				{
					chassis_mode->chassis_spin_mode = C_S_follow;
				}
		  	else
				{
					chassis_mode->chassis_spin_mode = C_S_top;
				}
		  }
		  else
		  {
			  
		  }
		}
	}
	/*键盘切小陀螺*/
}

void Chassis_Ctrl(Chassis_Mode_t* chassis_mode)
{
	Chassis_Mode_Update(chassis_mode);
	
	Chassis_Command_React(chassis_mode);
	
	Chassis_Process(chassis_mode);
		
	Chassis_Command_Init();
}




void Chassis_Process(Chassis_Mode_t* chassis_mode)
{
	float err_yaw = 0.0f,double_pi = 3.1415926535f * 2.0f;
	char buff = 0;
	
	if(1)    //判断依据待定
	{
		buff = CHAS_MID_BUFF;
	}
	
	err_yaw = Gimbal.info->yaw_angle_mec_measure;
	
	switch(chassis_mode->chassis_move_mode)
	{
		case C_M_sleep:
			break;
		case C_M_stop:
			Chassis.base_info.target.front_speed = 0;
		  Chassis.base_info.target.right_speed = 0;
		  break;
		case C_M_normal:
			Chassis.base_info.target.front_speed = rc.base_info->ch3 * buff;
	    Chassis.base_info.target.right_speed = rc.base_info->ch2 * buff;
		  break;
		case C_M_special:
			Chassis.base_info.target.front_speed = (rc.base_info->ch3 * buff) * cos(err_yaw/8191.0f * double_pi) - (rc.base_info->ch3 * buff) * sin(err_yaw/8191.0f * double_pi);
		  Chassis.base_info.target.right_speed = (rc.base_info->ch2 * buff) * cos(err_yaw/8191.0f * double_pi) + (rc.base_info->ch3 * buff) * sin(err_yaw/8191.0f * double_pi);
		  break;
		default:
			break;
	}
	
	switch(chassis_mode->chassis_spin_mode)
	{
		case C_S_sleep:
			break;
		case C_S_stop:
			Chassis.base_info.target.cycle_speed = 0;
			break;
		case C_S_normal:
			Chassis.base_info.target.cycle_speed = rc.base_info->ch0 * buff;
		  break;
		case C_S_follow:
			if(abs(err_yaw) > 2048)
			{
				err_yaw -= sgn(err_yaw) * 4096;
			}
		
      if(abs(err_yaw) >= 100)
			{
				Chassis.base_info.target.cycle_speed = 6.f * err_yaw + sgn(err_yaw) * 100;
			}
			else
			{
				Chassis.base_info.target.cycle_speed = 3.0f * err_yaw;//这里要画图，不然会目标值跳变，会卡
			}
		  
		  
//			Chassis.base_info.target.cycle_speed = 0;//需要一个pid,需要吗？
		  break;
		case C_S_top:
			  Chassis.base_info.target.cycle_speed = chassis_top_speed_level_1;
			break;
		default:
			break;
	}

		Chassis.work(&Chassis);
}



