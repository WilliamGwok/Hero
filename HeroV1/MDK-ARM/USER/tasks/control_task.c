#include "control_task.h"


void control_task(void const * argument)
{
	car_init(&Car);
	
  for(;;)
  {
		rc_ctrl(&rc);
		
		imu_update(&imu);
		
		Gimbal.imu_update(&Gimbal);
		
		car_ctrl(&Car);
		
		Cal_Pulse();

		if(rc.info->status == DEV_ONLINE)
		{
      Gimbal_Ctrl(&Gimbal);
			
			Chassis_Ctrl();
		}
		else
		{
      Car_Sleep();
		}
		
    osDelay(1);
  }
}


