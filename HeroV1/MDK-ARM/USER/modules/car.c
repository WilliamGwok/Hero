#include "car.h"
#include "stdbool.h"
#include "config_status.h"

bool top_car_on = false;
bool top_car_off = false;

bool shoot_start = false;
bool shoot_stop = false;
bool shoot_standby = false;
bool shoot_wake = false;
bool feed_reload = false;
bool feed_unload = false;

bool car_mode_change = false;
bool car_init_ok = false;

extern bool gimbal_init_ok;
extern bool imu_init_Y_O_N;

car_t Car;

void car_init(car_t* car)
{
	car->car_move_command = sleep_car;
	
	car->car_move_status = sleep_car;
	
	car->ctrl_mode = RC_CTRL;
	
	car_command_init();
}

void car_command_init(void)
{
	top_car_on = false;
  top_car_off = false;
	shoot_start = false;
  shoot_stop = false;
	shoot_standby = false;
  shoot_wake = false;
  feed_reload = false;
  feed_unload = false;
}

void car_ctrl(car_t* car)
{
	car_status_update(car);
	
	car_ctrl_mode_update(car);
}

void car_ctrl_mode_update(car_t* car)
{
	Rc_S2_Status_Check(car);
	
	switch(car->ctrl_mode)
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

void car_init_judge(car_t* car)
{
	if(gimbal_init_ok == true && imu_init_Y_O_N == true)
	{
		car_init_ok = true;
	}
}

void car_status_update(car_t* car)
{
	car_init_judge(car);
	
	if(rc.info->status == DEV_OFFLINE)
	{
		car->car_move_status = sleep_car;
		car_mode_change = true;
		gimbal_init_ok = false;
		car_init_ok = false;
		car_command_init();
	}
	else if(car_init_ok == false)
	{
	  car->car_move_status = init_car;
		car_mode_change = true;
		car_command_init();
	}
	else
	{
		if(car->car_move_status != car->car_move_command)
		{
			car->car_move_status = car->car_move_command;
			
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
			car->ctrl_mode = KEY_CTRL;
		  car->car_move_command = imu_car;
			break;
		case mid_R:
			car->ctrl_mode =RC_CTRL;
		  car->car_move_command = mec_car;
			break;
		case down_R:
			car->ctrl_mode =RC_CTRL;
		  car->car_move_command = imu_car;
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
      shoot_start = true;
			break;
		case mid_R:
      shoot_standby = true;
			break;
		case down_R:
      shoot_stop = true;
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
			switch(car->car_move_command)
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
			switch(car->car_move_command)
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

