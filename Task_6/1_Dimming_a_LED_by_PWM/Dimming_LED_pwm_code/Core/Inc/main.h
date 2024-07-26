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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern TIM_OC_InitTypeDef sConfigOC;
extern TIM_HandleTypeDef htim1;
extern  TIM_HandleTypeDef htim3;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_1_Pin GPIO_PIN_0
#define LED_1_GPIO_Port GPIOB
#define STICK_CENTER_Pin GPIO_PIN_2
#define STICK_CENTER_GPIO_Port GPIOB
#define STICK_CENTER_EXTI_IRQn EXTI2_3_IRQn
#define STICK_RIGHT_Pin GPIO_PIN_10
#define STICK_RIGHT_GPIO_Port GPIOB
#define STICK_RIGHT_EXTI_IRQn EXTI4_15_IRQn
#define STICK_UP_Pin GPIO_PIN_11
#define STICK_UP_GPIO_Port GPIOB
#define STICK_UP_EXTI_IRQn EXTI4_15_IRQn
#define STICK_LEFT_Pin GPIO_PIN_12
#define STICK_LEFT_GPIO_Port GPIOB
#define STICK_LEFT_EXTI_IRQn EXTI4_15_IRQn
#define STICK_DOWN_Pin GPIO_PIN_13
#define STICK_DOWN_GPIO_Port GPIOB
#define STICK_DOWN_EXTI_IRQn EXTI4_15_IRQn
#define LED_extern_Pin GPIO_PIN_9
#define LED_extern_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
