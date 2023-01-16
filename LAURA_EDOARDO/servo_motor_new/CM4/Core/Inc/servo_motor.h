/*
 * servo_motor.h
 *
 *  Created on: Dec 12, 2022
 *      Author: ceced
 */
#include <main.h>


#ifndef INC_SERVO_MOTOR_H_
#define INC_SERVO_MOTOR_H_

#define DRITTO 90
#define MAX_ANGOLO 23
#define MIN_ANGOLO -23
#define TEMPO 0.0111111
#define PRESCALER 4800
#define TIMER_CLOCK 240000000


void servo_motor(int);


#endif /* INC_SERVO_MOTOR_H_ */
