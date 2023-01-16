#include "PID.h"

//Questa funziona setta i prametri principali
void init_PID(PID* p, float Tc, float u_max, float u_min){
	p->Tc = Tc;
	p->u_max = u_max;
	p->u_min = u_min;
}

void tune_PID(PID*p, float Kp, float Ki, float Kd){
	p->Kp = Kp;
	p->Ki = Ki;
	p->Kd = Kd;
}

float PID_controller(PID* p , float y, float r){
	//printf("%f\r\n", y);
	//printf("%f\r\n", r);
	//printf("------------\r\n");
	static float e_old=0, Iterm=0;
	float u;
	float newIterm;

	float e = r-y;
	//printf("--------------------------------------\r\n");
	//printf("err: %f\r\n", e);
	//printf("|||||||||||||||||||||||||||||||||||||||||||\r\n");
	//printf("-------------------\r\n");

	float Pterm = p->Kp*e;
	newIterm = Iterm + (p->Ki)*p->Tc*e_old;
	float Dterm = (p->Kd/p->Tc)*(e - e_old);

	e_old = e;

	u = Pterm + newIterm + Dterm;
	//printf("uuuuuuuuuuuuuu: %f \r\n", u);
	//printf("*********************************\r\n");
	if(u > p->u_max){
		u = p->u_max; //limite massimo
	} else if(u < p->u_min){
		u = p->u_min; //limite minimo
	} else {
		Iterm = newIterm;
	}


	//printf("-----------------\r\n");
	return u;
}

