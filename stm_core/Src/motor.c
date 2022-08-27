/*
 * motor.c
 *
 *  Created on: Jun 16, 2022
 *      Author: a3352
 */
#include "motor.h"
#include "tim.h"
#include "usart.h"
#include "protocol.h"

float step_deg = 1.8;

Motor *Current_tim6_motor;
Motor *Current_tim7_motor;
uint8_t TIM6_stat;
uint8_t TIM7_stat;

Motor *base_motor = &(Motor){
	.GPIO_Port = Base_DIR_GPIO_Port,
    .GPIO_Pin = Base_DIR_Pin,
    .htim = &htim7,
    .channel = Base_channel,
	.deg = 120,
	.motor_div = 32,
	.rratio = 28,
	.pwm_us = 100,
	.name = Base_motor,
	.power = OFF,
	.position = NOT_RESETED,
	.check_sensor_period = TRUE,
	.check_sensor_dir = BACKWARD
};

Motor *body_motor = &(Motor){
	.GPIO_Port = Body_DIR_GPIO_Port,
    .GPIO_Pin = Body_DIR_Pin,
    .htim = &htim6,
    .channel = Body_channel,
	.deg = 360, 
	.motor_div = 32, 
	.rratio = 1, 
	.pwm_us = 200,
	.name = Body_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = NONE,
	.check_sensor_dir = NONE
};

Motor *head_motor = &(Motor){
	.GPIO_Port = Head_DIR_GPIO_Port,
    .GPIO_Pin = Head_DIR_Pin,
    .htim = &htim6,
    .channel = Head_channel,
	.deg = 20, 
	.motor_div = 32, 
	.rratio = 1, 
	.pwm_us = 200,
	.name = Head_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = NONE,
	.check_sensor_dir = NONE
};

Motor *lift_motor = &(Motor){
	.GPIO_Port = Lift_DIR_GPIO_Port,
    .GPIO_Pin = Lift_DIR_Pin,
    .htim = &htim7,
    .channel = Lift_channel,
	.deg = 360, 
	.motor_div = 32, 
	.rratio = 1, 
	.pwm_us = 200,
	.name = Lift_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = TRUE,
	.check_sensor_dir = FORWARD
};

Motor *pushing_book_motor = &(Motor){
	.GPIO_Port = Pushing_book_DIR_GPIO_Port,
    .GPIO_Pin = Pushing_book_DIR_Pin,
    .htim = &htim7,
    .channel = Pushing_book_channel,
	.deg = 360, 
	.motor_div = 32, 
	.rratio = 1, 
	.pwm_us = 200,
	.name = Pushing_book_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = NONE,
	.check_sensor_dir = NONE
};

Motor *forward_pressing_board_motor = &(Motor){
	.GPIO_Port = Forward_pressing_board_DIR_GPIO_Port,
    .GPIO_Pin = Forward_pressing_board_DIR_Pin,
    .htim = &htim7,
    .channel = Forward_pressing_board_channel,
	.deg = 360, 
	.motor_div = 32, 
	.rratio = 1, 
	.pwm_us = 200,
	.name = Forward_pressing_board_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = NONE,
	.check_sensor_dir = NONE
};

Motor *pressing_board_motor = &(Motor){
	.GPIO_Port = Pressing_board_DIR_GPIO_Port,
    .GPIO_Pin = Pressing_board_DIR_Pin,
    .htim = &htim6,
    .channel = Pressing_board_channel,
	.deg = 360, 
	.motor_div = 32, 
	.rratio = 1, 
	.pwm_us = 200,
	.name = Pressing_board_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = TRUE,
	.check_sensor_dir = FORWARD
};

Motor *rotating_shelf_motor = &(Motor){
	.GPIO_Port = Rotating_shelf_DIR_GPIO_Port,
    .GPIO_Pin = Rotating_shelf_DIR_Pin,
    .htim = &htim6,
    .channel = Rotating_shelf_channel,
	.deg = 360, 
	.motor_div = 32, 
	.rratio = 28, 
	.pwm_us = 100,
	.name = Rotating_shelf_motor,
	.power = OFF,
	.position = RESETED,
	.check_sensor_period = NONE,
	.check_sensor_dir = NONE
};


