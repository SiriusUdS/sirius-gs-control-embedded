/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "GSControl.h"
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
CRC_HandleTypeDef hcrc;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
GPIO gpios[GS_CONTROL_GPIO_AMOUNT] = {0};
UART uart                          = {0};
volatile USB usb                   = {0};
Telecommunication telecom          = {0};
Button buttons[GS_CONTROL_BUTTON_AMOUNT] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI2_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

static void setupGPIOs();
static void setupUART();
static void setupUSB();
static void setupTelecommunication();
static void setupButton();
static void setupDataBridge();

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  //MX_SDIO_SD_Init();
  MX_SPI2_Init();
  //MX_FATFS_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  // Setup Peripherals
  setupGPIOs();
  setupUART();
  setupUSB();
  setupButton();

  // Setup Sensors/Devices
  setupTelecommunication();
  
  GSControl_init(gpios, &uart, &usb, &telecom, &buttons);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  { 
    GSControl_tick(HAL_GetTick());
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
  RCC_OscInitStruct.PLL.PLLN = 96;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
  hsd.Init.ClockDiv = 2;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_OUTPUT_EXT_FLASH_HOLD_Pin|GPIO_OUTPUT_EXT_FLASH_WP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : GPIO_INPUT_SWITCH_ALLOW_FILL_Pin GPIO_INPUT_SWITCH_ARM_VALVE_Pin GPIO_INPUT_SWITCH_ARM_IGNITER_Pin GPIO_INPUT_SWITCH_UNUSED_Pin
                           GPIO_INPUT_BUTTON_EMERGENCY_STOP_Pin GPIO_INPUT_BUTTON_FIRE_IGNITER_Pin GPIO_INPUT_KEY_SWITCH_UNSAFE_Pin */
  GPIO_InitStruct.Pin = GPIO_INPUT_SWITCH_ALLOW_FILL_Pin|GPIO_INPUT_SWITCH_ARM_VALVE_Pin|GPIO_INPUT_SWITCH_ARM_IGNITER_Pin|GPIO_INPUT_SWITCH_UNUSED_Pin
                          |GPIO_INPUT_BUTTON_EMERGENCY_STOP_Pin|GPIO_INPUT_BUTTON_FIRE_IGNITER_Pin|GPIO_INPUT_KEY_SWITCH_UNSAFE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE8 PE9 PE10 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_OUTPUT_EXT_FLASH_HOLD_Pin GPIO_OUTPUT_EXT_FLASH_WP_Pin */
  GPIO_InitStruct.Pin = GPIO_OUTPUT_EXT_FLASH_HOLD_Pin|GPIO_OUTPUT_EXT_FLASH_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CARD_DETECT_PD_Pin */
  GPIO_InitStruct.Pin = SD_CARD_DETECT_PD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(SD_CARD_DETECT_PD_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// These should only link HAL to instance and set base function pointers

void setupGPIOs() {
  gpios[GS_CONTROL_GPIO_ALLOW_FILL_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_ALLOW_FILL_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_ALLOW_FILL_INDEX].pinNumber = GPIO_PIN_7;
  gpios[GS_CONTROL_GPIO_ALLOW_FILL_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_ALLOW_FILL_INDEX].init = (GPIO_init)GPIOHAL_init;

  gpios[GS_CONTROL_GPIO_ARM_VALVE_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_ARM_VALVE_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_ARM_VALVE_INDEX].pinNumber = GPIO_PIN_8;
  gpios[GS_CONTROL_GPIO_ARM_VALVE_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_ARM_VALVE_INDEX].init = (GPIO_init)GPIOHAL_init;

  gpios[GS_CONTROL_GPIO_ARM_IGNITER_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_ARM_IGNITER_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_ARM_IGNITER_INDEX].pinNumber = GPIO_PIN_9;
  gpios[GS_CONTROL_GPIO_ARM_IGNITER_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_ARM_IGNITER_INDEX].init = (GPIO_init)GPIOHAL_init;

  gpios[GS_CONTROL_GPIO_UNUSED_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_UNUSED_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_UNUSED_INDEX].pinNumber = GPIO_PIN_10;
  gpios[GS_CONTROL_GPIO_UNUSED_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_UNUSED_INDEX].init = (GPIO_init)GPIOHAL_init;

  gpios[GS_CONTROL_GPIO_EMERGENCY_STOP_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_EMERGENCY_STOP_INDEX].externalHandle = GPIOA;
  gpios[GS_CONTROL_GPIO_EMERGENCY_STOP_INDEX].pinNumber = GPIO_PIN_4;
  gpios[GS_CONTROL_GPIO_EMERGENCY_STOP_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_EMERGENCY_STOP_INDEX].init = (GPIO_init)GPIOHAL_init;

  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].externalHandle = GPIOA;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].pinNumber = GPIO_PIN_5;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].init = (GPIO_init)GPIOHAL_init;

  gpios[GS_CONTROL_GPIO_UNSAFE_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_UNSAFE_INDEX].externalHandle = GPIOA;
  gpios[GS_CONTROL_GPIO_UNSAFE_INDEX].pinNumber = GPIO_PIN_6;
  gpios[GS_CONTROL_GPIO_UNSAFE_INDEX].mode = GPIO_INPUT_MODE;
  gpios[GS_CONTROL_GPIO_UNSAFE_INDEX].init = (GPIO_init)GPIOHAL_init;
}

void setupUART() {
  uart.errorStatus.bits.notInitialized = 1;
  uart.init = (UART_init)UARTHAL_init;
  uart.externalHandle = &huart1;
}

void setupUSB() {
  usb.errorStatus.bits.notInitialized = 1;
  usb.init = (USB_init)USBHAL_init;

  usbCdc = &usb;
}

void setupTelecommunication(){
  telecom.errorStatus.bits.notInitialized = 1;
  telecom.init = (Telecommunication_init)XBEE_init;
}

void setupButton(){
  for(uint8_t i = 0; i < GS_CONTROL_BUTTON_AMOUNT; i++)
  {
    buttons[i].errorStatus.bits.notInitialized = 1;
    buttons[i].init = (Button_init)ButtonActiveHigh_init;
  }
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
