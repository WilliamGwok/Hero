#include "Shooting.h"
#include "rc.h"
#include "couple.h"
#include "judge_sensor.h"

#define Fric_Speed0    0
#define Fric_Speed15   4400
#define Fric_Speed18   4700
#define Fric_Speed22   5200
#define Fric_Speed30   7020

int16_t	Shoot_Current[4] = {0,0,0,0};
int16_t Shoot_Stop_Cmd[4] = {0,0,0,0};

void Shoot_Motor_Current(motor_t *shoot,int16_t *data)
{
	if(RC_OFFLINE)
		data[shoot->driver->drv_id] = 0;
	else 
		data[shoot->driver->drv_id] = shoot->pid->out;
}

void Angle_Process(motor_t * motor)
{
	int err = 0;
	
	err = motor->info->angle - motor->info->last_angle;
	
	if(abs(err)>4095)
	{
		if(err >= 0)
		{
			motor->info->total_angle += (err - 8191);
		}
		else
		{
			motor->info->total_angle += err + 8191;
		}
	}
	else
	{
		motor->info->total_angle += err;
	}
	
	motor->info->last_angle = motor->info->angle;
}

/*.....................摩擦轮射速控制与热量控制.....................*/
//15 18 22 30
uint16_t Friction_Speed_Limit(void)
{
	uint16_t speed_limit = 0,fric_tar = 0;
	
	
	speed_limit = judge_info.game_robot_status.shooter_id1_17mm_speed_limit;
	
	switch(speed_limit)
	{
		case 0:
			fric_tar = Fric_Speed0;
		  break;
		case 15:
			fric_tar = Fric_Speed15;
		  break;
		case 18:
			fric_tar = Fric_Speed18;
		  break;
		case 22:
			fric_tar = Fric_Speed22;
		  break;
		case 30:
			fric_tar = Fric_Speed30;
		  break;
		default:
			if(speed_limit < 40 && speed_limit)
				fric_tar = Fric_Speed15 + (speed_limit - 15) * 110;
			else
				fric_tar = Fric_Speed30;
			break;
	}
	
	return fric_tar;
}

/*.....................摩擦轮控制.....................*/

void Friction_Init(void)
{
	Shoot_state.Friction_Mode = FALSE;
	Shoot_state.Fric_ok_flg = FALSE;
}

void Friction_Output(motor_t *fric)
{
	uint16_t tar = 0;
	
	tar = Friction_Speed_Limit();
	
	motor_pid_t  *pid  = fric->pid;	
	motor_info_t *info = fric->info;
	
	if(fric->id==DEV_ID_FRIC_L)
		pid->Friction_Pid.target = (-tar);
	else if(fric->id==DEV_ID_FRIC_R)
	  pid->Friction_Pid.target = (tar);
	
	if(Shoot_state.Friction_Mode == FALSE)
	{
		pid->Friction_Pid.target = 0;
	}
	
	err_calculation(&pid->Friction_Pid,info->speed);
	single_pid_ctrl(&pid->Friction_Pid);
	
	pid->out = pid->Friction_Pid.out;
	
	if(Shoot_state.Friction_Heat_OK == FALSE)//可以考虑让feed先停转，再使摩擦轮停，这样会防止卡弹
	{
		pid->out = 0;
	}
	
	Shoot_Motor_Current(fric,Shoot_Current);
}

void Friction_Ctrl(motor_t *shoot)
{
	Friction_Output(shoot);
}





/*.....................堵转处理.....................*/
char fine = 0;
void Jam_Process(motor_t *feed)
{
	motor_pid_t  *pid  = feed->pid;	
	motor_info_t *info = feed->info;
	
	if((fine == 0) && (pid->Feed_Speed_Pid.target < 0 | pid->Feed_Single_Angle_Pid.target < 0)) 
	{
		feed->info->total_angle = 0;//是不是傻，这里每次清零那totalangle就一直为零，无法累加

		pid->Feed_Single_Angle_Pid.target = 36859.0f;
		
		fine = 1;
	}
	else if((fine == 0) && (pid->Feed_Single_Angle_Pid.target > 0))
	{
		feed->info->total_angle = 0;
		
		pid->Feed_Single_Angle_Pid.target = -36859.0f;
		
		fine = 1;
	}
	
	 err_calculation(&pid->Feed_Single_Angle_Pid,info->total_angle);
	 single_pid_ctrl(&pid->Feed_Single_Angle_Pid);
	
	 pid->Feed_Single_Speed_Pid.target = pid->Feed_Single_Angle_Pid.out;
	
	 err_calculation(&pid->Feed_Single_Speed_Pid,info->speed);
	 single_pid_ctrl(&pid->Feed_Single_Speed_Pid);

	 pid->out = pid->Feed_Single_Speed_Pid.out;

	
	if(abs(pid->Feed_Single_Angle_Pid.err) <= 10)
	{
		pid->Feed_Single_Angle_Pid.target = 0;
		
    feed->info->total_angle = 0;
		
		fine = 0;
		
		Shoot_state.Jam_State = FALSE;
	}
	
	Shoot_Motor_Current(feed,Shoot_Current);
}