void Start_TIM2_Motor(uint16_t deg, uint16_t motor_div, uint16_t rratio, uint32_t pwm_us, uint32_t channel){
	uint32_t pulse_num = deg*motor_div*rratio/step_deg;
	MX_TIM6_Init(pulse_num, pwm_us);
	MX_TIM2_Init(pwm_us);
	HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_PWM_Start(&htim2, channel);
}

void Stop_TIM2_Motor(void){
	HAL_TIM_Base_Stop(&htim6);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_ALL);
	TIM6_IT_count = 0;
}

void Start_TIM3_Motor(uint16_t deg, uint16_t motor_div, uint16_t rratio, uint32_t pwm_us, uint32_t channel){
	uint32_t pulse_num = deg*motor_div*rratio/step_deg;
	MX_TIM7_Init(pulse_num, pwm_us);
	MX_TIM3_Init(pwm_us);
	HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_PWM_Start(&htim3, channel);
}

void Stop_TIM3_Motor(void){
	HAL_TIM_Base_Stop(&htim7);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_ALL);
	TIM7_IT_count = 0;
}


void Forward_motor(Motor *motor, float per){
	SET_Forward_DIR(motor);
	if (motor->htim == &htim6){
		TIM6_stat = ON;
		Current_tim6_motor = motor;
		Start_TIM2_Motor((uint16_t) (motor->deg)*per, motor->motor_div, motor->rratio, motor->pwm_us, motor->channel);
	}
	else if (motor->htim == &htim7){
		TIM7_stat = ON;
		Current_tim7_motor = motor;
		Start_TIM3_Motor((uint16_t) (motor->deg)*per, motor->motor_div, motor->rratio, motor->pwm_us, motor->channel);
	}
    motor->power = ON;
	/* CHECK P ONLY IN FORWARD PROCESS */
	if (motor->check_sensor_dir == FORWARD){
		motor->check_sensor_period = TRUE;
		Sensor_current_check = motor->name;
		while (motor->position == RESETED && motor->check_sensor_period == TRUE)
		{
			if (Check_P_cpl == CPL)   // if former check process has already been done
			{
				Get_P_sensor_avg();
			}
		}
		motor->check_sensor_period = TRUE;
	}
	/* CHECK P ONLY IN FORWARD PROCESS */
    motor->position = NOT_RESETED;
}

void Backward_motor(Motor *motor, float per){
	SET_Backward_DIR(motor);
	if (motor->htim == &htim6){
		TIM6_stat = ON;
		Current_tim6_motor = motor;
		Start_TIM2_Motor((uint16_t) (motor->deg)*per, motor->motor_div, motor->rratio, motor->pwm_us, motor->channel);
	}
	else if (motor->htim == &htim7){
		TIM7_stat = ON;
		Current_tim7_motor = motor;
		Start_TIM3_Motor((uint16_t) (motor->deg)*per, motor->motor_div, motor->rratio, motor->pwm_us, motor->channel);
	}
    motor->power = ON;
	/* CHECK P ONLY IN RESET PROCESS */
	if (motor->check_sensor_dir == BACKWARD){
		motor->check_sensor_period = TRUE;
		Sensor_current_check = motor->name;
		while (motor->position == NOT_RESETED && motor->check_sensor_period == TRUE)
		{
			if (Check_P_cpl == CPL)   // if former check process has already been done
			{
				Get_P_sensor_avg();
			}
		}
		motor->check_sensor_period = TRUE;
	}
	/* CHECK P ONLY IN RESET PROCESS */
    motor->position = RESETED;
}

void Stop_motor(Motor *motor){
	if (motor->power == ON)
	{
		if (motor->htim == &htim6){
			Stop_TIM2_Motor();
			TIM6_stat = OFF;
			tim6_result_str(motor);
			UART_Send(tim6_result_data);
		}
		else if (motor->htim == &htim7){
			Stop_TIM3_Motor();
			TIM7_stat = OFF;
			tim7_result_str(motor);
			UART_Send(tim7_result_data);
		}
	    motor->power = OFF;
	    motor->check_sensor_period = FALSE;
	}
}








