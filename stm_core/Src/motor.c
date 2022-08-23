/*
 * motor.c
 *
 *  Created on: Jun 16, 2022
 *      Author: a3352
 */
#include "motor.h"

float step_deg = 1.8;

uint8_t Base_Power = OFF; //初始状态为关机
uint8_t Base_Position = ROTATED; //初始状态为未复位
uint8_t Base_Check_Period = TRUE;

uint8_t Body_Power = OFF; //初始状态为关机
uint8_t Body_Position = RESETED; //初始状态为未复位
uint8_t Body_Check_Period = TRUE;

uint8_t Head_Power = OFF; //初始状态为关机
uint8_t Head_Position = RESETED; //初始状态为未复位

uint8_t Lift_Power = OFF; //初始状态为关机
uint8_t Lift_Position = LOW; //初始状态为未升高
uint8_t Lift_Check_Period = TRUE;

uint8_t Pushing_book_Power = OFF; //初始状态为关机
uint8_t Pushing_book_Position = BACK; //初始状态为复位

uint8_t Forward_pressing_board_Power = OFF; //初始状态为关机
uint8_t Forward_pressing_board_Position = BACK; //初始状态为未前进

uint8_t Pressing_board_Power = OFF; //初始状态为关机
uint8_t Pressing_board_Position = HIGH; //初始状态为未下降
uint8_t Pressing_board_Check_Period = TRUE;

uint8_t Rotating_shelf_Power = OFF; //初始状态为关机
uint8_t Rotating_shelf_Position = RESETED; //初始状态为未复位


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

/* Base Motor control function */

void Flip_Base(void){
	Base_DIR(FORWARD);
	Start_TIM3_Motor(120, 32, 28, 100, Base_channel);
    Base_Power = ON;
    Base_Position = ROTATED;
}

void Reset_Base(void){
	Base_DIR(BACKWARD);
	Start_TIM3_Motor(120, 32, 28, 100, Base_channel);
    Base_Power = ON;
	/* CHECK P ONLY IN RESET PROCESS */
	Base_Check_Period = TRUE;
	Sensor_current_check = Adsorp_head_sensor;
	while (Base_Position == ROTATED && Base_Check_Period == TRUE)
	{
		if (Check_P_cpl == CPL)   // if former check process has already been done
		{
			Get_P_sensor_avg();
		}
	}
	Base_Check_Period = TRUE;
	/* CHECK P ONLY IN RESET PROCESS */
	Base_Position = RESETED;
}

void Stop_Base(void){
	if (Base_Power == ON)
	{
		Stop_TIM3_Motor();
	    Base_Power = OFF;
	    Base_Check_Period = FALSE;
		UART_Send("Base Motor stop");
	}
}

/* Base Motor control function */


/* Body Motor control function */

void Stretch_Body(void){
	Body_DIR(FORWARD);
	Start_TIM2_Motor(360, 32, 1, 200, Body_channel);
	Body_Power = ON;
	Body_Position = STRETCHED;
}

void Lower_Body(void){
	Body_DIR(BACKWARD);
	Start_TIM2_Motor(360, 32, 1, 200, Body_channel);
	Body_Power = ON;
	Body_Position = RESETED;
}

void Stop_Body(void){
	if (Body_Power == ON)
	{
		Stop_TIM2_Motor();
		Body_Power = OFF;
		UART_Send("Body Motor stop");
	}
}

/* Body Motor control function */


/* Head Motor control function */

void Rotate_Head(void){
	Head_DIR(FORWARD);
	Start_TIM2_Motor(10, 32, 1, 1000, Head_channel);
	Head_Power = ON;
	Head_Position = ROTATED;
}

void Reset_Head(void){
	Head_DIR(BACKWARD);
	Start_TIM2_Motor(10, 32, 1, 1000, Head_channel);
	Head_Power = ON;
	Head_Position = RESETED;
}

void Stop_Head(void){
	if (Head_Power == ON)
	{
		Stop_TIM2_Motor();
		Head_Power = OFF;
		UART_Send("Head Motor stop");
	}
}

/* Head Motor control function */


/* Lift Motor control function */

void Lift_book(void){
	Lift_DIR(FORWARD);
	Start_TIM3_Motor(720, 32, 1, 100, Lift_channel);
	Lift_Power = ON;

	/* CHECK P ONLY IN LIFT PROCESS */
	Lift_Check_Period = TRUE;
	Sensor_current_check = Lift_sensor;
	while (Lift_Position == LOW && Lift_Check_Period == TRUE)
	{
		if (Check_P_cpl == CPL)
		{
			Get_P_sensor_avg();
		}
	}
	Lift_Check_Period = TRUE;
	/* CHECK P ONLY IN LIFT PROCESS */
	Lift_Position = HIGH;
}

