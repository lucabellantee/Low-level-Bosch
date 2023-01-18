/*
 * enc2.h
 *
 *  Created on: Jan 18, 2023
 *      Author: lucab
 */

#ifndef INC_ENC2_H_
#define INC_ENC2_H_


#define PI_GREEK 3.1416
unsigned int MODULO_ENC2;
//float mDEGREE_PER_PULSE;
float get_position_enc2(int count);
float get_position_in_rad_enc2(float pos);
float get_speed_in_degree_per_sec_enc2(float pos, float old_pos);
float get_speed_in_rad_per_sec_enc2(float speed);

/* Main structure that contains the measured data of position and speed */
struct encoder_data {
long int position; //absolute position in the number of pulses (with sign)
//starting from the initial position taken as equal to 0
int speed; //speed in number of pulses per ms
int oscillazione; //indicates whether the encoder (the tree) is stopped
float position_in_degree; //position with sign of degrees from the initial
//position assumed equal to 0
float position_in_pigreek_radians_units; //position in pigreek units with sign
float speed_in_degree_per_sec; //speed in degrees per second with sign
float speed_in_rad_per_sec; //speed in radians per second with sign
};



#endif /* INC_ENC2_H_ */