/*.....................2006控制.....................*/

void Keep_Shooting_Output(motor_t *feed)
{
	motor_pid_t  *pid  = feed->pid;	
	motor_info_t *info = feed->info;
	//顺时针转要给负电流
	pid->Feed_Speed_Pid.target = -3500;
	
	if(Shoot_state.Friction_Heat_OK == FALSE)
	{
		pid->Feed_Speed_Pid.target = 0;
	}
	
	err_calculation(&pid->Feed_Speed_Pid,info->speed);
	single_pid_ctrl(&pid->Feed_Speed_Pid);
	
	pid->out = pid->Feed_Speed_Pid.out;
	
	Shoot_Motor_Current(feed,Shoot_Current);
}


void Single_Shooting_Output(motor_t *feed)
{
	motor_pid_t  *pid  = feed->pid;	
	motor_info_t *info = feed->info;
	
	//处理目标值
	if(single_nowstate != single_prestate)
	{
		pid->Feed_Single_Angle_Pid.target = -36859;
		
		single_prestate = 1;
	}
	
	err_calculation(&pid->Feed_Single_Angle_Pid,info->total_angle);
	single_pid_ctrl(&pid->Feed_Single_Angle_Pid);
	
	pid->Feed_Single_Speed_Pid.target = pid->Feed_Single_Angle_Pid.out;
	
	err_calculation(&pid->Feed_Single_Speed_Pid,info->speed);
	single_pid_ctrl(&pid->Feed_Single_Speed_Pid);

	pid->out = pid->Feed_Single_Speed_Pid.out;
	
	if(abs(pid->Feed_Single_Angle_Pid.err) <= 50)
	{
		pid->Feed_Single_Angle_Pid.target = 0;
    feed->info->total_angle = 0;
	}
	
	Shoot_Motor_Current(feed,Shoot_Current);
}


/*.....................舵机控制.....................*/
void Servo_Ctrl(void)
{
	if(Shoot_state.Servo_State == TRUE)
		Servo_Out(180);
	else if(Shoot_state.Servo_State == FALSE)
		Servo_Out(250);
}





/*.....................发射机构总控制.....................*/

void Shooting_Ctrl(void)
{
	
	if(Move_state.Mec_Mode == TRUE && rc.work_state == 0)
	{
		Servo_Ctrl();
	}
	else if(Move_state.Mec_Mode == FALSE && rc.work_state == 0)
	{
		Servo_Wheel_Ctrl = 0;
		
		Servo_Out(250);
	}
	else if(rc.work_state == 1)
	{
		Servo_Out(0);
	}
	
	if(Shoot_state.Friction_Mode == TRUE)
	{
		Friction_Ctrl( &Shot_Motor[Left_Fric] );
		Friction_Ctrl( &Shot_Motor[Right_Fric] );
	}
	else
	{
		Friction_Ctrl( &Shot_Motor[Left_Fric] );
		Friction_Ctrl( &Shot_Motor[Right_Fric] );
	  if(Shoot_state.Fric_ok_flg == FALSE)
		Send_Current(SHOOT_CAN , SHOOT_ID , Shoot_Stop_Cmd);
	}
	
	if(Shoot_state.Jam_State == FALSE)
	{
		if(Shoot_state.Feed_Keep_Mode == TRUE && Shoot_state.Fric_ok_flg == TRUE && Shoot_state.Friction_Mode == TRUE)
		  Keep_Shooting_Output( &Shot_Motor[Feed] );

	  if(Shoot_state.Feed_Single_Mode == TRUE && Shoot_state.Fric_ok_flg == TRUE && Shoot_state.Friction_Mode == TRUE)
		  Single_Shooting_Output( &Shot_Motor[Feed] );
	}
	else
	{
		Jam_Process(&Shot_Motor[Feed]);
	}
	
	if(RC_ONLINE)
	{
		Send_Current(SHOOT_CAN , SHOOT_ID , Shoot_Current);
	}
	else
	{
    Friction_Init();
		Send_Current(SHOOT_CAN , SHOOT_ID , Shoot_Stop_Cmd);
	}
}

