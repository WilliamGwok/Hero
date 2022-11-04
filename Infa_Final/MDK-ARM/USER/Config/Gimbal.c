#include "Gimbal.h"
#include "rc.h"
#include "imu_sensor.h"
#include "vision_sensor.h"

int16_t	Gimb_Current[4] = {0,0,0,0};
int16_t Gimb_Stop_Cmd[4] = {0,0,0,0};

float yaw_mec_final_tar = 4800;
float pitch_mec_final_tar = 6861;
float yaw_imu_final_tar = 0;
float pitch_imu_final_tar = 0;


#define   YAW_MEC_Final_Tar            yaw_mec_final_tar
#define   PITCH_MEC_Final_Tar          pitch_mec_final_tar
#define   YAW_IMU_Final_Tar            yaw_imu_final_tar
#define   PITCH_IMU_Final_Tar          pitch_imu_final_tar
float a,b;

void Gimbal_Motor_Current(motor_t *gimbal,int16_t *data)
{
	if(RC_OFFLINE)
		data[gimbal->driver->drv_id] = 0;
	else 
		data[gimbal->driver->drv_id] = gimbal->pid->out;
}

void GIMBAL_YAW_Mec_INIT(void)
{
	YAW_MEC_Final_Tar = 4800;
}

void GIMBAL_PITCH_Mec_INIT(void)
{
	PITCH_MEC_Final_Tar = 6861;
}

void GIMBAL_YAW_Imu_INIT(void)
{
	YAW_IMU_Final_Tar = imu_sensor.info->yaw;
}

void GIMBAL_PITCH_Imu_INIT(void)
{
	PITCH_IMU_Final_Tar = imu_sensor.info->pitch;
}


/*.....................云台初始化.....................*/
char gimbal_yaw_init = 0;
char gimbal_pitch_init = 0;

void Yaw_Gimbal_Init(motor_t *gimbal)
{
	
	motor_pid_t  *pid  = gimbal->pid;	
	motor_info_t *info = gimbal->info;

	pid->Mec_Angle_Pid.target = 4800;
	
	pid->out = PID_CAL(&pid->Mec_Angle_Pid,
	          				 &pid->Mec_Speed_Pid,
	          				 info->angle, YAW_V);
	
	YAW_IMU_Final_Tar = imu_sensor.info->yaw;//模式切换处理
	
	if(abs(pid->Mec_Angle_Pid.err) == 0)
	{
		Move_state.Gim_Init_Flg = TRUE;
		gimbal_yaw_init++;
	}
	
	Gimbal_Motor_Current(gimbal, Gimb_Current);	
}

void Pitch_Gimbal_Init(motor_t *gimbal)
{
	motor_pid_t  *pid  = gimbal->pid;	
	motor_info_t *info = gimbal->info;

	pid->Mec_Angle_Pid.target = 6861;
	
	pid->out = PID_CAL(&pid->Mec_Angle_Pid,
	          				 &pid->Mec_Speed_Pid,
	          				 info->angle, PITCH_V);
	
	YAW_IMU_Final_Tar = imu_sensor.info->yaw;//模式切换处理
	
	if(abs(pid->Imu_Angle_Pid.err) == 0)
	{
		Move_state.Gim_Init_Flg = TRUE;
		gimbal_pitch_init++;
	}
	
	Gimbal_Motor_Current(gimbal, Gimb_Current);	
}

void Gimbal_Init(void)
{
	Yaw_Gimbal_Init( & Gimb_Motor[GIMB_Y] );
//	if(gimbal_yaw_init > gimbal_pitch_init)
//	{
//		Pitch_Gimbal_Init( & Gimb_Motor[GIMB_P] );
//	}
}

/*.....................机械模式.....................*/

//void Head_Change(motor_t *gimbal)
//{
//	motor_pid_t  *pid  = gimbal->pid;	
//	motor_info_t *info = gimbal->info;
//	
//	if(YAW_MEC_Final_Tar == 4800)
//	{
//		YAW_MEC_Final_Tar -= 4095;
//	}
//	else
//	{
//		YAW_MEC_Final_Tar = 4800;
//	}
//	
//	if(abs(pid->Mec_Angle_Pid.err) == 0)
//	{
//		Gim_Ctrl.Head_Flag = FALSE;
//		
//		YAW_IMU_Final_Tar = imu_sensor.info->yaw;
//	}
//}

float Gimb_Yaw_Mec_input(void)
{
	float rcdata = 0.0f;	

	float rate=0.01;

	rcdata = 0;//啊这个函数我写来干什么的  10/13/2022
	
	YAW_MEC_Final_Tar += rcdata * rate;
		
	if(YAW_MEC_Final_Tar>8191.0f) YAW_MEC_Final_Tar -= 8191.0f;
	if(YAW_MEC_Final_Tar<0.0f) YAW_MEC_Final_Tar += 8191.0f;
	
	return YAW_MEC_Final_Tar;
}

