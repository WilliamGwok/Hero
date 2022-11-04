#include "Chassis.h"
#include "State.h"
#include "imu_sensor.h"
#include "math.h"
#include "Gimbal.h"
#include "Power_Limit.h"
#include "judge_sensor.h"

#define SPEED_MAX 8000.f
float x;

int16_t	Chassis_Power_Current[4] = {0,0,0,0};
CHASSIS_t CHASSIS;

float Speed_zz_Imu_Cal(motor_t * gimbal)
{
	float speed_zz_output = 0.0f;
	
	motor_pid_t  *pid  = gimbal->pid;	
	motor_info_t *info = gimbal->info;
	
	if(Dir_state.Front_Mode)
	{
		pid->For_Chas_Angle_Pid.target = 4800;//返回头部
		yaw_mec_final_tar = 4800;
		if(Dir_state.Ang45_Mode == TRUE)
		{
			pid->For_Chas_Angle_Pid.target = (4800-1024);
		}
	}
	else
	{
		pid->For_Chas_Angle_Pid.target = 660;//返回尾部
		yaw_mec_final_tar = 660;
		if(Dir_state.Ang45_Mode == TRUE)
		{
			pid->For_Chas_Angle_Pid.target = (660-1024);
		}
	}
	
	pid->out = PID_CAL(&pid->For_Chas_Angle_Pid,
	          				 &pid->For_Chas_Speed_Pid,
	          				 info->angle, info->speed);
	
	speed_zz_output = pid->out;
	
	return speed_zz_output;
}

void Chassis_Power_Transmit(int16_t *data)
{
	float OUT[4];
	
	for(char i = 0 ; i < 4 ; i++)
	{
		OUT[i] = Chas_Motor[i].pid->out;		
	}
	
	for(char i = 0 ; i < 4 ; i++)
	{
		if(Chas_Motor[i].work_state == DEV_ONLINE)
			data[i] = OUT[i];		
		else
			data[i] = 0;
	}
}

