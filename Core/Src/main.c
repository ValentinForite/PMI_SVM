/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* Definitions for blink_led */
osThreadId_t blink_ledHandle;
const osThreadAttr_t blink_led_attributes = {
  .name = "blink_led",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Transmission2 */
osThreadId_t Transmission2Handle;
const osThreadAttr_t Transmission2_attributes = {
  .name = "Transmission2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for PLoadStatus */
osThreadId_t PLoadStatusHandle;
const osThreadAttr_t PLoadStatus_attributes = {
  .name = "PLoadStatus",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for OBCAskU */
osThreadId_t OBCAskUHandle;
const osThreadAttr_t OBCAskU_attributes = {
  .name = "OBCAskU",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for OBCAskV */
osThreadId_t OBCAskVHandle;
const osThreadAttr_t OBCAskV_attributes = {
  .name = "OBCAskV",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
void StartBlink_led(void *argument);
void StartTransm2(void *argument);
void PLStatus(void *argument);
void StartOBCAsku(void *argument);
void StartOBCAskV(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Rx1_data[10];
char Rx2_data[10];
char Rx3_data[10];
//http://www.pomad.fr/node/19
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){ //https://os.mbed.com/platforms/ST-Nucleo-L476RG/
	if(huart==&huart2){
		if(Rx2_data=='1'){
			HAL_UART_Transmit(huart,"OK\n",3,HAL_MAX_DELAY);
		}
		HAL_UART_Transmit(huart,Rx2_data,1,HAL_MAX_DELAY);
		HAL_UART_Receive_IT(huart, Rx2_data, 1);
	}
	else if(huart==&huart3){ //Virtual PLoad
		HAL_UART_Transmit(&huart2,"CU a recu ",10,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,Rx3_data,1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,"\n",1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,"[PLD->OBC]CU transmet D\n",24,HAL_MAX_DELAY);
		HAL_UART_Transmit(huart,"D",1,HAL_MAX_DELAY);
		HAL_UART_Receive_IT(huart, Rx3_data, 1);
	}
	else if(huart==&huart1){
		HAL_UART_Transmit(&huart2,"OBC a recu ",11,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,Rx1_data,1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,"\n",1,HAL_MAX_DELAY);
		//HAL_UART_Transmit(&huart2,Rx1_data,1,HAL_MAX_DELAY);
		HAL_UART_Receive_IT(huart, Rx1_data,1);
	}
	else{
		HAL_UART_Transmit(&huart2,"Erreur UART RX\n",15,HAL_MAX_DELAY);
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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  /* Changer les buffers */
  HAL_UART_Receive_IT(&huart3, Rx3_data, 1);
  HAL_UART_Receive_IT(&huart1, Rx1_data, 1);
  HAL_UART_Receive_IT(&huart2, Rx2_data, 1);
  //HAL_UART_Transmit_IT(&huart2,Rx_data,4);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of blink_led */
  blink_ledHandle = osThreadNew(StartBlink_led, NULL, &blink_led_attributes);

  /* creation of Transmission2 */
  Transmission2Handle = osThreadNew(StartTransm2, NULL, &Transmission2_attributes);

  /* creation of PLoadStatus */
  PLoadStatusHandle = osThreadNew(PLStatus, NULL, &PLoadStatus_attributes);

  /* creation of OBCAskU */
  OBCAskUHandle = osThreadNew(StartOBCAsku, NULL, &OBCAskU_attributes);

  /* creation of OBCAskV */
  OBCAskVHandle = osThreadNew(StartOBCAskV, NULL, &OBCAskV_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartBlink_led */
/**
  * @brief  Function implementing the blink_led thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBlink_led */
void StartBlink_led(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

    osDelay(1000); //Every 1000 ms
  }
  osThreadTerminate(NULL);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTransm2 */
/**
* @brief Function implementing the Transmission2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTransm2 */
void StartTransm2(void *argument)
{
  /* USER CODE BEGIN StartTransm2 */
  /* Infinite loop */
  for(;;)
  {
	HAL_UART_Transmit(&huart2,"\n[OBC->PLD]OBC demande A\n",25,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,"A",1,HAL_MAX_DELAY);
    osDelay(1000);
  }
  /* USER CODE END StartTransm2 */
}

/* USER CODE BEGIN Header_PLStatus */
/**
* @brief Function implementing the PLoadStatus thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PLStatus */
void PLStatus(void *argument)
{
  /* USER CODE BEGIN StartPLoad */
	int StatusCount=0;
  /* Infinite loop */
  for(;;)
  {
	HAL_UART_Transmit(&huart2,"[PLD->OBC]CU envoie Status\n",27,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3,"S",1,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3,StatusCount+'0',1,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3,"\n",1,HAL_MAX_DELAY);
	StatusCount= StatusCount<9? StatusCount+1 : 0;
    osDelay(3000);
  }
  /* USER CODE END StartPLoad */
}
/* USER CODE BEGIN Header_StartOBCAsku */
/**
* @brief Function implementing the OBCAskU thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOBCAsku */
void StartOBCAsku(void *argument)
{
  /* USER CODE BEGIN StartOBCAsku */
  int StatusCount=0;
  /* Infinite loop */
  for(;;)
  {
	HAL_UART_Transmit(&huart2,"[OBC->PLD]OBC envoie U\n",22,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,"U",1,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,StatusCount+'0',1,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,"\n",1,HAL_MAX_DELAY);
	StatusCount= StatusCount<9? StatusCount+1 : 0;
    osDelay(2000);
  }
  /* USER CODE END StartOBCAsku */
}

/* USER CODE BEGIN Header_StartOBCAskV */
/**
* @brief Function implementing the OBCAskV thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOBCAskV */
void StartOBCAskV(void *argument)
{
  /* USER CODE BEGIN StartOBCAskV */
	  int StatusCount=0;
	  /* Infinite loop */
	  for(;;)
	  {
		HAL_UART_Transmit(&huart2,"[OBC->PLD]OBC envoie V\n",22,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1,"V",1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1,StatusCount+'0',1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1,"\n",1,HAL_MAX_DELAY);
		StatusCount= StatusCount<9? StatusCount+1 : 0;
	    osDelay(2500);
	  }
  /* USER CODE END StartOBCAskV */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
