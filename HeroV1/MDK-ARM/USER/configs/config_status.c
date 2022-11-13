#include "config_status.h"

uint8_t stop_buff[16];

void Car_Sleep(void)
{
	memset(stop_buff,0,16);
	
	CAN_SendData(&hcan1,0x200,stop_buff);
	CAN_SendData(&hcan1,0x1FF,&stop_buff[8]);
	CAN_SendData(&hcan2,0x200,stop_buff);
	CAN_SendData(&hcan2,0x1FF,&stop_buff[8]);
	
	memset(stop_buff,0,16);
}

void Cal_Pulse(void)
{
	rc_tick_task(&rc);
	Chassis_Heartbeat();
	Gimbal_Heartbeat();
	Shooting_Heartbeat();
}

