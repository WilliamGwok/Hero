#include "led_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "led_drv.h"
#include "Motor.h"

extern char offlinenum;

void Start_led_Task(void const * argument)
{
  /* USER CODE BEGIN Start_led_Task */
  /* Infinite loop */
  for(;;)
  {
		if(offlinenum>0)
		{
			Float_LED();
		}
		else
		{
			Alarm_LED();
		}
    osDelay(1);
  }
  /* USER CODE END Start_led_Task */
}

