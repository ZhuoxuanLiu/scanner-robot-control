/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "queue.h"
#include "motor.h"
/* USER CODE BEGIN 0 */
uint8_t USART_RX_BUF[USART_REC_LEN];
uint8_t RxBuffer[RXBUFFERSIZE];
uint16_t USART_RX_STA = 0;
uint32_t UART_Send_timeout = 0;

uint8_t USART_TX_BUF[USART_TRA_LEN];
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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
  QueueInit(recv_queue);
  HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer, RXBUFFERSIZE);
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		if((USART_RX_STA&0x8000)==0)
		{
			if(USART_RX_STA&0x4000)
			{
				if(RxBuffer[0]!=0x0a) USART_RX_STA=0;
				else 
        {
          uint8_t queue_buf[4];
          if (USART_RX_BUF[1]==Head_motor || USART_RX_BUF[1]==Body_motor || 
          USART_RX_BUF[1]==Pressing_board_motor || USART_RX_BUF[1]==Rotating_shelf_motor)
          {
            QueuePush(htim6_queue, USART_RX_BUF);
          }
          else if ((USART_RX_BUF[1]==Base_motor || USART_RX_BUF[1]==Lift_motor || 
          USART_RX_BUF[1]==Pushing_book_motor || USART_RX_BUF[1]==Forward_pressing_board_motor))
          {
            QueuePush(htim7_queue, USART_RX_BUF);
          }
          else
          {
            QueuePush(extra_queue, USART_RX_BUF);
          }
          USART_RX_STA = 0;
        }
			}
			else
			{
				if(RxBuffer[0]==0x0d) USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=RxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1)) USART_RX_STA=0;
				}
			}
		}
		uint32_t timeout=0;
		while(HAL_UART_Receive_IT(huart, (uint8_t *)RxBuffer, RXBUFFERSIZE) != HAL_OK)
		{
		 timeout++;
		 if(timeout>HAL_MAX_DELAY) break;
		}
	}
}

void UART_FlagClear(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
}

void UART_Send(uint8_t *send_data)
{
	while(HAL_UART_Transmit(&huart1, send_data, (uint16_t) sizeof(send_data), HAL_MAX_DELAY)!= HAL_OK)
	{
		UART_FlagClear(&huart1);
	}
}


/* USER CODE END 1 */
