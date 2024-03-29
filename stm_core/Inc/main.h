/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Base_DIR_Pin GPIO_PIN_0
#define Base_DIR_GPIO_Port GPIOC
#define Body_DIR_Pin GPIO_PIN_1
#define Body_DIR_GPIO_Port GPIOC
#define Head_DIR_Pin GPIO_PIN_2
#define Head_DIR_GPIO_Port GPIOC
#define Lift_DIR_Pin GPIO_PIN_3
#define Lift_DIR_GPIO_Port GPIOC
#define Pressing_board_motor_Pin GPIO_PIN_2
#define Pressing_board_motor_GPIO_Port GPIOA
#define Rotating_shelf_motor_Pin GPIO_PIN_3
#define Rotating_shelf_motor_GPIO_Port GPIOA
#define Head_sensor_Pin GPIO_PIN_4
#define Head_sensor_GPIO_Port GPIOA
#define Pressing_board_1_Pin GPIO_PIN_5
#define Pressing_board_1_GPIO_Port GPIOA
#define Pressing_board_2_Pin GPIO_PIN_6
#define Pressing_board_2_GPIO_Port GPIOA
#define Lift_sensor_Pin GPIO_PIN_7
#define Lift_sensor_GPIO_Port GPIOA
#define Forward_pressing_board_DIR_Pin GPIO_PIN_6
#define Forward_pressing_board_DIR_GPIO_Port GPIOG
#define Pressing_board_DIR_Pin GPIO_PIN_7
#define Pressing_board_DIR_GPIO_Port GPIOG
#define Rotating_shelf_DIR_Pin GPIO_PIN_8
#define Rotating_shelf_DIR_GPIO_Port GPIOG
#define Base_motor_Pin GPIO_PIN_6
#define Base_motor_GPIO_Port GPIOC
#define Lift_motor_Pin GPIO_PIN_7
#define Lift_motor_GPIO_Port GPIOC
#define Pushing_book_motor_Pin GPIO_PIN_8
#define Pushing_book_motor_GPIO_Port GPIOC
#define Forward_pressing_board_motor_Pin GPIO_PIN_9
#define Forward_pressing_board_motor_GPIO_Port GPIOC
#define Head_motor_Pin GPIO_PIN_15
#define Head_motor_GPIO_Port GPIOA
#define Pushing_book_DIR_Pin GPIO_PIN_15
#define Pushing_book_DIR_GPIO_Port GPIOG
#define Body_motor_Pin GPIO_PIN_3
#define Body_motor_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_5
#define LED0_GPIO_Port GPIOB
#define Vacuum_pump_Pin GPIO_PIN_0
#define Vacuum_pump_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#ifndef CPL
#define CPL             '1'
#endif

#ifndef NCPL
#define NCPL            '0'
#endif

#define TRUE          	'1'
#define FALSE         	'0'

#define NOT_RESETED   	'1'
#define RESETED       	'0'
#define ON            	'1'
#define OFF           	'0'

#define NONE			      'n'
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
