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
SD_HandleTypeDef hsd;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
GPIO gpios[GS_CONTROL_GPIO_AMOUNT] = {0};
UART uart                          = {0};
volatile USB usb                   = {0};
Telecommunication telecom          = {0};
Button button[6]                   = {0};
DataBridge databridge;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI2_Init(void);
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
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_USB_DEVICE_Init();
  MX_SDIO_SD_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  // Setup Peripherals
  setupGPIOs();
  setupUART();
  setupUSB();
  setupButton();

  // Setup Sensors/Devices
  setupTelecommunication();

  setupDataBridge();

  
  GSControl_init(gpios, &uart, &usb, &telecom, &button, &databridge);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /*Storage storage = {
    .fetchData = SDCard_fetch4kbData,
    .storePage = SDCard_store4kbData,
    .init = SDCard_init,
    .externalInstance = (void*)&SDFatFS,
  };*/

  /*uint8_t data[4096/8] = {0};
  data[0] = 1;
  data[1] = 2;
  data[2] = 3;
  data[3] = 4;*/

  while (1)
  { 
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
    /*data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    storage.storePage(&storage, data);

    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    storage.fetchData(&storage, data);*/
    GSControl_tick(HAL_GetTick());

    uint8_t d[] = "+++";
    //HAL_UART_Transmit(&huart1, d, sizeof(d)-1, HAL_MAX_DELAY);
    uint8_t din[10];
     /*while(1){
      HAL_UART_Receive(&huart1, din, 10, HAL_MAX_DELAY);
      if(din[0] == 'O'){
        break;
      }

      in++;

    }*/
    //HAL_Delay(100);
    // E_MATCH
    /*HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_Delay(1000);*/

    // HEATPAD
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
    /*HAL_Delay(750);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_Delay(250);*/
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
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 6;
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
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_OUTPUT_EXT_FLASH_HOLD_Pin|GPIO_OUTPUT_EXT_FLASH_WP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_OUTPUT_HEATPAD_1_Pin|GPIO_OUTPUT_HEATPAD_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_OUTPUT_EMATCH_1_Pin|GPIO_OUTPUT_EMATCH_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GPIO_INPUT_EMATCH_INDICATOR_2_Pin GPIO_INPUT_EMATCH_INDICATOR_1_Pin GPIO_INPUT_NOS_VALVE_SWITCH_CLOSED_Pin GPIO_INPUT_NOS_VALVE_SWITCH_OPENED_Pin
                           GPIO_INPUT_IPA_VALVE_SWITCH_CLOSED_Pin GPIO_INPUT_IPA_VALVE_SWITCH_OPENED_Pin */
  GPIO_InitStruct.Pin = GPIO_INPUT_EMATCH_INDICATOR_2_Pin|GPIO_INPUT_EMATCH_INDICATOR_1_Pin|GPIO_INPUT_NOS_VALVE_SWITCH_CLOSED_Pin|GPIO_INPUT_NOS_VALVE_SWITCH_OPENED_Pin
                          |GPIO_INPUT_IPA_VALVE_SWITCH_CLOSED_Pin|GPIO_INPUT_IPA_VALVE_SWITCH_OPENED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_PRESSURE_SENSOR_TANK_Pin ADC_PRESSURE_SENSOR_COMBUSTION_CHAMBER_Pin ADC_BATTERY_TENSION_Pin ADC_UNUSED_3_Pin
                           ADC_UNUSED_LOAD_SENSOR_1_Pin ADC_UNUSED_LOAD_SENSOR_2_Pin */
  GPIO_InitStruct.Pin = ADC_PRESSURE_SENSOR_TANK_Pin|ADC_PRESSURE_SENSOR_COMBUSTION_CHAMBER_Pin|ADC_BATTERY_TENSION_Pin|ADC_UNUSED_3_Pin
                          |ADC_UNUSED_LOAD_SENSOR_1_Pin|ADC_UNUSED_LOAD_SENSOR_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_TEMPERATURE_SENSOR_1_Pin ADC_TEMPERATURE_SENSOR_2_Pin ADC_TEMPERATURE_SENSOR_3_Pin ADC_TEMPERATURE_SENSOR_4_Pin
                           ADC_TEMPERATURE_SENSOR_5_Pin ADC_TEMPERATURE_SENSOR_6_Pin ADC_TEMPERATURE_SENSOR_7_Pin ADC_TEMPERATURE_SENSOR_8_Pin */
  GPIO_InitStruct.Pin = ADC_TEMPERATURE_SENSOR_1_Pin|ADC_TEMPERATURE_SENSOR_2_Pin|ADC_TEMPERATURE_SENSOR_3_Pin|ADC_TEMPERATURE_SENSOR_4_Pin
                          |ADC_TEMPERATURE_SENSOR_5_Pin|ADC_TEMPERATURE_SENSOR_6_Pin|ADC_TEMPERATURE_SENSOR_7_Pin|ADC_TEMPERATURE_SENSOR_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_UNUSED_1_Pin ADC_UNUSED_2_Pin */
  GPIO_InitStruct.Pin = ADC_UNUSED_1_Pin|ADC_UNUSED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_OUTPUT_EXT_FLASH_HOLD_Pin GPIO_OUTPUT_EXT_FLASH_WP_Pin */
  GPIO_InitStruct.Pin = GPIO_OUTPUT_EXT_FLASH_HOLD_Pin|GPIO_OUTPUT_EXT_FLASH_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_OUTPUT_HEATPAD_1_Pin GPIO_OUTPUT_HEATPAD_2_Pin */
  GPIO_InitStruct.Pin = GPIO_OUTPUT_HEATPAD_1_Pin|GPIO_OUTPUT_HEATPAD_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_OUTPUT_EMATCH_1_Pin GPIO_OUTPUT_EMATCH_2_Pin */
  GPIO_InitStruct.Pin = GPIO_OUTPUT_EMATCH_1_Pin|GPIO_OUTPUT_EMATCH_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// These should only link HAL to instance and set base function pointers

void setupGPIOs() {
  // SAFE
  gpios[GS_CONTROL_GPIO_SAFE_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_SAFE_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_SAFE_INDEX].pinNumber = GPIO_PIN_7;
  gpios[GS_CONTROL_GPIO_SAFE_INDEX].mode = GPIO_OUTPUT_MODE;
  gpios[GS_CONTROL_GPIO_SAFE_INDEX].init = (GPIO_init)GPIOHAL_init;

  // FIRE IGNITER
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].pinNumber = GPIO_PIN_8;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].mode = GPIO_OUTPUT_MODE;
  gpios[GS_CONTROL_GPIO_FIRE_IGNITER_INDEX].init = (GPIO_init)GPIOHAL_init;

  // ENABLE FILL
  gpios[GS_CONTROL_GPIO_ENABLE_FILL_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_ENABLE_FILL_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_ENABLE_FILL_INDEX].pinNumber = GPIO_PIN_9;
  gpios[GS_CONTROL_GPIO_ENABLE_FILL_INDEX].mode = GPIO_OUTPUT_MODE;
  gpios[GS_CONTROL_GPIO_ENABLE_FILL_INDEX].init = (GPIO_init)GPIOHAL_init;

  // BACKUP
  gpios[GS_CONTROL_GPIO_BACKUP_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_BACKUP_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_BACKUP_INDEX].pinNumber = GPIO_PIN_10;
  gpios[GS_CONTROL_GPIO_BACKUP_INDEX].mode = GPIO_OUTPUT_MODE;
  gpios[GS_CONTROL_GPIO_BACKUP_INDEX].init = (GPIO_init)GPIOHAL_init;

  // IGNITER ARMED
  gpios[GS_CONTROL_GPIO_IGNITER_ARMED_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_IGNITER_ARMED_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_IGNITER_ARMED_INDEX].pinNumber = GPIO_PIN_11;
  gpios[GS_CONTROL_GPIO_IGNITER_ARMED_INDEX].mode = GPIO_OUTPUT_MODE;
  gpios[GS_CONTROL_GPIO_IGNITER_ARMED_INDEX].init = (GPIO_init)GPIOHAL_init;

  // ROCKET ARMED
  gpios[GS_CONTROL_GPIO_ROCKET_ARMED_INDEX].errorStatus.bits.notInitialized = 1;
  gpios[GS_CONTROL_GPIO_ROCKET_ARMED_INDEX].externalHandle = GPIOE;
  gpios[GS_CONTROL_GPIO_ROCKET_ARMED_INDEX].pinNumber = GPIO_PIN_12;
  gpios[GS_CONTROL_GPIO_ROCKET_ARMED_INDEX].mode = GPIO_OUTPUT_MODE;
  gpios[GS_CONTROL_GPIO_ROCKET_ARMED_INDEX].init = (GPIO_init)GPIOHAL_init;
}

void setupDataBridge(){
  databridge.init = DATABRIDGE_init;

  databridge.uart = &uart;
  databridge.usb = &usb;
  databridge.xbee = &telecom;
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
  for(uint8_t i=0; i<GS_CONTROL_BUTTON_AMOUNT;i++)
  {
    button[i].errorStatus.bits.notInitialized = 1;
    button[i].init = (Button_init)ButtonActiveLow_init;
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