void Chassis_MEC_Input(void)
{
  float speed_max = 0.0f,speed_zz,speed_xy[4],speed[4],scale=1.0f,buff,top_err,top_err2;
	
	buff = SPEED_MAX/660.0f;
	
	if(Move_state.High_Speed == TRUE)
	{
		buff *= 3.0f;
	}
	
	if(Move_state.Mec_Mode == TRUE)
	{
		buff *= 0.7f;
	}
	
	CHASSIS.V_MAX = SPEED_MAX;
	
	if(Remote_Ctrl.RC_Mode == TRUE)
	{
		CHASSIS.Ch2 = CH2_VALUE;
    CHASSIS.Ch3 = CH3_VALUE;
    CHASSIS.Ch0 = CH0_VALUE;
	}
	else
	{
		CHASSIS.Ch2 = CH2_VALUE_K;
    CHASSIS.Ch3 = CH3_VALUE_K;
    CHASSIS.Ch0 = CH0_VALUE_K;
		buff *= 2.7f;
	}
  
	
		//判断头部方向
	if(Dir_state.Back_Mode && Move_state.Top_Mode == FALSE)
	{
		if(Remote_Ctrl.RC_Mode == TRUE)
		{
			CHASSIS.Ch2 = -CH2_VALUE;
      CHASSIS.Ch3 = -CH3_VALUE;
		}
		else
		{
			CHASSIS.Ch2 = -CH2_VALUE_K;
			CHASSIS.Ch3 = -CH3_VALUE_K;
		}
	}
  
  CHASSIS.X =  CHASSIS.Ch2 * buff;
  CHASSIS.Y =  CHASSIS.Ch3 * buff;
  CHASSIS.Z =  CHASSIS.Ch0 * buff;
  
	//判断移动模式
	if(Move_state.Mec_Mode)
	{
		CHASSIS.X =  CHASSIS.Ch2 * buff;
    CHASSIS.Y =  CHASSIS.Ch3 * buff;
	}
	else if(Move_state.Imu_Mode == TRUE && Move_state.Top_Mode == FALSE)
	{
		CHASSIS.X =  CHASSIS.Ch2 * buff;
    CHASSIS.Y =  CHASSIS.Ch3 * buff;
	}
	else if(Move_state.Imu_Mode == TRUE && Move_state.Top_Mode == TRUE)
	{
		top_err = 4800.0f - Gimb_Motor[GIMB_Y].info->angle;
		CHASSIS.X = (CHASSIS.Ch2 * buff) * cos(top_err/8191.0f * 2.0f * 3.1415f) + (CHASSIS.Ch3 * buff) * sin(top_err/8191.0f * 2.0f * 3.1415f);
		CHASSIS.Y = (CHASSIS.Ch3 * buff) * cos(top_err/8191.0f * 2.0f * 3.1415f) - (CHASSIS.Ch2 * buff) * sin(top_err/8191.0f * 2.0f * 3.1415f);
	}
	
	if(Move_state.Mec_Mode)
	{
		if(Remote_Ctrl.RC_Mode == TRUE)
		  speed_zz = CHASSIS.Z * 0.6;
		else
			if(Move_state.Slow_Turn == FALSE)
			{
				speed_zz = CHASSIS.Z * 0.1;
			}
			else
			{
				if(Q_FLAG)
				{
					speed_zz += 500;
				}
				else
				{
					speed_zz -= 500;
				}
			}
		CHASSIS.V_MAX = SPEED_MAX - 2000.0f;
	}
	else if(Move_state.Imu_Mode == TRUE && Move_state.Top_Mode == FALSE)
	{
		if(Gim_Ctrl.Head_Flag == TRUE)
		{
			speed_zz = 0;
			
			if(abs(Gimb_Motor[GIMB_Y].pid->Imu_Angle_Pid.err) < 3)
	      {
		       Gim_Ctrl.Head_Flag = FALSE;
	      }
		}
		else
		{
			if(Dir_state.Ang45_Mode == TRUE)
			{
				if(Dir_state.Front_Mode == TRUE)
				{
					top_err2 = 4800.0f - Gimb_Motor[GIMB_Y].info->angle;
				}
				else
				{
					top_err2 = 660.0f - Gimb_Motor[GIMB_Y].info->angle;
				}
		    CHASSIS.X = (CHASSIS.Ch2 * buff) * cos(top_err2/8191.0f * 2.0f * 3.1415f) + (CHASSIS.Ch3 * buff) * sin(top_err2/8191.0f * 2.0f * 3.1415f);
		    CHASSIS.Y = (CHASSIS.Ch3 * buff) * cos(top_err2/8191.0f * 2.0f * 3.1415f) - (CHASSIS.Ch2 * buff) * sin(top_err2/8191.0f * 2.0f * 3.1415f);
			}
			if(Gim_Ctrl.Head_Flag == FALSE)
			{
				speed_zz = Speed_zz_Imu_Cal( & Gimb_Motor[GIMB_Y] );
			  speed_zz *= 1.5f;
			}
		}
	}
	else if(Move_state.Imu_Mode == TRUE && Move_state.Top_Mode == TRUE)
	{
		if(Remote_Ctrl.Key_Mode == TRUE)
		{
			switch(F_FLAG%3)
			{
				case 0:
					speed_zz = 1000;
					break;
				case 1:
					speed_zz = 1400;
					break;
				case 2:
					speed_zz = 1800;
					break;
			}
		}
		else
		{
			speed_zz = 1500;
		}
	}
	
  speed_xy[0] = (float)( CHASSIS.X + CHASSIS.Y + speed_zz);
  speed_xy[1] = (float)( CHASSIS.X - CHASSIS.Y + speed_zz);	
  speed_xy[2] = (float)(-CHASSIS.X + CHASSIS.Y + speed_zz);	
  speed_xy[3] = (float)(-CHASSIS.X - CHASSIS.Y + speed_zz);

  speed_max = 0.0f;

  for(int i = 0; i < 4; i++)
	{
		if(abs(speed_xy[i]) > abs(speed_max))
		{
			speed_max = speed_xy[i];
			scale = SPEED_MAX/abs(speed_xy[i]);
		}	
	}
	
	if(abs(scale) >= 1)
	{
		scale = 1;
	}
	
	speed_xy[0]  =  scale * speed_xy[0];
  speed_xy[1]  =  scale * speed_xy[1];	
  speed_xy[2]  =  scale * speed_xy[2];	
  speed_xy[3]  =  scale * speed_xy[3];
	speed_zz =  scale * speed_zz;

  speed[0] = speed_xy[0] + speed_zz; 
  speed[1] = speed_xy[1] + speed_zz; 
  speed[2] = speed_xy[2] + speed_zz; 
  speed[3] = speed_xy[3] + speed_zz; 

  /*-目标值-*/
  Chas_Motor[POWER_0].pid->speed.target = speed[0];          
  Chas_Motor[POWER_1].pid->speed.target = speed[1];
  Chas_Motor[POWER_2].pid->speed.target = speed[2];
  Chas_Motor[POWER_3].pid->speed.target = speed[3];
}

void Chassis_Power_Output(void)
{	
  for(char i = 0 ; i < 4 ; i++)
	{
		Chas_Motor[i].pid->out =
		PID_CAL(&Chas_Motor[i].pid->speed,  NULL, 
						 Chas_Motor[i].info->speed, NULL);
	}
	
	Chassis_Power_Transmit(Chassis_Power_Current);
	
	if(JUDGE_ONLINE)
	{
		Chassis_Motor_Power_Limit(Chassis_Power_Current);
		
  }
	else if(JUDGE_OFFLINE)
	{
		Judge_Offline_Power_Limit(Chassis_Power_Current);
	}
	
	Send_Current(POWER_CAN, POWER_ID, Chassis_Power_Current);
}

void Chassis_Motor_Stop(void)
{
	CHASSIS.Ch2 = 0;
	CHASSIS.Ch3 = 0;
	CHASSIS.Ch0 = 0;	
	
	CHASSIS.V_rate = 0;
	
	Chas_Motor[POWER_0].pid->speed.target = 0;                 
	Chas_Motor[POWER_1].pid->speed.target = 0;
	Chas_Motor[POWER_2].pid->speed.target = 0;
	Chas_Motor[POWER_3].pid->speed.target = 0;

	Chassis_Power_Output();
}

float target_0,target_1,target_2,target_3;
void CHASSIS_CTRL(void)
{
	target_0 = abs(Chas_Motor[POWER_0].pid->speed.target);
	target_1 = abs(Chas_Motor[POWER_1].pid->speed.target);
	target_2 = abs(Chas_Motor[POWER_2].pid->speed.target);
	target_3 = abs(Chas_Motor[POWER_3].pid->speed.target);	

	if(Move_state.Gim_Init_Flg == TRUE)
	{
		Chassis_MEC_Input();		
	  Chassis_Power_Output();
	}
}