void Gimbal_Yaw_MEC_Output(motor_t *gimbal ,float tar)
{
	motor_pid_t  *pid  = gimbal->pid;	
	motor_info_t *info = gimbal->info;

	pid->Mec_Angle_Pid.target = tar;
	
	pid->out = PID_CAL(&pid->Mec_Angle_Pid,
	          				 &pid->Mec_Speed_Pid,
	          				 info->angle, YAW_V);
	
	YAW_IMU_Final_Tar = imu_sensor.info->yaw;//模式切换处理
	
	Gimbal_Motor_Current(gimbal, Gimb_Current);	
}

void Gimbal_Yaw_MEC_Ctrl(motor_t *gimbal)
{
	float tar;
	tar = Gimb_Yaw_Mec_input();
	Gimbal_Yaw_MEC_Output(gimbal , tar);
}

float Gimb_Pitch_Mec_input(void)
{
	float rcdata = 0.0f;	
	float rate=0.008;
	
	if(Remote_Ctrl.RC_Mode == TRUE)
	{
		rcdata = (-CH1_VALUE);
	}
  else
	{
		rcdata = (-CH1_VALUE_K);
	}
	
	PITCH_MEC_Final_Tar += rcdata* rate ;
	
	PITCH_MEC_Final_Tar = constrain(PITCH_MEC_Final_Tar,5880,7280);
	
	PITCH_IMU_Final_Tar = imu_sensor.info->pitch;
	
	return PITCH_MEC_Final_Tar;
}

void Gimbal_Pitch_MEC_Output(motor_t *gimbal ,float tar)
{
	motor_pid_t  *pid  = gimbal->pid;	
	motor_info_t *info = gimbal->info;

	pid->Mec_Angle_Pid.target = tar;
	
	pid->out = PID_CAL(&pid->Mec_Angle_Pid,
	          				 &pid->Mec_Speed_Pid,
	          				 info->angle, PITCH_V);

	Gimbal_Motor_Current(gimbal, Gimb_Current);	
}

void Gimbal_Pitch_MEC_Ctrl(motor_t *gimbal)
{
	float tar;
	tar = Gimb_Pitch_Mec_input();
	Gimbal_Pitch_MEC_Output(gimbal , tar);
}






/*.....................陀螺仪模式.....................*/

float Gimb_Yaw_Imu_input(void)
{
	float rcdata = 0.0f;	

	float rate=0.0006;
	
	if(Remote_Ctrl.RC_Mode == TRUE)rcdata = - CH0_VALUE;
	else rcdata = -CH0_VALUE_K;
	
	if(Gim_Ctrl.Head_Flag == FALSE)
	{
		YAW_IMU_Final_Tar += rcdata * rate;
	}
		
	if(YAW_IMU_Final_Tar>180.0f) YAW_IMU_Final_Tar -= 360.0f;
	if(YAW_IMU_Final_Tar<-180.0f) YAW_IMU_Final_Tar += 360.0f;
	
	
	return YAW_IMU_Final_Tar;
}

void Gimbal_Yaw_Imu_Output(motor_t *gimbal ,float tar)
{
	a=value_yaw * cos((imu_sensor.info->pitch)/180 * 3.1415);
	b =cos((imu_sensor.info->pitch)/180 * 3.1415);
	motor_pid_t  *pid  = gimbal->pid;	

	pid->Imu_Angle_Pid.target = tar;

	pid->out = PID_CAL(&pid->Imu_Angle_Pid,
	          				 &pid->Imu_Speed_Pid,
	          				 imu_sensor.info->yaw, YAW_V);
	
	Gimbal_Motor_Current(gimbal, Gimb_Current);	
}

void Gimbal_Yaw_IMU_Ctrl(motor_t *gimbal)
{
	float tar;
	
	tar = Gimb_Yaw_Imu_input();
	
	Gimbal_Yaw_Imu_Output(gimbal , tar);
}

float Gimb_Pitch_Imu_input(void)
{
	float rcdata = 0.0f;	

	float rate=0.0006;
	
	if(Remote_Ctrl.RC_Mode == TRUE)rcdata = - CH1_VALUE;
	else rcdata = -CH1_VALUE_K;
	
	PITCH_IMU_Final_Tar += rcdata * rate;
	
	PITCH_IMU_Final_Tar = constrain(PITCH_IMU_Final_Tar,-41,25);
	
	return PITCH_IMU_Final_Tar;
}

void Gimbal_Pitch_Imu_Output(motor_t *gimbal ,float tar)
{
	motor_pid_t  *pid  = gimbal->pid;	

	pid->Imu_Angle_Pid.target = tar;
	
	pid->out = PID_CAL(&pid->Imu_Angle_Pid,
	          				 &pid->Imu_Speed_Pid,
	          				 imu_sensor.info->pitch, PITCH_V);

	PITCH_MEC_Final_Tar = gimbal->info->angle;//模式切换处理
	
	Gimbal_Motor_Current(gimbal, Gimb_Current);	
}

