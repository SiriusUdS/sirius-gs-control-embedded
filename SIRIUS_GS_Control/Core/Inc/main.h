/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_INPUT_SWITCH_ALLOW_FILL_Pin GPIO_PIN_0
#define GPIO_INPUT_SWITCH_ALLOW_FILL_GPIO_Port GPIOA
#define GPIO_INPUT_SWITCH_ARM_VALVE_Pin GPIO_PIN_1
#define GPIO_INPUT_SWITCH_ARM_VALVE_GPIO_Port GPIOA
#define GPIO_INPUT_SWITCH_ARM_IGNITER_Pin GPIO_PIN_2
#define GPIO_INPUT_SWITCH_ARM_IGNITER_GPIO_Port GPIOA
#define GPIO_INPUT_SWITCH_UNUSED_Pin GPIO_PIN_3
#define GPIO_INPUT_SWITCH_UNUSED_GPIO_Port GPIOA
#define GPIO_INPUT_BUTTON_EMERGENCY_STOP_Pin GPIO_PIN_4
#define GPIO_INPUT_BUTTON_EMERGENCY_STOP_GPIO_Port GPIOA
#define GPIO_INPUT_BUTTON_FIRE_IGNITER_Pin GPIO_PIN_5
#define GPIO_INPUT_BUTTON_FIRE_IGNITER_GPIO_Port GPIOA
#define GPIO_INPUT_KEY_SWITCH_UNSAFE_Pin GPIO_PIN_6
#define GPIO_INPUT_KEY_SWITCH_UNSAFE_GPIO_Port GPIOA
#define GPIO_OUTPUT_EXT_FLASH_HOLD_Pin GPIO_PIN_8
#define GPIO_OUTPUT_EXT_FLASH_HOLD_GPIO_Port GPIOD
#define GPIO_OUTPUT_EXT_FLASH_WP_Pin GPIO_PIN_9
#define GPIO_OUTPUT_EXT_FLASH_WP_GPIO_Port GPIOD
#define SD_CARD_DETECT_PD_Pin GPIO_PIN_15
#define SD_CARD_DETECT_PD_GPIO_Port GPIOA
#define SDIO_D0_Pin GPIO_PIN_4
#define SDIO_D0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
