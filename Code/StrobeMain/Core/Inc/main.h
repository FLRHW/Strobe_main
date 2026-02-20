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
#include "stm32g4xx_ll_adc.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_crs.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_usart.h"
#include "stm32g4xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

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
#define U_SH1000_Pin LL_GPIO_PIN_0
#define U_SH1000_GPIO_Port GPIOA
#define U_SH240_Pin LL_GPIO_PIN_1
#define U_SH240_GPIO_Port GPIOA
#define I_SH240_Pin LL_GPIO_PIN_2
#define I_SH240_GPIO_Port GPIOA
#define I_SH1000_Pin LL_GPIO_PIN_3
#define I_SH1000_GPIO_Port GPIOA
#define U_L_Pin LL_GPIO_PIN_4
#define U_L_GPIO_Port GPIOA
#define I_P_Pin LL_GPIO_PIN_5
#define I_P_GPIO_Port GPIOA
#define U_C_Pin LL_GPIO_PIN_6
#define U_C_GPIO_Port GPIOA
#define TP511_Pin LL_GPIO_PIN_7
#define TP511_GPIO_Port GPIOA
#define TP506_Pin LL_GPIO_PIN_0
#define TP506_GPIO_Port GPIOB
#define ST_LED_Pin LL_GPIO_PIN_8
#define ST_LED_GPIO_Port GPIOA
#define LAMP_EN_Pin LL_GPIO_PIN_11
#define LAMP_EN_GPIO_Port GPIOA
#define TP516_Pin LL_GPIO_PIN_12
#define TP516_GPIO_Port GPIOA
#define TP508_Pin LL_GPIO_PIN_3
#define TP508_GPIO_Port GPIOB
#define TP510_Pin LL_GPIO_PIN_4
#define TP510_GPIO_Port GPIOB
#define TP512_Pin LL_GPIO_PIN_5
#define TP512_GPIO_Port GPIOB
#define HV_TX_Pin LL_GPIO_PIN_6
#define HV_TX_GPIO_Port GPIOB
#define HV_RX_Pin LL_GPIO_PIN_7
#define HV_RX_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
