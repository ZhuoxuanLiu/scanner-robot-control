/*
 * sensor.h
 *
 *  Created on: Jun 23, 2022
 *      Author: a3352
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "adc.h"
#include "stdlib.h"
#include "usart.h"
#include "motor.h"

#define CHANNEL_4_VAL       0
#define CHANNEL_5_VAL       1
#define CHANNEL_6_VAL       2
#define CHANNEL_7_VAL       3

#define Adsorp_head_sensor_channel       			CHANNEL_4_VAL
#define Pressing_board_left_sensor_channel       	CHANNEL_5_VAL
#define Pressing_board_right_sensor_channel       	CHANNEL_6_VAL
#define Lift_sensor_channel       					CHANNEL_7_VAL

#define PRESS_MIN           10
#define PRESS_MAX           1000

#define RESET_P_Threshold   			600
#define PRESSING_BOARD_P_Threshold   	600
#define LIFT_P_Threshold   				600

#define Adsorp_head_sensor         				1
#define Pressing_board_sensor            		2
#define Lift_sensor    							3

extern uint8_t Sensor_current_check;
extern uint8_t Check_P_cpl;
extern uint32_t Adsorp_head_P;
extern uint32_t Pressing_board_left_P;
extern uint32_t Pressing_board_right_P;
extern uint32_t Lift_P;

void Check_P_sensor(void);
void Get_P_sensor_avg(void);
void UART_Send_P_Value(void);

#endif /* INC_SENSOR_H_ */