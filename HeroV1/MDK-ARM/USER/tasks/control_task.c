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
			Shooting_Test();
			
      Gimbal_Ctrl(&Gimbal);
			
			Chassis_Ctrl(&Chassis_Mode);
			
			CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
		  CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
			CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
	    CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
		}
		else
		{
      Car_Sleep();
		}
		
    osDelay(1);
  }
}


