#include <DC_motor.h>

float DegreeSec2RPM(float speed_degsec){
	float speed_rpm = speed_degsec * 60/360;
	return speed_rpm;
}

float Voltage2Duty(float u){

	if(u<=0){
		u=-u;
	}

	float duty = 100*u/V_MAX;

	if(duty>100){
		duty=100;
	} else if(duty<7.2 && duty > 0){
		duty = 7.5;
	} else if(duty<0){
		duty = 0;
	}

	return duty;
}

uint8_t Ref2Direction(float y_ref){
	uint8_t dir;

	if(y_ref>=0){
		dir = 0; //Senso orario
		//dir = 1;
	} else {
		dir = 1; //Senso antiorario
		//dir = 0;
	}
	return dir;
}

void set_PWM_and_dir(uint32_t duty, uint8_t dir){

	TIM2->CCR1 = ((float)duty/100)*TIM2->ARR;

	/*uint8_t current_dir = (TIM4->CR1 & 0x0010);
	printf("%d\r\n",current_dir);
	if(dir != current_dir){
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);
	}*/
	if( dir == 0){
		//TIM2->CR1 = dir;
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	}else if ( dir == 1){
		//TIM2->CR1 = dir;
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	}
}
