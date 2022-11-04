#include "S_function.h"
#include "rc.h"

/*.....................电机过零点判断兵处理.....................*/
void Zero_Point_Judge(pid_ctrl_t *out)
{
	float err = 0.0f;
	err = out->err;
	if(Move_state.Mec_Mode || Gim_Ctrl.Com_Ctrl == TRUE)
	{
		  if(abs(err) > 4095)
	  {
		  if(err < 0) out->err += 8191.0f;
	  	if(err > 0) out->err -= 8191.0f;
  	}
	}
	else if(Move_state.Imu_Mode )
	{
		if(out->pid_flag == FOR_CHAS_PID_FLAG)
		{
			if(abs(err) > 4095)
	    {
		    if(err < 0) out->err += 8191.0f;
	  	  if(err > 0) out->err -= 8191.0f;
		  }
  	}
		else if(out->pid_flag == Pitch_PID_FLAG | out->pid_flag == YAW_PID_FLAG)
		if(abs(err) > 180)
		{
			if(err < 0) out->err += 360.0f;
			if(err > 0)	out->err -= 360.0f;		
		}
	}
}

/*.....................PID单双环计算.....................*/

float PID_CAL(pid_ctrl_t *out, pid_ctrl_t *inn, float meas1, float meas2)
{
	if(inn == NULL)
	{
		err_calculation(out , meas1);
		
    if( abs(out->err) <= (out->blind_err) ) out->err=0;		
		
		single_pid_ctrl(out);

		return out->out;	
	}
	else 
	{
		err_calculation(out , meas1);	
		
		Zero_Point_Judge(out);
		
		single_pid_ctrl(out);
		
		inn->target = out->out;	
		

		err_calculation(inn , meas2);

	  if( abs(inn->err) <= (inn->blind_err) ) inn->err=0;	
		
		single_pid_ctrl(inn);	
		
		return inn->out;	
	}
}


/*.....................滑动均值滤波.....................*/
Mean_Filter_t Mean_Filter_Yaw_Init;
Mean_Filter_t Mean_Filter_Yaw;
Mean_Filter_t Mean_Filter_Pitch;
Mean_Filter_t Mean_Filter_Yaw_Mec;
Mean_Filter_t Mean_Filter_Pitch_Mec;

void Mean_Filter_Init(Mean_Filter_t * filter)
{
	filter->len = FILTER_LEN;
	filter->cnt = 0.0f;
	filter->index = 0;
	for(int i = 0;i < filter->len;i++)
	{
		filter->SPEED[i] = 0.0f;
	}
}

void Mean_Filter_Input(Mean_Filter_t * filter,float mea_speed)
{
  filter->index %= 10;
	
	filter->SPEED[filter->index] = mea_speed;
	
	filter->index++;
	
	filter->cnt ++;
}

float Mean_Filter_Output(Mean_Filter_t * filter,float mea_speed)
{
	float sum = 0,speed = 0;
	
	if(filter->cnt < FILTER_LEN)
	{
		for(int i = 0;i < filter->cnt;i++)
		{
			sum += filter->SPEED[i];
		}
		if(filter->cnt != 0)
		{
			speed = sum / filter->cnt;
		}
		else
		{
			speed = mea_speed;
		}
	}
	else
	{
		for(int j = 0;j < FILTER_LEN;j++)
		{
			sum += filter->SPEED[j];
			
			speed = sum / FILTER_LEN;
		}
	}
	return speed;                  
}

float Mean_Filter_Pro(Mean_Filter_t * filter,float mea_speed)
{
	float speed_process = 0;
	
	Mean_Filter_Input(filter,mea_speed);
	
	speed_process = Mean_Filter_Output(filter,mea_speed);
	
	return speed_process;
}


/*.....................遥控器算法.....................*/
float SF(float t,float *slopeFilter,float res)
{
  for(int i = SF_LENGTH-1;i>0;i--)
  {
    slopeFilter[i] = slopeFilter[i-1];
  }slopeFilter[0] = t;
  for(int i = 0;i<SF_LENGTH;i++)
  {
    res += slopeFilter[i];
  }
	return (res/SF_LENGTH);
}




