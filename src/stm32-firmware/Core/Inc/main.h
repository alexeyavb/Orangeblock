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
#include <memory.h>
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
#define LED_PIN_Pin GPIO_PIN_13
#define LED_PIN_GPIO_Port GPIOC
#define BUTTON_KEY_Pin GPIO_PIN_0
#define BUTTON_KEY_GPIO_Port GPIOA
#define BUTTON_KEY_EXTI_IRQn EXTI0_IRQn
#define CONSOLE_TX_Pin GPIO_PIN_2
#define CONSOLE_TX_GPIO_Port GPIOA
#define CONSOLE_RX_Pin GPIO_PIN_3
#define CONSOLE_RX_GPIO_Port GPIOA
#define I2S1_LRCK_Pin GPIO_PIN_4
#define I2S1_LRCK_GPIO_Port GPIOA
#define I2S1_BCK_Pin GPIO_PIN_5
#define I2S1_BCK_GPIO_Port GPIOA
#define I2S2_SCK_Pin GPIO_PIN_6
#define I2S2_SCK_GPIO_Port GPIOA
#define I2S1_DATA_OUT_Pin GPIO_PIN_7
#define I2S1_DATA_OUT_GPIO_Port GPIOA
#define I2S2_LRCK_Pin GPIO_PIN_12
#define I2S2_LRCK_GPIO_Port GPIOB
#define I2S2_BCK_Pin GPIO_PIN_13
#define I2S2_BCK_GPIO_Port GPIOB
#define I2S2_DATA_IN_Pin GPIO_PIN_15
#define I2S2_DATA_IN_GPIO_Port GPIOB
#define PCM0_SYNC_Pin GPIO_PIN_15
#define PCM0_SYNC_GPIO_Port GPIOA
#define PCM0_CLK_Pin GPIO_PIN_3
#define PCM0_CLK_GPIO_Port GPIOB
#define PCM0_OUT_Pin GPIO_PIN_4
#define PCM0_OUT_GPIO_Port GPIOB
#define PCM0_INP_Pin GPIO_PIN_5
#define PCM0_INP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
