/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
// 头文件

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
// 导出全局定义

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// 导出常量

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
// 导出函数宏

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
// 声明函数原型

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define bmiINT_Pin GPIO_PIN_13
#define bmiINT_GPIO_Port GPIOC
#define encoderS_Pin GPIO_PIN_0
#define encoderS_GPIO_Port GPIOA
#define encoderA_Pin GPIO_PIN_1
#define encoderA_GPIO_Port GPIOA
#define encoderB_Pin GPIO_PIN_2
#define encoderB_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOA
#define decoderA_Pin GPIO_PIN_6
#define decoderA_GPIO_Port GPIOA
#define decoderB_Pin GPIO_PIN_7
#define decoderB_GPIO_Port GPIOA
#define decoderC_Pin GPIO_PIN_0
#define decoderC_GPIO_Port GPIOB
#define decoderE_Pin GPIO_PIN_2
#define decoderE_GPIO_Port GPIOB
#define lcdRW_Pin GPIO_PIN_12
#define lcdRW_GPIO_Port GPIOB
#define lceE_Pin GPIO_PIN_13
#define lceE_GPIO_Port GPIOB
#define rgb_Pin GPIO_PIN_8
#define rgb_GPIO_Port GPIOA
#define lcdDC_Pin GPIO_PIN_15
#define lcdDC_GPIO_Port GPIOA
#define nrfCE_Pin GPIO_PIN_6
#define nrfCE_GPIO_Port GPIOB
#define nrfIRQ_Pin GPIO_PIN_7
#define nrfIRQ_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
