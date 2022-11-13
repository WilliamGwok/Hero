#include "control_task.h"


void control_task(void const * argument)
{
  for(;;)
  {
		Cal_Pulse();

		if(rc.info->status == DEV_ONLINE)
		{
      Gimbal_Ctrl();
			
			Chassis_Ctrl();
		}
		else
		{
      Car_Sleep();
		}
		
    osDelay(1);
  }
}


