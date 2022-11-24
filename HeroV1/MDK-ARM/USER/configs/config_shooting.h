#ifndef __CONFIG_SHOOTING_H
#define __CONFIG_SHOOTING_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "imu.h"
#include "car.h"
#include "ave_filter.h"

#define feed_reload_work_angle 33480
#define position_work_speed_init 2000
#define friction_work_speed_init 2000
#define friction_10ms_work_speed 2000
#define friction_16ms_work_speed 3000

#define feed_work_reload_time 10

#define resonable_err 500

#endif


