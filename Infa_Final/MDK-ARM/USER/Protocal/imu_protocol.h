/**
 * @file        imu_protocol.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       Imu Protocol.
 * @update      
 *              v1.0(9-September-2020)
 *              v1.1(24-October-2021)
 *                  1.ÐÞ¸Äimu_potocol.c/.h->imu_protocol.c/.h 
 */
 
#ifndef __IMU_PROTOCOL_H
#define __IMU_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "math.h"
#include "S_function.h"

extern short gyrox, gyroy, gyroz;
extern float value_yaw,value_pitch;

#define YAW_V (value_yaw * cos((imu_sensor.info->pitch)/180 * 3.1415))
#define PITCH_V value_pitch

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#endif




//-180 `180  0~360   0~2pi

