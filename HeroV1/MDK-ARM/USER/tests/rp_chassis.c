/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2022, hwx, China, SZU.
  *                            N0   Rights  Reserved
  *                              
  *                   
  * @FileName   : rp_chassis.c   
  * @Version    : v1.0		
  * @Author     : hwx			
  * @Date       : 2022-11-06         
  * @Description:    
  *
  *
  ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "rp_chassis.h"


/* Exported variables --------------------------------------------------------*/
chassis_base_info_t Chassis_Base_Info;
chassis_work_info_t Chassis_Work_Info;

chassis_t Chassis = {
	
	.base_info = &Chassis_Base_Info,
	.work_info = &Chassis_Work_Info,
	
	.init      = Chassis_Init,
	.work      = Chassis_Work,
	.top_calc  = Chassis_Top_Speed_Calculating,
	
	.config_cycle_div_front = Chassis_Config_cycle_div_front,
	.config_power_limit     = Chassis_Config_power_limit,

};




/* Function  body --------------------------------------------------------*/
/**
  * @Name    Chassic_Init
  * @brief   注册底盘
  * @param   底盘类 底盘初始化结构体
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/

void Chassis_Init(chassis_t *Chassis , Chassis_InitTypeDef *Chassis_Init_structure)
{
	
	
	if(  Chassis_Init_structure->motor_LF           == NULL ||\
		 Chassis_Init_structure->motor_RF           == NULL ||\
		 Chassis_Init_structure->motor_LB           == NULL ||\
		 Chassis_Init_structure->motor_RB           == NULL )
	{
		return;
	}
	
	if( Chassis_Init_structure->power_limit_buffer  == NULL)
    {
		Chassis_Config_power_limit(POWER_LIMIT_OFF);
    }
 	
	/*注册四个电机*/
	Chassis->motor_LB = Chassis_Init_structure->motor_LB;
	Chassis->motor_LF = Chassis_Init_structure->motor_LF;
	Chassis->motor_RF = Chassis_Init_structure->motor_RF;
	Chassis->motor_RB = Chassis_Init_structure->motor_RB;
	
	/*裁判系统功率缓冲区*/
	Chassis->work_info->chassis_limit_buffer = Chassis_Init_structure->power_limit_buffer;
	
	Chassis->work_info->chassis_output_max        = CHASSIS_SPEED_MAX;
	Chassis->work_info->chassis_speed_max         = CHASSIS_OUTPUT_MAX;
	Chassis->work_info->chassis_power_limit_state = POWER_LIMIT_ON;
	Chassis->work_info->chassis_cycle_div_front   = CHASSIS_CYCLE_DIV_FRONT;

}

/**
  * @Name    Chassis_Power_Limit
  * @brief   底盘功率限制(经典祖传算法)
  * @param   底盘 
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Power_Limit(chassis_t * chassis)
{
	if(POWER_LIMIT_OFF){
		return;
	}
	
	
	float buffer = *Chassis.work_info->chassis_limit_buffer;
	float heat_rate,Limit_k, CHAS_LimitOutput, CHAS_TotalOutput;
	
	uint16_t OUT_MAX = 0;
	
	int16_t limit_output_current[4];
	
	limit_output_current[0] = chassis->base_info->motor_LF_ouput_current;
	limit_output_current[1] = chassis->base_info->motor_RF_ouput_current;
	limit_output_current[2] = chassis->base_info->motor_LB_ouput_current;
	limit_output_current[3] = chassis->base_info->motor_RB_ouput_current;
	
	
	OUT_MAX = CHASSIS_SPEED_MAX * 4;
	
	if(buffer > 60)buffer = 60;//防止飞坡之后缓冲250J变为正增益系数
	
	Limit_k = buffer / 60;
	
	if(buffer < 25)
		Limit_k = Limit_k * Limit_k ;// * Limit_k; //3方
	else
		Limit_k = Limit_k;// * str->Limit_k; //平方
	
	if(buffer < 60)
		CHAS_LimitOutput = Limit_k * OUT_MAX;
	else 
		CHAS_LimitOutput = OUT_MAX;    
	
	CHAS_TotalOutput = c_abs(limit_output_current[0]) + c_abs(limit_output_current[1]) + c_abs(limit_output_current[2]) + c_abs(limit_output_current[3]) ;
	
	heat_rate = CHAS_LimitOutput / CHAS_TotalOutput;
	
  if(CHAS_TotalOutput >= CHAS_LimitOutput)
  {
		for(char i = 0 ; i < 4 ; i++)
		{	
			limit_output_current[i] = (int16_t)(limit_output_current[i] * heat_rate);	
		}
	}
  
    /*重新赋值*/
  	chassis->base_info->motor_LF_ouput_current = limit_output_current[0];
	chassis->base_info->motor_RF_ouput_current = limit_output_current[1];
	chassis->base_info->motor_LB_ouput_current = limit_output_current[2];
	chassis->base_info->motor_RB_ouput_current = limit_output_current[3];
}


