/*
 * motor.h
 *
 *  Created on: Jun 16, 2022
 *      Author: a3352
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include "usart.h"
#include "sensor.h"

#define FORWARD  GPIO_PIN_SET
#define BACKWARD GPIO_PIN_RESET

/* Define on off state -------------------------------------*/
#define HIGH          1
#define LOW           0
#define ROTATED       1
#define STRETCHED     1
#define RESETED       0
#define FRONT         1
#define BACK          0
#define ON            1
#define OFF           0
#define TRUE          1
#define FALSE         0
/* Define true false state -------------------------------------*/

/* Define timer for every movement -------------------------------------*/

#define Base_channel                    TIM_CHANNEL_1   //htim7
#define Head_channel                    TIM_CHANNEL_1   //htim6
#define Body_channel                    TIM_CHANNEL_2   //htim6
#define Lift_channel                    TIM_CHANNEL_2   //htim7
#define Pushing_book_channel            TIM_CHANNEL_3   //htim7
#define Pressing_board_channel          TIM_CHANNEL_3   //htim6
#define Forward_pressing_board_channel  TIM_CHANNEL_4   //htim7
#define Rotating_shelf_channel          TIM_CHANNEL_4   //htim6

/* Define timer for every movement  -------------------------------------*/

#define Base_DIR(x)                         HAL_GPIO_WritePin(Base_DIR_GPIO_Port,Base_DIR_Pin,x)
#define Body_DIR(x)                         HAL_GPIO_WritePin(Body_DIR_GPIO_Port,Body_DIR_Pin,x)
#define Head_DIR(x)                         HAL_GPIO_WritePin(Head_DIR_GPIO_Port,Head_DIR_Pin,x)
#define Lift_DIR(x)                         HAL_GPIO_WritePin(Lift_DIR_GPIO_Port,Lift_DIR_Pin,x)
#define Pushing_book_DIR(x)                 HAL_GPIO_WritePin(Pushing_book_DIR_GPIO_Port,Pushing_book_DIR_Pin,x)
#define Forward_pressing_board_DIR(x)       HAL_GPIO_WritePin(Forward_pressing_board_DIR_GPIO_Port,Forward_pressing_board_DIR_Pin,x)
#define Pressing_board_DIR(x)               HAL_GPIO_WritePin(Pressing_board_DIR_GPIO_Port,Pressing_board_DIR_Pin,x)
#define Rotating_shelf_DIR(x)               HAL_GPIO_WritePin(Rotating_shelf_DIR_GPIO_Port,Rotating_shelf_DIR_Pin,x)

extern uint8_t Base_Power;
extern uint8_t Base_Position;
extern uint8_t Base_Check_Period;

extern uint8_t Body_Power;
extern uint8_t Body_Position;
extern uint8_t Body_Check_Period;

extern uint8_t Head_Power;
extern uint8_t Head_Position;

extern uint8_t Lift_Power;
extern uint8_t Lift_Position;
extern uint8_t Lift_Check_Period;

extern uint8_t Pushing_book_Power;
extern uint8_t Pushing_book_Position;

extern uint8_t Forward_pressing_board_Power;
extern uint8_t Forward_pressing_board_Position;

extern uint8_t Pressing_board_Power;
extern uint8_t Pressing_board_Position;
extern uint8_t Pressing_board_Check_Period;

extern uint8_t Rotating_shelf_Power;
extern uint8_t Rotating_shelf_Position;

void Start_TIM2_Motor(uint16_t deg, uint16_t motor_div, uint16_t rratio, uint32_t pwm_us, uint32_t channel);
void Stop_TIM2_Motor(void);
void Start_TIM3_Motor(uint16_t deg, uint16_t motor_div, uint16_t rratio, uint32_t pwm_us, uint32_t channel);
void Stop_TIM3_Motor(void);


void Flip_Base(void);

void Reset_Base(void);

void Stop_Base(void);

void Stretch_Body(void);

void Lower_Body(void);

void Stop_Body(void);

void Rotate_Head(void);

void Reset_Head(void);

void Stop_Head(void);

void Lift_book(void);

void Lower_lifter(void);

void Stop_Lift(void);

void Forward_Pushing_book(void);

void Backward_Pushing_book(void);

void Stop_Pushing_book(void);

void Elevate_Pressing_board(void);

void Lower_Pressing_board(void);

void Stop_Pressing_board(void);

void Forward_Pressing_board(void);

void Backward_Pressing_board(void);

void Stop_Forward_Pressing_board(void);

void Rotate_shelf(void);

void Reset_shelf(void);

void Stop_Rotating_shelf(void);


#endif /* INC_MOTOR_H_ */
