#include "Motor.h"
#include "main.h"
#include "rp_driver.h"
#include "can_drv.h"
#include "Shoot_motor.h"
#include "Shooting.h"

//motor_info_t motor_info;
//motor_t MOTOR = {
// .info = &motor_info,
//};


void motor_check(motor_t *motor);
void motor_heart_beat(motor_t *motor);
/*  电机总和  */
extern motor_t Chas_Motor[CHAS_MOTOR_CNT];
extern motor_t Gimb_Motor[GIMB_MOTOR_CNT];
extern motor_t Shot_Motor[SHOT_MOTOR_CNT];

motor_combination_t MOTOR = {
	.chas[POWER_0] = &Chas_Motor[POWER_0],
	.chas[POWER_1] = &Chas_Motor[POWER_1],
	.chas[POWER_2] = &Chas_Motor[POWER_2],	
	.chas[POWER_3] = &Chas_Motor[POWER_3],	

	.gimb[GIMB_Y] = &Gimb_Motor[GIMB_Y],
	.gimb[GIMB_P] = &Gimb_Motor[GIMB_P],
	
	.shot[Left_Fric] = &Shot_Motor[Left_Fric],
	.shot[Right_Fric] = &Shot_Motor[Right_Fric],
	.shot[Feed]    = &Shot_Motor[Feed],
	.shot[Cover] = &Shot_Motor[Cover],	
	
	.init       = motor_init,
	.update     = motor_update,
//	.check      = motor_check,
	.heart_beat = motor_heart_beat,
};

void Init_Motor(void)
{	
	MOTOR.init(&Chas_Motor[POWER_0]);
	MOTOR.init(&Chas_Motor[POWER_1]);
	MOTOR.init(&Chas_Motor[POWER_2]);
	MOTOR.init(&Chas_Motor[POWER_3]);
	
	MOTOR.init(&Gimb_Motor[GIMB_Y]);
	MOTOR.init(&Gimb_Motor[GIMB_P]);
	
	MOTOR.init(&Shot_Motor[Left_Fric]);
	MOTOR.init(&Shot_Motor[Right_Fric]);
	MOTOR.init(&Shot_Motor[Feed]);
	MOTOR.init(&Shot_Motor[Cover]);
	
}

/**
 *	@brief	获取电机角度位置
 */
static uint16_t CAN_GetMotorAngle(uint8_t *rxData)
{
	uint16_t angle;
	angle = ((uint16_t)rxData[0] << 8| rxData[1]);
	return angle;
}

/**
 *	@brief	获取电机转子转速
 */
static int16_t CAN_GetMotorSpeed(uint8_t *rxData)
{
	int16_t speed;
	speed = ((uint16_t)rxData[2] << 8| rxData[3]);
	return speed;
}

/**
 *	@brief	获取电机实际转矩电流
 */
static int16_t CAN_GetMotorCurrent(uint8_t *rxData)
{
	int16_t current;
	current = ((int16_t)rxData[4] << 8 | rxData[5]);
	return current;
}

/**
 *	@brief	获取电机实际温度
 */
static uint8_t CAN_GetMotorTemperature(uint8_t *rxData)
{
	uint8_t temperature;
	temperature = rxData[6];
	return temperature;
}

/**
 *	@brief	更新电机数据
 */
void motor_update(motor_t *motor, uint8_t *rxBuf)
{
	motor_info_t *motor_info = motor->info;
	
	motor_info->angle   = CAN_GetMotorAngle(rxBuf);	
	
	motor_info->speed   = CAN_GetMotorSpeed(rxBuf);
	
	motor_info->current = CAN_GetMotorCurrent(rxBuf);
	
	motor_info->temperature = CAN_GetMotorTemperature(rxBuf);
	
	motor_info->offline_cnt=0;
}

char motor_offline_check(void)
{
	char offline_num = 0;
	
	MOTOR.heart_beat(&Chas_Motor[POWER_0]);
	MOTOR.heart_beat(&Chas_Motor[POWER_1]);
	MOTOR.heart_beat(&Chas_Motor[POWER_2]);
	MOTOR.heart_beat(&Chas_Motor[POWER_3]);
	
	MOTOR.heart_beat(&Gimb_Motor[GIMB_Y]);
	MOTOR.heart_beat(&Gimb_Motor[GIMB_P]);
	
	MOTOR.heart_beat(&Shot_Motor[Left_Fric]);
	MOTOR.heart_beat(&Shot_Motor[Right_Fric]);
	MOTOR.heart_beat(&Shot_Motor[Feed]);
	MOTOR.heart_beat(&Shot_Motor[Cover]);	
  
	for(char i=0 ; i < 4 ; i++ )
	{     
		if(MOTOR.chas[i]->work_state == DEV_OFFLINE)offline_num++;
	}		
	for(char i=0 ; i < 2 ; i++ )
	{     
		if(MOTOR.gimb[i]->work_state == DEV_OFFLINE)offline_num++;
	}	
	for(char i=0 ; i < 4 ; i++ )
	{     
		if(MOTOR.shot[i]->work_state == DEV_OFFLINE)offline_num++;
	}
	return offline_num;
}

