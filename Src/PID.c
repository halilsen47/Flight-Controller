/*
 * PID.c
 *
 *  Created on: 5 Haz 2026
 *      Author: senha
 */

#include "PID.h"


float Compute_PID(PID_controller_t *pid,float setpoint,float current_speed)
{

	//compute dt
	uint32_t tim = __HAL_TIM_GET_COUNTER(&htim2);
	uint32_t tim_diff = tim - pid->prev_tim;
	float dt = (float)tim_diff / 1000000.0f;

	pid->prev_tim = tim;

	//compute err and PİD
	float err = setpoint - current_speed;

	float p_output = pid->kp * err;

	pid->Integral_tot += err * dt;
	//wind-up
	if(pid->Integral_tot > 200)
		pid->Integral_tot  = 200;
	else if(pid->Integral_tot < -200)
		pid->Integral_tot  = -200;

	float i_output = pid->ki * pid->Integral_tot;



	float derivative = (err - pid->prev_err) / dt;
	float d_output = pid->kd * derivative;
	pid->prev_err = err;

	float tot = p_output + i_output + d_output;
	if(tot > 500)
		tot = 500;
	else if(tot < -500)
		tot = -500;

	return tot;

}


