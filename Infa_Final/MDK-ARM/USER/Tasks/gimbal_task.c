#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can_drv.h"
#include "Motor.h"
#include "gimbal_task.h"


void Start_gimbal_task(void const * argument)
{
  /* USER CODE BEGIN Start_gimbal_task */
	GIMBAL_YAW_Mec_INIT();
  GIMBAL_PITCH_Mec_INIT();
  GIMBAL_YAW_Imu_INIT();
  GIMBAL_PITCH_Imu_INIT();
  /* Infinite loop */
  for(;;)
  {
		GIMBAL_CTRL();
    osDelay(1);
  }
  /* USER CODE END Start_gimbal_task */
}

