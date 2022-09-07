/*
 * motor.h
 *
 *  Created on: Jun 16, 2022
 *      Author: a3352
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "sensor.h"
#include "main.h"

#define FORWARD  GPIO_PIN_SET
#define BACKWARD GPIO_PIN_RESET

#define FORWARD_PERIOD  	'1'
#define BACKWARD_PERIOD 	'0'


/* Define timer -------------------------------------*/

#define Base_channel                    TIM_CHANNEL_1   //htim7
#define Head_channel                    TIM_CHANNEL_1   //htim6
#define Body_channel                    TIM_CHANNEL_2   //htim6
#define Lift_channel                    TIM_CHANNEL_2   //htim7
#define Pushing_book_channel            TIM_CHANNEL_3   //htim7
#define Pressing_board_channel          TIM_CHANNEL_3   //htim6
#define Forward_pressing_board_channel  TIM_CHANNEL_4   //htim7
#define Rotating_shelf_channel          TIM_CHANNEL_4   //htim6

#define Base_motor                    '0'
#define Body_motor                    '1'
#define Head_motor                    '2'
#define Lift_motor                    '3'
#define Pushing_book_motor            '4'
#define Forward_pressing_board_motor  '5'
#define Pressing_board_motor          '6'
#define Rotating_shelf_motor          '7'

/* Define timer -------------------------------------*/

#define SET_Forward_DIR(x)                         HAL_GPIO_WritePin(x->GPIO_Port,x->GPIO_Pin,FORWARD)
#define SET_Backward_DIR(x)                        HAL_GPIO_WritePin(x->GPIO_Port,x->GPIO_Pin,BACKWARD)

typedef struct Motor
{
    GPIO_TypeDef * GPIO_Port;
    uint16_t GPIO_Pin;
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    uint16_t deg;
    uint16_t motor_div;
    uint16_t rratio;
    uint32_t pwm_us;
    uint8_t name;
    uint8_t power;
    uint8_t position;
    uint8_t check_sensor_period;
    uint8_t check_sensor_dir;
}Motor;

extern Motor *base_motor;

extern Motor *body_motor;

extern Motor *head_motor;

extern Motor *lift_motor;

extern Motor *pushing_book_motor;

extern Motor *forward_pressing_board_motor;

extern Motor *pressing_board_motor;

extern Motor *rotating_shelf_motor;

extern Motor *Current_tim6_motor;

extern Motor *Current_tim7_motor;

extern uint8_t TIM6_stat;

extern uint8_t TIM7_stat;

void Forward_motor(Motor *motor, float per);

void Backward_motor(Motor *motor, float per);

void Stop_motor(Motor *motor);


#endif /* INC_MOTOR_H_ */
