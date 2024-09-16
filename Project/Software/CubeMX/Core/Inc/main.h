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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OPT_LED_Pin GPIO_PIN_1
#define OPT_LED_GPIO_Port GPIOA
#define DFPlayer_USART_TX_Pin GPIO_PIN_2
#define DFPlayer_USART_TX_GPIO_Port GPIOA
#define DFPlayer_USART_RX_Pin GPIO_PIN_3
#define DFPlayer_USART_RX_GPIO_Port GPIOA
#define Battery_Indicator_Pin GPIO_PIN_7
#define Battery_Indicator_GPIO_Port GPIOA
#define DFPlayer_On_Off_Pin GPIO_PIN_0
#define DFPlayer_On_Off_GPIO_Port GPIOB
#define Button_1_Pin GPIO_PIN_10
#define Button_1_GPIO_Port GPIOB
#define Button_1_EXTI_IRQn EXTI15_10_IRQn
#define Button_2_Pin GPIO_PIN_11
#define Button_2_GPIO_Port GPIOB
#define Button_2_EXTI_IRQn EXTI15_10_IRQn
#define Button_3_Pin GPIO_PIN_12
#define Button_3_GPIO_Port GPIOB
#define Button_3_EXTI_IRQn EXTI15_10_IRQn
#define Button_4_Pin GPIO_PIN_13
#define Button_4_GPIO_Port GPIOB
#define Button_4_EXTI_IRQn EXTI15_10_IRQn
#define Rotary_Encoder_B_Pin GPIO_PIN_8
#define Rotary_Encoder_B_GPIO_Port GPIOA
#define Rotary_Encoder_B_EXTI_IRQn EXTI9_5_IRQn
#define Rotary_Encoder_A_Pin GPIO_PIN_9
#define Rotary_Encoder_A_GPIO_Port GPIOA
#define Rotary_Encoder_A_EXTI_IRQn EXTI9_5_IRQn
#define OPT_UART_TX_Pin GPIO_PIN_10
#define OPT_UART_TX_GPIO_Port GPIOC
#define OPT_UART_RX_Pin GPIO_PIN_11
#define OPT_UART_RX_GPIO_Port GPIOC
#define Display_Reset_Pin GPIO_PIN_3
#define Display_Reset_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
