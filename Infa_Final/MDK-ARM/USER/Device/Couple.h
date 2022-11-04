#ifndef __COUPLE_H
#define __COUPLE_H

#include "stm32f4xx_hal.h"
#include "rp_device.h"
#include "judge_sensor.h"

#define COUPLE  1

typedef __packed struct{
	
	uint8_t bullet_freq;  // 子弹射频(Hz)
	
	float bullet_speed;		// 子弹射速(m/s)
	
	uint16_t shooter_id1_17mm_cooling_heat; //1 号 17mm 枪口热量
	
}Pack_1_t;

typedef __packed struct{
	
	uint16_t chassis_volt;   				// 底盘输出电压，单位：mV
	
	uint16_t chassis_current;				// 底盘输出电流，单位：mA
	
	float chassis_power;   					// 底盘瞬时功率，单位：W
	
}Pack_2_t;

typedef __packed struct{
	
	uint16_t chassis_power_buffer;	// 底盘功率缓存
	
	uint16_t shooter_id1_17mm_speed_limit;
	
	uint16_t shooter_id1_17mm_cooling_limit;
	
	
	
}Pack_3_t;


typedef struct{
	
	Pack_1_t   Pack_1;
	
	Pack_2_t    Pack_2;
	
	Pack_3_t    Pack_3;
	
	uint16_t		       offline_cnt;
	
  uint16_t		       offline_max_cnt;	
	
  dev_work_state_t	 work_state;
	
}Down_t;

extern Down_t Down;

void Down_Send_Pack(judge_sensor_t	*judge_sensor);
void Up_Receive_Data(uint32_t id, uint8_t *buff);
void Down_heart_beat(void);

#endif


