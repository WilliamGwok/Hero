#include "car.h"
#include "stdbool.h"
#include "config_status.h"

bool top_car_on = false;
bool top_car_off = false;
bool car_mode_change = false;

void car_init(car_t* car)
{
	car->move_command = sleep_car;
	
	car->car_move_status = sleep_car;
	
	car->ctrl_command = RC_CTRL;
	
	car_command_init();
}

void car_command_init(void)
{
	top_car_on = false;
  top_car_off = false;
	car_mode_change = false;
}

void car_ctrl(car_t* car)
{
	car_ctrl_mode_update(car);
	
	car_status_update(car);
}

void car_ctrl_mode_update(car_t* car)
{
	Rc_S2_Status_Check(car);
	
	switch(car->ctrl_command)
	{
		case KEY_CTRL:
			Key_Status_Scan(car);
			break;
		case RC_CTRL:
			Rc_Status_Scan(car);
			break;
		default:
			break;
	}
}

void car_status_update(car_t* car)
{
	if(rc.info->status == DEV_OFFLINE)
	{
		car->car_move_status = sleep_car;
	}
	else
	{
		if(car->car_move_status != car->move_command)
		{
			car->car_move_status = car->move_command;
			
			car_mode_change = true;
		}
		else
		{
			car_mode_change = false;
		}
	}
}

void Rc_S2_Status_Check(car_t* car)
{
	switch(rc.base_info->s2.status)
	{
		case up_R:
			car->ctrl_command = KEY_CTRL;
		  car->move_command = imu_car;
			break;
		case mid_R:
			car->ctrl_command =RC_CTRL;
		  car->move_command = mec_car;
			break;
		case down_R:
			car->ctrl_command =RC_CTRL;
		  car->move_command = imu_car;
			break;
		default:
			break;
	}
}

void Rc_Status_Scan(car_t* car)
{
	Rc_S1_Status_Check();
	
	Rc_Thumbwheel_Check(car);
}

void Rc_S1_Status_Check(void)
{
	switch(rc.base_info->s1.status)
	{
		case up_R:

			break;
		case mid_R:

			break;
		case down_R:

			break;
		default:
			
			break;
	}
}

void Rc_Thumbwheel_Check(car_t* car)
{
	switch(rc.base_info->thumbwheel.status)
	{
		case up_R:
			switch(car->move_command)
			{
				case mec_car:
					break;
				case imu_car:
					top_car_on = true;
				  break;
			}
			break;
		case mid_R:
			break;
		case down_R:
			switch(car->move_command)
			{
				case mec_car:
					break;
				case imu_car:
					top_car_off = true;
				  break;
			}
			break;
			default:
			break;
	}
}

void Key_Status_Scan(car_t* car)
{
	
}

