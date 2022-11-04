#include "pid_algo.h"
#include "main.h"


void pid_val_init(pid_ctrl_t *pid)
{
	pid->err = 0;
	pid->last_err = 0;
	pid->integral = 0;
	pid->pout = 0;
	pid->iout = 0;
	pid->dout = 0;
	pid->out = 0;
}

float err_calculation(pid_ctrl_t *pid,float measure)
{
	pid->measure = measure;
	
	pid->err = pid->target - pid->measure;
	
	return pid->err;
}

void single_pid_ctrl(pid_ctrl_t *pid)
{
	if(abs(pid->err)<=(pid->blind_err))
		pid->err = 0;

	pid->integral += pid->err;
	pid->integral = constrain(pid->integral, -pid->integral_max, +pid->integral_max);

	pid->pout = pid->kp * pid->err;
	pid->iout = pid->ki * pid->integral;

	pid->out = pid->pout + pid->iout;
	pid->out = constrain(pid->out, -pid->out_max, pid->out_max);

	pid->last_err = pid->err;
}

