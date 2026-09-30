/*
 * PID.h
 *
 *  Created on: 5 Haz 2026
 *      Author: senha
 */

#ifndef INC_PID_H_
#define INC_PID_H_

typedef struct
{
	float kp;
	float ki;
	float kd;

	float Integral_tot;
	float prev_err;

	uint32_t prev_tim;
}PID_controller_t;


float Compute_PID(PID_controller_t *pid,float setpoint,float current_speed);


#endif /* INC_PID_H_ */