void Lower_lifter(void){
	Lift_DIR(BACKWARD);
	Start_TIM3_Motor(720, 32, 1, 100, Lift_channel);
	Lift_Power = ON;
	Lift_Position = LOW;
}

void Stop_Lift(void){
	if (Lift_Power == ON)
	{
		Stop_TIM3_Motor();
		Lift_Power = OFF;
		UART_Send("Lift Motor stop");
	}
}

/* Lift Motor control function */


/* Pushing book Motor control function */

void Forward_Pushing_book(void){
	Pushing_book_DIR(FORWARD);
	Start_TIM3_Motor(720, 32, 1, 100, Pushing_book_channel);
	Pushing_book_Power = ON;
	Pushing_book_Position = FRONT;
}

void Backward_Pushing_book(void){
	Pushing_book_DIR(BACKWARD);
	Start_TIM3_Motor(720, 32, 1, 100, Pushing_book_channel);
	Pushing_book_Power = ON;
	Pushing_book_Position = BACK;
}

void Stop_Pushing_book(void){
	if (Pushing_book_Power == ON)
	{
		Stop_TIM3_Motor();
		Pushing_book_Power = OFF;
		UART_Send("Pushing book Motor stop");
	}
}

/* Pushing book Motor control function */


/* Pressing board Motor control function */

void Elevate_Pressing_board(void){
	Pressing_board_DIR(FORWARD);
	Start_TIM2_Motor(720, 32, 1, 100, Pressing_board_channel);
	Pressing_board_Power = ON;
	Pressing_board_Position = HIGH;
}

void Lower_Pressing_board(void){
	Pressing_board_DIR(BACKWARD);
	Start_TIM2_Motor(720, 32, 1, 100, Pressing_board_channel);
	Pressing_board_Power = ON;

	/* CHECK P ONLY IN Pressing board PROCESS */
	Pressing_board_Check_Period = TRUE;
	Sensor_current_check = Pressing_board_sensor;
	while (Pressing_board_Position == HIGH && Pressing_board_Check_Period == TRUE)
	{
		if (Check_P_cpl == CPL)
		{
			Get_P_sensor_avg();
		}
	}
	Pressing_board_Check_Period = TRUE;
	/* CHECK P ONLY IN Pressing board PROCESS */
	Pressing_board_Position = LOW;
}

void Stop_Pressing_board(void){
	if (Pressing_board_Power == ON)
	{
		Stop_TIM2_Motor();
		Pressing_board_Power = OFF;
		UART_Send("Pressing board Motor stop");
	}
}

/* Pressing board Motor control function */


/* Forward Pressing board Motor control function */

void Forward_Pressing_board(void){
	Forward_pressing_board_DIR(FORWARD);
	Start_TIM3_Motor(720, 32, 1, 100, Forward_pressing_board_channel);
	Forward_pressing_board_Power = ON;
	Forward_pressing_board_Position = FRONT;
}

void Backward_Pressing_board(void){
	Forward_pressing_board_DIR(BACKWARD);
	Start_TIM3_Motor(720, 32, 1, 100, Forward_pressing_board_channel);
	Forward_pressing_board_Power = ON;
	Forward_pressing_board_Position = BACK;
}

void Stop_Forward_Pressing_board(void){
	if (Forward_pressing_board_Power == ON)
	{
		Stop_TIM3_Motor();
		Forward_pressing_board_Power = OFF;
		UART_Send("Forward Pressing board Motor stop");
	}
}

/* Forward Pressing board Motor control function */


/* Rotating shelf Motor control function */

void Rotate_shelf(void){
	Rotating_shelf_DIR(FORWARD);
	Start_TIM2_Motor(720, 32, 1, 100, Rotating_shelf_channel);
	Rotating_shelf_Power = ON;
	Rotating_shelf_Position = ROTATED;
}

void Reset_shelf(void){
	Rotating_shelf_DIR(BACKWARD);
	Start_TIM2_Motor(720, 32, 1, 100, Rotating_shelf_channel);
	Rotating_shelf_Power = ON;
	Rotating_shelf_Position = RESETED;
}

void Stop_Rotating_shelf(void){
	if (Rotating_shelf_Power == ON)
	{
		Stop_TIM2_Motor();
		Rotating_shelf_Power = OFF;
		UART_Send("Rotating shelf Motor stop");
	}
}

/* Rotating shelf Motor control function */