void Gimbal_Pitch_Imu_Ctrl(motor_t *gimbal)
{
	float tar;
	tar = Gimb_Pitch_Imu_input();
	Gimbal_Pitch_Imu_Output(gimbal , tar);
}




//遥控器数值为0时才会进入自瞄，这个还没写
/*.....................电脑寄生模式.....................*/

extern Vision_Cmd_Id_t AUTO_CMD_MODE;

float Yaw_Tar_Process(float tar)
{
	if(tar >= 0 && tar < 4095)
	{
		tar = (tar/8191) * (-360.0f);
	}
	else
	{
		tar = 360.0f - (tar/8191.0f) * 360.0f;
	}
	
	return tar;
}

float Pitch_Tar_Process(float tar)
{
	tar = (tar - 4096)/8191.0f * 360.0f;
	
	return tar;
}

float Gimbal_Yaw_Com_Input(void)
{
	float tar = 0;
	
	float rcdata = 0.0f;	

	float rate=0.0006;
	
	rcdata = - CH0_VALUE;
	
	tar = Yaw_Tar_Process( vision_sensor.info->RxPack.RxData.yaw_angle );
	
	tar += rcdata * rate;
	
	a = tar;
	
	if(tar>180.0f) tar -= 360.0f;
	if(tar<-180.0f) tar += 360.0f;
	
	return tar;
}

void Gimbal_Yaw_Com_Output(motor_t *gimbal,float tar)
{
	motor_pid_t *pid = gimbal->pid;
	
	pid->Imu_Angle_Pid.target = tar;
	
	pid->out = PID_CAL(&pid->Imu_Angle_Pid,
	                   &pid->Imu_Speed_Pid,
	                   imu_sensor.info->yaw,YAW_V);
	
	Gimbal_Motor_Current(gimbal,Gimb_Current);
}

float Gimbal_Pitch_Com_Input(void)
{
	float tar = 0;
	
	tar = Pitch_Tar_Process(vision_sensor.info->RxPack.RxData.pitch_angle);
	
	return tar;
}

void Gimbal_Pitch_Com_Output(motor_t *gimbal,float tar)
{
	motor_pid_t *pid = gimbal->pid;
//	motor_info_t *info = gimbal->info;
	
	pid->Imu_Angle_Pid.target = tar;
	
	pid->out = PID_CAL(&pid->Imu_Angle_Pid,
	                   &pid->Imu_Speed_Pid,
	                   imu_sensor.info->pitch,PITCH_V);
	
	Gimbal_Motor_Current(gimbal,Gimb_Current);
}

void Gimbal_Yaw_Com_Ctrl(motor_t *gimbal)
{
	float tar;
	
	tar = Gimbal_Yaw_Com_Input();
	
	Gimbal_Yaw_Com_Output(gimbal,tar);
}

void Gimbal_Pitch_Com_Ctrl(motor_t *gimbal)
{
	float tar;
	
	tar = Gimbal_Pitch_Com_Input();
	
	Gimbal_Pitch_Com_Output(gimbal,tar);
}

/*.....................模式转换处理.....................*/

//void Change_To_Imu_Mode_Init(char* target_change)
//{
//	for(char i = 0; i < *target_change; i++)
//	{
//		YAW_IMU_Final_Tar = imu_sensor.info->yaw;
//		PITCH_IMU_Final_Tar = imu_sensor.info->pitch;
//		target_change--;
//	}
//}


//为什么我可以想出这么笨的方法





/*.....................云台总控制.....................*/

void GIMBAL_CTRL(void)
{
//	if(Gim_Ctrl.Head_Flag == TRUE)
//	{
//		Head_Change(& Gimb_Motor[GIMB_Y]);
//	}
	
	if(Move_state.Gim_Init_Flg == FALSE)
	{
		Gimbal_Init();
	}
	else
	{
		if(Move_state.Mec_Mode)
	  {
	  	Gimbal_Yaw_MEC_Ctrl( & Gimb_Motor[GIMB_Y] );
	    Gimbal_Pitch_MEC_Ctrl( & Gimb_Motor[GIMB_P] );
	  }
	  else if(Move_state.Imu_Mode)
	  {
	  	Gimbal_Yaw_IMU_Ctrl( & Gimb_Motor[GIMB_Y] );
		  Gimbal_Pitch_Imu_Ctrl( & Gimb_Motor[GIMB_P] );
	  }
    else if(Gim_Ctrl.Com_Ctrl == TRUE)
	  {
		  Gimbal_Yaw_Com_Ctrl( & Gimb_Motor[GIMB_Y] );
		  Gimbal_Pitch_Com_Ctrl( & Gimb_Motor[GIMB_P] );
	  }
	}
	
	if(RC_ONLINE)
	{
		Send_Current(GIMBA_CAN, GIMBA_ID, Gimb_Current);
	}
	else
	{
		Move_state.Gim_Init_Flg = FALSE;
		Send_Current(GIMBA_CAN, GIMBA_ID, Gimb_Stop_Cmd);
	}
}











