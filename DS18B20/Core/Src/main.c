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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

#include "OneWire.h"
#include "DallasTemperature.h"

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

/* USER CODE BEGIN PV */
// define structures
OneWire_HandleTypeDef ow;
DallasTemperature_HandleTypeDef dt;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// redirect output of fprint() to UART2
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

// print device address
void printAddress(CurrentDeviceAddress deviceAddress)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		printf("0x%02X", deviceAddress[i]);
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  	  // now printf prints to Serial via UART2 because we redefine _write()
	printf("Serial started at %d baud rate.\r\n", huart2.Init.BaudRate);

	printf("[%8lu] Debug UART2 is OK!\r\n", HAL_GetTick());

	// start One Wire on UATR1
	OW_Begin(&ow, &huart1);
	if(OW_Reset(&ow) == OW_OK)
	{
	  printf("[%8lu] OneWire devices are present :)\r\n", HAL_GetTick());
	}
	else
	{
	  printf("[%8lu] OneWire no devices :(\r\n", HAL_GetTick());
	}

	DT_SetOneWire(&dt, &ow);

	// arrays to hold device address
	CurrentDeviceAddress insideThermometer;

	// locate devices on the bus
	printf("[%8lu] Locating devices...", HAL_GetTick());

	DT_Begin(&dt);

	printf("[%8lu] Found %d devices.\r\n", HAL_GetTick(), DT_GetDeviceCount(&dt));

	if (!DT_GetAddress(&dt, insideThermometer, 0))
	  printf("[%8lu] Unable to find address for Device 0\r\n", HAL_GetTick());

	printf("[%8lu] Device 0 Address: ", HAL_GetTick());
	printAddress(insideThermometer);
	printf("\r\n");

	// set the resolution to 12 bit (Each Dallas/Maxim device is capable of several different resolutions)
	DT_SetResolution(&dt, insideThermometer, 12, true);

	printf("[%8lu] Device 0 Resolution: %d\r\n", HAL_GetTick(), DT_GetResolution(&dt, insideThermometer));

	if (!DT_GetAddress(&dt, insideThermometer, 1))
	  printf("[%8lu] Unable to find address for Device 1\r\n", HAL_GetTick());

	printf("[%8lu] Device 1 Address: ", HAL_GetTick());
	printAddress(insideThermometer);
	printf("\r\n");

	// set the resolution to 12 bit (Each Dallas/Maxim device is capable of several different resolutions)
	DT_SetResolution(&dt, insideThermometer, 12, true);

	printf("[%8lu] Device 1 Resolution: %d\r\n", HAL_GetTick(), DT_GetResolution(&dt, insideThermometer));
	/* USER CODE END 2 */


  /* USER CODE END 2 */

//  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	// call DT_RequestTemperatures(&dt) to issue a global temperature
	// request to all devices on the bus
	printf("[%8lu] Requesting temperatures...", HAL_GetTick());
	DT_RequestTemperatures(&dt); // Send the command to get temperatures
	printf("\r\n[%8lu] DONE\r\n", HAL_GetTick());
	// After we got the temperatures, we can print them here.
	// We use the function ByIndex, and as an example get the temperature from the first sensor only.
	printf("[%8lu] Temperature for the device 1 (index 0) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 0));
	printf("[%8lu] Temperature for the device 2 (index 1) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 1));
	HAL_Delay(2000);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
