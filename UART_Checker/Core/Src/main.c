/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "ssd1306_hal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	SETTING			= 0,
	RECEIVE_TEST,
	TRANSMIT_TEST,
	MONITOR_MODE,
} UART_Config_t;

typedef enum {
	BAUDRATE	= 0,
	RECEIVE,
	TRANSMIT,
	MONITOR,
} UART_Setting_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t baudrate = 115200;
uint32_t baudrate_select[10] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800};

uint8_t receive_num = 1;
uint8_t receive_buf[32] = {0};

uint8_t transmit_num = 1;
uint8_t transmi_buf[32] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void UART_Setting(void);
void Receive_Test(void);
void Transmit_Test(void);
void Monitor_Mode(void);
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init(&hi2c1, OLED_Init_Data, sizeof(OLED_Init_Data));

  OLED_Thrcot_Large_Logo_Display(&hi2c1);
  HAL_Delay(2000);
  OLED_AllClear(&hi2c1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  static UART_Config_t config = SETTING;

	  OLED_DataClear();
	  OLED_Char_Print("Select to do.", 0, 0);
	  OLED_Char_Print(" Settings", 0, 8);
	  OLED_Char_Print(" Receive test", 0, 16);
	  OLED_Char_Print(" Transmit test", 0, 24);
	  OLED_Char_Print(" SerialMonitor", 0, 32);
	  OLED_Char_Print(">", 0, config * 8 + 8);
	  OLED_Display(&hi2c1);

	  if (HAL_GPIO_ReadPin(decide_sw_GPIO_Port, decide_sw_Pin) == 1) {
		  while (HAL_GPIO_ReadPin(decide_sw_GPIO_Port, decide_sw_Pin) == 1);

		  switch (config) {
			case SETTING:
				UART_Setting();
				break;

			case RECEIVE_TEST:

				break;

			case TRANSMIT_TEST:

				break;

			case MONITOR_MODE:

				break;
		}
	  }

	  if (HAL_GPIO_ReadPin(leftup_sw_GPIO_Port, leftup_sw_Pin) == 1) {
		  while (HAL_GPIO_ReadPin(leftup_sw_GPIO_Port, leftup_sw_Pin) == 1);
		  config = (config == SETTING) ? MONITOR_MODE : config - 1;
	  }

	  if (HAL_GPIO_ReadPin(leftdown_sw_GPIO_Port, leftdown_sw_Pin) == 1) {
		  while (HAL_GPIO_ReadPin(leftdown_sw_GPIO_Port, leftdown_sw_Pin) == 1);
		  config = (config == MONITOR_MODE) ? SETTING : config + 1;
	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0000020B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : leftup_sw_Pin leftdown_sw_Pin decide_sw_Pin */
  GPIO_InitStruct.Pin = leftup_sw_Pin|leftdown_sw_Pin|decide_sw_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : rightdown_sw_Pin rightup_sw_Pin */
  GPIO_InitStruct.Pin = rightdown_sw_Pin|rightup_sw_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void UART_Setting(void)
{
	UART_Setting_t setting = BAUDRATE;

	while (1) {
		OLED_DataClear();
		OLED_Char_Print("Select the setting", 0, 0);
		OLED_Char_Print(" Baud Rate", 0, 8);
		OLED_Char_Print(" Receive", 0, 16);
		OLED_Char_Print(" Transmit", 0, 24);
		OLED_Char_Print(" SerialMonitor", 0, 32);
		OLED_Char_Print(">", 0, setting * 8 + 8);
		OLED_Display(&hi2c1);

		if (HAL_GPIO_ReadPin(rightup_sw_GPIO_Port, rightup_sw_Pin) == 1 && HAL_GPIO_ReadPin(rightdown_sw_GPIO_Port, rightdown_sw_Pin) == 1) {
			break;
		}

		if (HAL_GPIO_ReadPin(decide_sw_GPIO_Port, decide_sw_Pin) == 1) {
			while (HAL_GPIO_ReadPin(decide_sw_GPIO_Port, decide_sw_Pin) == 1);

			switch (setting) {
				case BAUDRATE:
					int select_i = 0;
					int rate_i = 0;

					while (1) {
						OLED_DataClear();
						OLED_Char_Print("Now BaudRate:", 0, 0);
						OLED_Int_Print(baudrate, 78, 0);
						OLED_Char_Print(" BaudRate:", 0, 8);
						OLED_Int_Print(baudrate_select[rate_i], 60, 8);
						OLED_Char_Print(" Change BaudRate", 0, 16);
						OLED_Char_Print(">", 0, select_i * 8 + 8);
						OLED_Display(&hi2c1);

						if (HAL_GPIO_ReadPin(rightup_sw_GPIO_Port, rightup_sw_Pin) == 1 && HAL_GPIO_ReadPin(rightdown_sw_GPIO_Port, rightdown_sw_Pin) == 1) {
							break;
						}

						if (HAL_GPIO_ReadPin(rightup_sw_GPIO_Port, rightup_sw_Pin) == 1 && select_i == 0) {
							while (HAL_GPIO_ReadPin(rightup_sw_GPIO_Port, rightup_sw_Pin) == 1);
							rate_i = (rate_i == 9) ? 9 : rate_i + 1;
						}

						if (HAL_GPIO_ReadPin(rightdown_sw_GPIO_Port, rightdown_sw_Pin) == 1 && select_i == 0) {
							while (HAL_GPIO_ReadPin(rightdown_sw_GPIO_Port, rightdown_sw_Pin) == 1);
							rate_i = (rate_i == 0) ? 0 : rate_i - 1;
						}

						if (HAL_GPIO_ReadPin(decide_sw_GPIO_Port, decide_sw_Pin) == 1 && select_i == 1) {
							while (HAL_GPIO_ReadPin(decide_sw_GPIO_Port, decide_sw_Pin) == 1);

							baudrate = baudrate_select[rate_i];

							HAL_UART_DeInit(&huart2);
							huart2.Init.BaudRate = baudrate;

							if (HAL_UART_Init(&huart2) != HAL_OK) {
								Error_Handler();
							}

							break;
						}

						if (HAL_GPIO_ReadPin(leftup_sw_GPIO_Port, leftup_sw_Pin) == 1) {
							while (HAL_GPIO_ReadPin(leftup_sw_GPIO_Port, leftup_sw_Pin) == 1);
							select_i = (select_i == 0) ? 1 : 0;
						}

						if (HAL_GPIO_ReadPin(leftdown_sw_GPIO_Port, leftdown_sw_Pin) == 1) {
							while (HAL_GPIO_ReadPin(leftdown_sw_GPIO_Port, leftdown_sw_Pin) == 1);
							select_i = (select_i == 1) ? 0 : 1;
						}
					}

					while (HAL_GPIO_ReadPin(rightup_sw_GPIO_Port, rightup_sw_Pin) == 1 || HAL_GPIO_ReadPin(rightdown_sw_GPIO_Port, rightdown_sw_Pin) == 1);

					break;

				case RECEIVE:

					break;

				case TRANSMIT:

					break;

				case MONITOR:

					break;
			}
		}

		if (HAL_GPIO_ReadPin(leftup_sw_GPIO_Port, leftup_sw_Pin) == 1) {
			while (HAL_GPIO_ReadPin(leftup_sw_GPIO_Port, leftup_sw_Pin) == 1);
			setting = (setting == BAUDRATE) ? MONITOR : setting - 1;
		}

		if (HAL_GPIO_ReadPin(leftdown_sw_GPIO_Port, leftdown_sw_Pin) == 1) {
			while (HAL_GPIO_ReadPin(leftdown_sw_GPIO_Port, leftdown_sw_Pin) == 1);
			setting = (setting == MONITOR) ? BAUDRATE : setting + 1;
		}
	}

	while (HAL_GPIO_ReadPin(rightup_sw_GPIO_Port, rightup_sw_Pin) == 1 || HAL_GPIO_ReadPin(rightdown_sw_GPIO_Port, rightdown_sw_Pin) == 1);
}
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
