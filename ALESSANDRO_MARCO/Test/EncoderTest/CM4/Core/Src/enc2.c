/*
 * enc2.c
 *
 *  Created on: Jan 18, 2023
 *      Author: lucab
 */


#include <enc2.h>
#define ENCODER_PPR_2 500 /* Pulse per revolution of the Encoder 2 */
#define REDUCTION_GAIN_2 66 /* reduction ratio reverse total (approximate);
from the tests seems that for every revolution
of us are about 66 encoder revolutions
*/
#define PHASE_COUNT_MODE_2 4 /* phase counting mode x4 normally enc 2 */
/* Number of pulses for shaft revolution (takes into account the reduction gain) */


unsigned int MODULO_ENC2 = (ENCODER_PPR_2*REDUCTION_GAIN_2*PHASE_COUNT_MODE_2);
float get_position_enc2(int count){
return (count*360)/MODULO_ENC2;
}
float get_position_in_rad_enc2(float pos){
/* Position in pi rad */
/* 180 : pi = deg : rad => rad = pi * (deg/180) */
return (PI_GREEK*pos)/180;
}
float get_speed_in_degree_per_sec_enc2(float pos, float old_pos){
return (pos-old_pos);
}
float get_speed_in_rad_per_sec_enc2(float speed){
return (PI_GREEK*speed)/180;
}