/**
  * @Name    Chassis_Config_power_limit
  * @brief   开关底盘功率限制（进入自杀模式）
  * @param   off_or_on
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Config_power_limit(uint8_t off_or_on)
{
	if(off_or_on == POWER_LIMIT_OFF || off_or_on == POWER_LIMIT_ON)
	{
		Chassis.work_info->chassis_power_limit_state = off_or_on;
	}

}

/**
  * @Name    Chassis_Config_cycle_div_front
  * @brief   修改旋转量比重
  * @param   cycle_div_front ( 0 ~ 100)
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Config_cycle_div_front(uint8_t cycle_div_front)
{
	if(cycle_div_front >= 0 || cycle_div_front <= 100)
	{
		Chassis.work_info->chassis_cycle_div_front = cycle_div_front;
	}
}

/**
  * @Name    Chassis_Speed_Limit
  * @brief   速度限制（等比例缩小）
  * @param   底盘 三个方向速度
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Speed_Limit(chassis_t *chassis, int16_t *front, int16_t *right, int16_t *cycle)
{
	int16_t speed_sum;
	float RATE;
	speed_sum = c_abs(chassis->base_info->target_front_speed) + \
	            c_abs(chassis->base_info->target_right_speed) + \
                c_abs(chassis->base_info->target_cycle_speed);
	
	if(speed_sum > chassis->work_info->chassis_speed_max)
	{
		RATE = (float)chassis->work_info->chassis_speed_max / (float)speed_sum;
	}
	else 
	{
		RATE = 1;
	}
	
	*front = chassis->base_info->target_front_speed * RATE;
	*right = chassis->base_info->target_right_speed * RATE;
	*cycle = chassis->base_info->target_cycle_speed * RATE;
	
	chassis->base_info->target_front_speed = (float)chassis->base_info->target_front_speed * RATE;
	chassis->base_info->target_right_speed = (float)chassis->base_info->target_right_speed * RATE;
	chassis->base_info->target_cycle_speed = (float)chassis->base_info->target_cycle_speed * RATE;
}


/**
  * @Name    Chassis_Speed_Calculating
  * @brief   底盘速度解算（速度分解）
  * @param   三个方向速度
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Speed_Calculating(chassis_t *chassis, int16_t front, int16_t right, int16_t cycle)
{
	
	chassis->base_info->motor_LF_ouput_speed   = (  front + right + cycle);
	chassis->base_info->motor_RF_ouput_speed   = (- front + right + cycle);
	chassis->base_info->motor_LB_ouput_speed   = (  front - right + cycle);
	chassis->base_info->motor_RB_ouput_speed   = (- front - right + cycle);
}





/**
  * @Name    Chassis_Top_Speed_Calculating
  * @brief   底盘小陀螺速度解算(将这个函数放在Work前)
  * @param   底盘 三个方向速度 差角（请换算成弧度制，并且注意数据类型是float）
  * @retval 
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Top_Speed_Calculating(chassis_t *chassis,float detal_angle, int16_t *front, int16_t *right)
{
	  float sin_x = (float)sin((double)(detal_angle));
	  float cos_x = (float)cos((double)(detal_angle));
	  int16_t tem_front =  (*front);
	  int16_t tem_right =  (*right);
	
	 /*警钟长鸣！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
	 *front =  (int16_t)(cos_x * tem_front)-(sin_x * tem_right);
	 *right =  (int16_t)(sin_x * tem_front)+(cos_x * tem_right);
}


/**
  * @Name    Chassis_Pid_Calculating
  * @brief   使用到底盘包
  * @param   底盘 
  * @retval 
  * @author  HWX
  * @Date    2022-11-07
**/
void Chassis_Pid_Calculating(chassis_t *chassis)
{
	
	chassis->base_info->motor_LF_ouput_current = chassis->motor_LF->c_speed(chassis->motor_LF ,chassis->base_info->motor_LF_ouput_speed);
	chassis->base_info->motor_RF_ouput_current = chassis->motor_RF->c_speed(chassis->motor_RF ,chassis->base_info->motor_RF_ouput_speed);
	chassis->base_info->motor_LB_ouput_current = chassis->motor_LB->c_speed(chassis->motor_LB ,chassis->base_info->motor_LB_ouput_speed);
	chassis->base_info->motor_RB_ouput_current = chassis->motor_RB->c_speed(chassis->motor_RB ,chassis->base_info->motor_RB_ouput_speed);

}
/**
  * @Name    Chassis_Work
  * @brief   底盘核心函数
  * @param   底盘 三个方向速度
  * @retval
  * @author  HWX
  * @Date    2022-11-06
**/
void Chassis_Work(chassis_t *chassis , int16_t front , int16_t right , int16_t cycle)
{
	int16_t output_current[4];
	
	/*底盘速度限制*/
	Chassis_Speed_Limit(chassis, &front, &right, &cycle);
	
	
	/*底盘速度解算*/
	Chassis_Speed_Calculating(chassis, front, right, cycle);
	
	
	/*PID计算*/
	Chassis_Pid_Calculating(chassis);
	
	/*旋转量算法*/
	
	
	/*裁判系统速度限制*/
	Chassis_Power_Limit(chassis);
	
	

	output_current[0] = chassis->base_info->motor_LF_ouput_current;
	output_current[1] = chassis->base_info->motor_RF_ouput_current;
	output_current[2] = chassis->base_info->motor_LB_ouput_current;
	output_current[3] = chassis->base_info->motor_RB_ouput_current;	
	

	/*发送电流*/
//	CAN_SendData(&CHASSIS_DRV_CAN_USE,CHASSIS_CAN_STD_ID,output_current);
  CAN1_Send_With_int16_to_uint8(CHASSIS_CAN_STD_ID, output_current);
}



