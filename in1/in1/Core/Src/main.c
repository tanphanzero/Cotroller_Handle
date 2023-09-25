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
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
double IC_Val1 = 0;
double IC_Val2 = 0;
double IC_Val3 = 0;
double IC_Val4 = 0;
double IC_Val5 = 0;
double IC_Val6 = 0;
double Width1 = 0;
double Width2 = 0;
double Width3 = 0;
double Duty_Cycle1;
double Duty_Cycle2;
double var;
uint8_t Mode = 0;
uint16_t Scale = 5;
double k1 = 0;
double k2 = 0;
uint8_t Is_First_Captured1 = 0;
uint8_t Is_First_Captured2 = 0;
uint8_t Is_First_Captured3 = 0;

int check_number(double n){
    uint16_t flag;
    if (n >= 0) flag = 0;
    else if (n < 0) flag = 1;
    return flag;
}

// 2 cau H tren phan cung bi nguoc chieu 
int check_number_inv(double n){
    uint16_t flag;
    if (n >= 0) flag = 1;
    else if (n < 0) flag = 0;
    return flag;
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

// Channel 1 tren RC dieu khien tien lui
	 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	 {
		   if (Is_First_Captured1 == 0) 
       {
         IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				 Is_First_Captured1 = 1;
				 __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
					 
       }
			 else if (Is_First_Captured1 == 1)  
       {
           IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); 
					 if(IC_Val2 > IC_Val1)
           {
               Width1 = IC_Val2 - IC_Val1;
           }
					 else if(IC_Val2 < IC_Val1)
           {
               Width1 = (0xFFFF-IC_Val1)+IC_Val2+1;
           }
					 Duty_Cycle1 = (Width1 - 1435);
					 
					// Duty_Cycle1 = (Width1 - 14.5)*1000/5;
					 if ((Duty_Cycle1 > -15) && (Duty_Cycle1 < 15)) Duty_Cycle1 = 0;
					 else if (Duty_Cycle1 < -15) Duty_Cycle1 = Duty_Cycle1*10000/485;
					 else if (Duty_Cycle1 > 15) Duty_Cycle1 = Duty_Cycle1*10000/515;
				//	 else if (Duty_Cycle1 > 1000) Duty_Cycle1 = 1000;
           Is_First_Captured1 = 0;
					 __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
					// __HAL_TIM_SET_COUNTER(htim,0);
       }	 
		 }

// Channel 4 tren RC dieu khien phai trai		 
	 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	 {
		   if (Is_First_Captured2 == 0) 
       {
         IC_Val3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				 Is_First_Captured2 = 1;
				 __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
					 
       }
			 else if (Is_First_Captured2 == 1)  
       {
           IC_Val4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); 
					 if(IC_Val4 > IC_Val3)
           {
               Width2 = IC_Val4 - IC_Val3;
           }
					 else if(IC_Val4 < IC_Val3)
           {
               Width2 = (0xFFFF-IC_Val3)+IC_Val4+1;
           }
					 Duty_Cycle2 = (Width2 - 1628);									// sai lech trong controller nen dung 16.5
					 if ((Duty_Cycle2 > -15) && (Duty_Cycle2 < 15)) Duty_Cycle2 = 0;		// controller khong on dinh nen phai gioi han gia tri
					 else if (Duty_Cycle2 < -15) Duty_Cycle2 = Duty_Cycle2*Scale/503;			
					 else if (Duty_Cycle2 > 15) Duty_Cycle2 = Duty_Cycle2*Scale/493;
           Is_First_Captured2 = 0;
					 __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
					 //__HAL_TIM_SET_COUNTER(htim,0);
       }	 
		 }

// Channel 5 tren RC de chon Mode		 
	 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	 {
		   if (Is_First_Captured3 == 0) 
       {
         IC_Val5 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
				 Is_First_Captured3 = 1;
				 __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
					 
       }
			 else if (Is_First_Captured3 == 1)  
       {
           IC_Val6 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); 
					 if(IC_Val6 > IC_Val5)
           {
               Width3 = IC_Val6 - IC_Val5;
           }
					 else if(IC_Val6 < IC_Val5)
           {
               Width3 = (0xFFFF-IC_Val5)+IC_Val6+1;
           }
					 //Duty_Cycle2 = (Width2 - 1628);									// sai lech trong controller nen dung 16.5
					 if (fabs(Width3 - 1000) < 5) Mode = 1;
					 else if (fabs(Width3 - 1500) < 5) Mode = 2;
					 else if (fabs(Width3 - 2000) < 5) Mode = 3;
           Is_First_Captured3 = 0;
					 __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
					 //__HAL_TIM_SET_COUNTER(htim,0);
       }	 
		 }
	 
// Xuat xung PWM
		switch (Mode) 
		{
			case 1:		// Mode 1: Tot do cao nhat, tang nhanh theo ham mu. Nhuoc diem la re trai, re phai dap ung cham
			k1 = Duty_Cycle1*pow(1.1,-Duty_Cycle2)/pow(1.1,Scale);
			k2 = Duty_Cycle1*pow(1.1,Duty_Cycle2)/pow(1.1,Scale);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,check_number_inv(k1));		// Banh phai
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, fabs(k1));
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,check_number(k2));		// Banh trai
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, fabs(k2));
			break;
			case 2:		// Mode 2: Di chuyen cham
			Duty_Cycle2 = Duty_Cycle2*10000/Scale;
			if ((Duty_Cycle1 == 0) && (Duty_Cycle2 == 0))
			{
				k1 = 0;
				k2 = 0;
			}
			else
			{
				k1 = (Duty_Cycle1*fabs(Duty_Cycle1) - Duty_Cycle2*fabs(Duty_Cycle2))/(fabs(Duty_Cycle1) + fabs(Duty_Cycle2));
				k2 = (Duty_Cycle1*fabs(Duty_Cycle1) + Duty_Cycle2*fabs(Duty_Cycle2))/(fabs(Duty_Cycle1) + fabs(Duty_Cycle2));
			}
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,check_number_inv(k1));		// Banh phai
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, fabs(k1));
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,check_number(k2));		// Banh trai
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, fabs(k2));
			break;
			case 3:		// Mode 3: Di chuyen on dinh. Re trai, re phai, tien lui dap ung nhanh. Nhuoc diem: Banh xe nhanh bi mon
			var = 4000 - fabs(Duty_Cycle2)*800;
			k1 = Duty_Cycle1*(6000 + var)/10000 - Duty_Cycle2*800;
			k2 = Duty_Cycle1*(6000 + var)/10000 + Duty_Cycle2*800;
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,check_number_inv(k1));		// Banh phai
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, fabs(k1));
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,check_number(k2));		// Banh trai
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, fabs(k2));
			break;
		}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
  htim2.Init.Prescaler = 840-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
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
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 84-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xFFFF;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
