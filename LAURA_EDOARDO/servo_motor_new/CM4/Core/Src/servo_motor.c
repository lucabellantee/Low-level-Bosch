/*
 * servo_motor.c
 *
 *  Created on: Dec 12, 2022
 *      Author: ceced
 */

#include "servo_motor.h"



void servo_motor(int angolo)
{
	float tic;
	tic = 0.02;
			//((PRESCALER/TIMER_CLOCK)*1000);
	  float ccr;
	  int conv_angolo;

	  if(angolo < MIN_ANGOLO)

		   angolo = MIN_ANGOLO;

	  else if (angolo > MAX_ANGOLO)

		   angolo = MAX_ANGOLO;

	  conv_angolo = angolo + DRITTO;

	  ccr=(((TEMPO*conv_angolo)+0.5)/tic);
	  TIM2->CCR1=ccr;
	  return ccr;

}
