#ifndef __PID_ALGO_H
#define __PID_ALGO_H

#include "stm32f4xx_hal.h"

typedef enum {
	Pitch_PID_FLAG,
  YAW_PID_FLAG,
  FOR_CHAS_PID_FLAG,  
}pid_flag;

typedef struct pid_ctrl {
    float		target;
    float		measure;
    float 	err;
    float 	last_err;
    float		kp;
    float 	ki;
    float 	kd;
    float 	pout;
    float 	iout;
    float 	dout;
    float 	out;
    float		integral;
    float 	integral_max;
    float 	out_max;
	  float   deta_err;
	  float   blind_err;
	  char    pid_flag;//这个写在外环，内环写了没用
} pid_ctrl_t;

void pid_val_init(pid_ctrl_t *pid);
float err_calculation(pid_ctrl_t *pid,float measure);
void single_pid_ctrl(pid_ctrl_t *pid);

#endif



