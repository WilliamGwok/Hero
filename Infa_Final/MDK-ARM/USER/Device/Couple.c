#include "Couple.h"
#include "judge_sensor.h"
#include "string.h"
#include "can_drv.h"


#define Pack1_ID    0x601 
#define Pack2_ID    0x602
#define Pack3_ID    0x603

Down_t Down = 
{
	.work_state = DEV_OFFLINE,
	.offline_max_cnt = 5000,
};

uint8_t Send_Buff[8];



void Down_Send_Pack(judge_sensor_t	*judge_sensor)
{
	judge_info_t 	*info = judge_sensor->info;
	
	Down.Pack_1.bullet_freq = info->shoot_data.bullet_freq;
	Down.Pack_1.bullet_speed = info->shoot_data.bullet_speed;
	Down.Pack_1.shooter_id1_17mm_cooling_heat = info->power_heat_data.shooter_id1_17mm_cooling_heat;
	
	memcpy(Send_Buff, &Down.Pack_1, 8);
	CAN_Transmit(Pack1_ID, Send_Buff,2,8);
	
	Down.Pack_2.chassis_current = info->power_heat_data.chassis_current;
	Down.Pack_2.chassis_volt = info->power_heat_data.chassis_volt;
	Down.Pack_2.chassis_power = info->power_heat_data.chassis_power;
	
	memcpy(Send_Buff, &Down.Pack_2, 8);
	CAN_Transmit(Pack2_ID, Send_Buff,2,8);
	
	Down.Pack_3.chassis_power_buffer = info->power_heat_data.chassis_power_buffer;
	Down.Pack_3.shooter_id1_17mm_cooling_limit = info->game_robot_status.shooter_id1_17mm_cooling_limit;
	Down.Pack_3.shooter_id1_17mm_speed_limit = info->game_robot_status.shooter_id1_17mm_speed_limit;
	
	memcpy(Send_Buff, &Down.Pack_3, 8);
	CAN_Transmit(Pack3_ID, Send_Buff,2,8);
	
}


void Up_Receive_Data(uint32_t id, uint8_t *buff)
{
	judge_info_t *info = judge_sensor.info;
	
	switch(id)
	{
		case Pack1_ID :
			memcpy(&Down.Pack_1, buff, 8);
		  info->shoot_data.bullet_freq = Down.Pack_1.bullet_freq;
	    info->shoot_data.bullet_speed = Down.Pack_1.bullet_speed;
	    info->power_heat_data.shooter_id1_17mm_cooling_heat = Down.Pack_1.shooter_id1_17mm_cooling_heat;
		  info->offline_cnt = 0;
		  Down.offline_cnt = 0;
		  break;
		
		case Pack2_ID :
			memcpy(&Down.Pack_2, buff, 8);
		  info->power_heat_data.chassis_current = Down.Pack_2.chassis_current;
	    info->power_heat_data.chassis_volt = Down.Pack_2.chassis_volt;
	    info->power_heat_data.chassis_power = Down.Pack_2.chassis_power;
		  info->offline_cnt = 0;
		  Down.offline_cnt = 0;
		  break;
		
		case Pack3_ID :
			memcpy(&Down.Pack_3, buff, 8);
		  info->power_heat_data.chassis_power_buffer = Down.Pack_3.chassis_power_buffer;
		  info->game_robot_status.shooter_id1_17mm_cooling_limit = Down.Pack_3.shooter_id1_17mm_cooling_limit;
	    info->game_robot_status.shooter_id1_17mm_speed_limit = Down.Pack_3.shooter_id1_17mm_speed_limit;
		  info->offline_cnt = 0;
		  Down.offline_cnt = 0;
	}
}


void Down_heart_beat(void)
{
	Down.offline_cnt++;
	if(Down.offline_cnt > Down.offline_max_cnt) 
	{
		Down.offline_cnt = Down.offline_max_cnt;
		Down.work_state = DEV_OFFLINE;
	}
	else 
	{
		if(Down.work_state == DEV_OFFLINE)
			 Down.work_state =  DEV_ONLINE;
	}
}


