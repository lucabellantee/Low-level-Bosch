/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "PID.h"
#include "DC_motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
//	int percentuale = 20;
	//int minPot = 0;
    int flag = 0;
    int i = 0;
    PID pid;
    //double V_MAX = 10;
    double u;
    float y_ref = 2000;
    uint8_t direction;
    double duty;
    //double T_ref;
    //float speed_rpm = 8;
    //int avanti=0, dietro=0;
    //int flag_100ms;
    int flag_Tc;
    int n_ref = 0;
    int counts;
    double delta_angle;
    double diff_angle;
    int dir;
    float speed;
    double old_delta_angle;
    int old_dir = 0;
    int ENCODER_PPR = 2048;
    int GEARBOX_RATIO = 1;
    int ENCODER_COUNTING_MODE =4;
    double dt = 0.001;
    int flag20_ms=0;
    int conta=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 //  float converti_perc(float) ;
 //  void set_vel_avanti(float);
 //  void set_vel_indietro(float);
 //  void set_vel_neutra(float);
  /* USER CODE END 1 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  /*
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	//int percentuale = 20;
	//int minPot = 0;


  //HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
  HAL_TIM_Base_Start_IT(&htim3);
  init_PID(&pid, dt, V_MAX, -V_MAX);
  tune_PID(&pid, 0.004, 0.0076, 0);  // Pi vecchio : 0.017  //0.0012
 //0.043, 0.86, 0 vecchi 	0.05 (dt)
  	  	  	  	  	  	  	  	  // 0.000341, 0, 0			0.001(dt)
  //  tune_PID(&pid, 0.00017, 0.017, 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*
	      Tutti i parametri sono settati prendendo come riferimento quelli del progetto di kitt

	   */
	  //TODO LUCA: ANTIORARIO --> 1 , ORARIO --> 0 ||  MA ALLE VOLTE LA DIR ME LAMETTE A 0 E A 1
	  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	  //TIM2->CCR1=100;	//min duty = 72

     if(flag_Tc == 1)
     {
    	 //flag_100ms = 0;
    	// printf("\n Flag: %d\r\n",flag20_ms);
    	 flag20_ms++;
    	 flag_Tc = 0;
    	 //counts = TIM4->CNT; //Prendo il valore
    	 //printf("%d\r\n", counts);
    	 //printf("------------\r\n");

    	 //Angolo
    	 diff_angle = (double)(counts-(TIM4->ARR/2))*360 / (ENCODER_PPR * ENCODER_COUNTING_MODE * GEARBOX_RATIO);
    	 speed = diff_angle/dt;

    	 //Velocità
    /*	 diff_angle = delta_angle - old_delta_angle;
    	 dir = (TIM4->CR1 & 0x0010)>>4; //0 = up, 1 = down
    	 if(dir == old_dir)
    	 {
    		 if(dir == 0 && diff_angle<0)
    		 {
    			 diff_angle = 360 + diff_angle;
    		 } else if (dir == 1 && diff_angle>0)
    		 {
    			 diff_angle = diff_angle - 360 ;
    		 }
    	 }
   */


    	// old_delta_angle = delta_angle;
    	 //old_dir = dir;

    	 //printf("%f\n", speed);


    	 /*if(n_ref <= T_ref/dt){
    		 y_ref = 30;
    	 } else {
    		 y_ref = 60;
    	 }*/

    	 //u = PID_controller(&pid, speed_rpm, y_ref);
    	 u = PID_controller(&pid, speed*60/360, y_ref);
    	 duty = Voltage2Duty(u);
    	 //printf("%f,%f\n",u,y_ref);
    	 direction = Ref2Direction(y_ref);
    	 //printf("%f\r\n",(float)direction);
    	 set_PWM_and_dir((uint32_t)duty, direction);
    	 //set_PWM_and_dir(50, direction);
    	 printf("%f;%d;%f\r\n", speed*60/360, (uint32_t)duty, u);

/*
    	 if(conta >= 5000){
    		 y_ref = 2000;
    	 }
    	 conta++;
*/
        // printf("%f\r\n",speed*60/360);
    	 //printf("%f;%f;%d\r\n",delta_angle,speed*60/360,dir);
    	 //printf("%f,COUNT: %d\n\r", speed,counts);
    //	 __io_putchar('c');
    //	 printf("DIR: %d\r\n", (int)dir);

     }

     //	 printf("\r\n Y REF = %f\r\n",y_ref);





     	 /*if(flag20_ms == 19){
     		 	 flag20_ms = 0;
     		 	 u = PID_controller(&pid, speed*60/360, y_ref);
     	    	 duty = Voltage2Duty(u);
     	    	 //printf("%f,%f\n",u,y_ref);
     	    	 direction = Ref2Direction(y_ref);
     	    	 //printf("%f\r\n",(float)direction);
     	    	 set_PWM_and_dir((uint32_t)duty, direction);
     	    	 //printf("%f;%d;%f;%f;%d\r\n", speed*60/360, (uint32_t)duty, u, delta_angle,(int)dir);
     	    	 printf("%f\n",speed*60/360);
     	    	 //set_PWM_and_dir(50, direction);

     	 }*/







	 // if(GPIO_Pin == GPIO_PIN_13)  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	//  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	  // Va al massimo per circa 10 secondi
	         // add a delay of 1 ms as dimmering time-step
	     //HAL_Delay(5000);
	     //  1000 = COUNTER PERIOD
	     // (percentuale/100)*1000
	  	 //TIM2->CCR1=300; // CCR3 = duty cycle register value					ATTENTO QUA

	 // Spengo il pwm per circa 5 secondi
	     //HAL_Delay(10000);
	     //TIM2->CCR1=minPot;

	//  if (i<=999) i++;
	//  else i=0;

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 240-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 12-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65536-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 10;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(dir_GPIO_Port, dir_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : dir_Pin */
  GPIO_InitStruct.Pin = dir_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(dir_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

  int __io_putchar(int ch)
    {
       HAL_UART_Transmit(&huart3, (uint8_t *)&ch , 1 , 0xFFFF);
       return ch;
    }

/*
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{

		if(GPIO_Pin == GPIO_PIN_13 && flag==0)
		{
			flag = 1;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		}

		else
		{
			flag = 0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		}
     }*/

	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		if(htim == &htim3){
			//flag_100ms = 1;
			counts = TIM4->CNT;
			TIM4->CNT = TIM4->ARR / 2;
			HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
			flag_Tc = 1;
			n_ref++;
		}
	}


	/*
    float converti_perc(float perc) {
          float deltaPulse = (perc*0.5)/100;
          if (deltaPulse >= 0 && deltaPulse <= 0.5)
                         return deltaPulse;
          else {
                 deltaPulse = 0.25;
                 return deltaPulse;
               }
    }


    void set_vel_avanti(float percentuale)
    {
    	int ccr = 0;
    	int periodo = 20;
    	float pulse = 1.5 + converti_perc(percentuale);
    	ccr = (pulse*1000)/periodo;
//      Credo che questi valori dipendano un po da quello che si vuole fare e dal motore , io gli ho presi dal pdf della macchina dello scorso anno(KITTY)
    	if(ccr >= 75 && ccr <= 100) TIM2->CCR2 = ccr;
    	else TIM2->CCR1 = 87;
    }


    void set_vel_indietro(float percentuale)
    {
    	int ccr = 0;
    	int periodo = 20;
    	float pulse = 1.5 - converti_perc(percentuale);
    	ccr = (pulse*1000)/periodo;

    	if(ccr >= 50 && ccr <= 75) TIM2->CCR1 = ccr;
    	else TIM2->CCR1 = 62;
    }


    void avanti_indietro()
    {
  	  if(flag == 0){
  		  if(i==0){
  			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  			  TIM2->CCR1=minPot;
  			  HAL_Delay(1000);}
  		  i=1;
  	  }
  	  else{
  		  if(i==1){
  			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
  			  TIM2->CCR1=minPot;
  			  HAL_Delay(1000);}
  		  i=0;
  	  }
    }*/
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
