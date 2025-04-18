/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_BUILTIN_Pin GPIO_PIN_13
#define LED_BUILTIN_GPIO_Port GPIOC
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOC
#define BUTTON_Pin GPIO_PIN_1
#define BUTTON_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOD
#define LCD_TE_Pin GPIO_PIN_11
#define LCD_TE_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOD
#define LCD_EN_Pin GPIO_PIN_9
#define LCD_EN_GPIO_Port GPIOA
#define CAM_EN_Pin GPIO_PIN_10
#define CAM_EN_GPIO_Port GPIOA
#define MPU_INT_Pin GPIO_PIN_15
#define MPU_INT_GPIO_Port GPIOA
#define SD_DETECT_Pin GPIO_PIN_3
#define SD_DETECT_GPIO_Port GPIOD
#define CAM_RST_Pin GPIO_PIN_6
#define CAM_RST_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