/*************** 信息协议处理 ***************/

/** RM3508 CAN标识符 **/
static uint32_t RM3508_GetStdId(drv_can_t *drv)
{
	if((drv->can_id - 0x201U) < 4)
		return 0x200;
	else
		return 0x1FF;
}

/** RM3508 CAN数据下标 **/
static uint8_t RM3508_GetDrvId(drv_can_t *drv)
{
	return (drv->can_id - 0x201U)%4;
}

static uint32_t RM2006_GetStdId(drv_can_t *drv)
{
	if((drv->can_id - 0x201U) < 4)
		return 0x200;
	else
		return 0x1FF;
}

static uint8_t RM2006_GetDrvId(drv_can_t *drv)
{
	return (drv->can_id - 0x201U)%4;
}

/*控制online和offline*/
void motor_heart_beat(motor_t *motor)
{
	motor_info_t *motor_info = motor->info;
	
	motor_info->offline_cnt++;
	if(motor_info->offline_cnt > motor_info->offline_max_cnt) 
	{
		motor_info->offline_cnt = motor_info->offline_max_cnt;
		motor->work_state = DEV_OFFLINE;
	}
	else 
	{
		if(motor->work_state == DEV_OFFLINE)
			motor->work_state = DEV_ONLINE;
	}
}

/**
 *	@brief	电机初始化，将失联计数值设置为最大值，只能由heart_beat任务清零
 */
void motor_init(motor_t *motor)
{
	drv_can_t *drv_can = motor->driver;
	
	motor->info->offline_cnt=motor->info->offline_max_cnt+1;
	motor->work_state=DEV_OFFLINE;
	
	if(motor->type == CHASSIS_POWER || motor->type == SHOOT_FRIC) 
	{
		drv_can->drv_id = RM3508_GetDrvId(drv_can);
		drv_can->std_id = RM3508_GetStdId(drv_can);
	}
	else if(motor->type == SHOOT_FEED || motor->type == SHOOT_COVER) 
	{
		drv_can->drv_id = RM2006_GetDrvId(drv_can);
		drv_can->std_id = RM2006_GetStdId(drv_can);
	}
}

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
uint8_t Send_Current(uint8_t CAN, uint32_t stdId, int16_t *dat)
{

	uint8_t ret,num = 0;
	
	for(char i = 0;i < 4; i++)
	{
		if(dat[i] == NULL)dat[i] = 0;
		if(dat[i] != 0)num++;
	}
	
	if(CAN == 1 && num)
	{
		ret = CAN_SendData(&hcan1, stdId, dat);
	}
	else if(CAN == 2 && num)
	{
		ret = CAN_SendData(&hcan2, stdId, dat);
	}
	//每次发送完后清空数据包
	for(char i = 0;i < 4; i++)
	dat[i] = 0;
	
	return ret;
}

void MOTOR_CAN1_RX(uint32_t canId, uint8_t *rxBuf)
{

	switch(canId)
	{
		case CAN_ID_POWER_0:
			MOTOR.update(&Chas_Motor[POWER_0], rxBuf);
			break;
		case CAN_ID_POWER_1:
			MOTOR.update(&Chas_Motor[POWER_1], rxBuf);
			break;		
		case CAN_ID_POWER_2:
			MOTOR.update(&Chas_Motor[POWER_2], rxBuf);
			break;	
		case CAN_ID_POWER_3:
			MOTOR.update(&Chas_Motor[POWER_3], rxBuf);
			break;		
		case CAN_ID_GIMB_Y:
			MOTOR.update(&Gimb_Motor[GIMB_Y], rxBuf);
			break;	
		case CAN_ID_GIMB_P:
			MOTOR.update(&Gimb_Motor[GIMB_P], rxBuf);
			break;		
	}
}

void MOTOR_CAN2_RX(uint32_t canId, uint8_t *rxBuf)
{

		switch(canId)
		{
			case CAN_ID_FRIC_L:
				MOTOR.update(&Shot_Motor[Left_Fric], rxBuf);
				break;
			case CAN_ID_FRIC_R:
				MOTOR.update(&Shot_Motor[Right_Fric], rxBuf);
				break;		
			case CAN_ID_BOX:
				MOTOR.update(&Shot_Motor[Feed], rxBuf);
//			  if(Shot_Motor[Feed].pid->Feed_Single_Angle_Pid.target != 0)
			   Angle_Process(&Shot_Motor[Feed]);
				break;		
			case CAN_ID_BARREL:
				MOTOR.update(&Shot_Motor[Cover], rxBuf);
				break;		
		}
}


