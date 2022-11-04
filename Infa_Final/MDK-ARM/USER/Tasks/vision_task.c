#include "vision_task.h"
#include "cmsis_os.h"
#include "main.h"
#include "State.h"


void Start_vision_task(void const * argument)
{
  /* USER CODE BEGIN Start_vision_task */
  /* Infinite loop */
  for(;;)
  {
//		if(Gim_Ctrl.Com_Ctrl == TRUE)
//		{
			Vision_Get_Data();
//		}
		Vision_Transmit_Data();
    osDelay(1);
  }
  /* USER CODE END Start_vision_task */
}



