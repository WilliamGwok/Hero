#ifndef __COUPLE_H
#define __COUPLE_H

#include "stm32f4xx_hal.h"
#include "rp_device.h"
#include "judge_sensor.h"

#define COUPLE  1

typedef __packed struct{
	
	uint8_t bullet_freq;  // �ӵ���Ƶ(Hz)
	
	float bullet_speed;		// �ӵ�����(m/s)
	
	uint16_t shooter_id1_17mm_cooling_heat; //1 �� 17mm ǹ������
	
}Pack_1_t;

typedef __packed struct{
	
	uint16_t chassis_volt;   				// ���������ѹ����λ��mV
	
	uint16_t chassis_current;				// ���������������λ��mA
	
	float chassis_power;   					// ����˲ʱ���ʣ���λ��W
	
}Pack_2_t;

typedef __packed struct{
	
	uint16_t chassis_power_buffer;	// ���̹��ʻ���
	
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


