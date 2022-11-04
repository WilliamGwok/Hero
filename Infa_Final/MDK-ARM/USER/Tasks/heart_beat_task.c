#include "heart_beat_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can_drv.h"
#include "Chassis.h"
#include "vision_sensor.h"
#include "judge_sensor.h"
#include "couple.h"


char offlinenum;
int16_t Stop_Cmd[4] = {0,0,0,0};

sleep_t RC_Sleep = 
{
	.time = 0,
};

void Sleep_Mode(void)
{
	Chassis_Motor_Stop();
//	Shot_Stop();
}

void RC_Offline_Sleep(void)
{
	if(RC_OFFLINE)
	{
		Remote_Ctrl.RC_Mode = TRUE;
		Remote_Ctrl.Key_Mode = FALSE;
		
		if(HAL_GetTick() - RC_Sleep.time < 1000)Sleep_Mode();
		
		else if(HAL_GetTick() - RC_Sleep.time < 4500)
		{
			CAN1_SendData(GM_F_ID, Stop_Cmd);
			CAN1_SendData(RM_F_ID, Stop_Cmd);
			
			CAN2_SendData(RM_F_ID, Stop_Cmd);			
			CAN2_SendData(GM_F_ID, Stop_Cmd);		
			Mode_Init();
		}
	}
	else if(RC_ONLINE)
	{
		RC_Sleep.time = HAL_GetTick();
	}
}

//遥控失联复位，并将状态初始化
void rc_offline_handle(void)
{
	RC_ResetData(&rc);
}

void Start_heart_beat_task(void const * argument)
{
  /* USER CODE BEGIN Start_heart_beat_task */
	Mode_Init();
  /* Infinite loop */
  for(;;)
  {
		RC_Offline_Sleep();
#if COUPLE == 1
		Mode_Ctrl();
		
		if(rc_offline_check())rc_offline_handle();
		offlinenum=motor_offline_check();
		
		vision_sensor.heart_beat(&vision_sensor);
		
		Down_heart_beat();
#endif
		
#if COUPLE == 0
		
		Down_Send_Pack(&judge_sensor);
		
		
#endif
		judge_sensor.heart_beat(&judge_sensor);
    osDelay(1);
  }
  /* USER CODE END Start_heart_beat_task */
}

