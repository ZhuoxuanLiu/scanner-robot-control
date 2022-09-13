/*
 * sensor.c
 *
 *  Created on: Jun 23, 2022
 *      Author: a3352
 */
#include "sensor.h"
#include "motor.h"
#include "adc.h"
#include <stdlib.h>
#include "usart.h"


uint32_t ADC_timeout = 0;
uint32_t Adsorp_head_P = 0;
uint32_t Pressing_board_left_P = 0;
uint32_t Pressing_board_right_P = 0;
uint32_t Lift_P = 0;
uint32_t avg[4];
uint8_t Sensor_current_check;

uint32_t P_SENSOR[4];
uint16_t P_SENSOR_NUM = 4;
uint16_t ADC_Sample_num = 15;

uint32_t sensor_map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return abs((x - in_min) * (out_max - out_min) / (in_max - in_min));
}

void Get_P_avg(void)
{
	for (int i=0; i<4; i++)
	{
		avg[i] = 0;
	}
	for (int i=0; i<ADC_Sample_num; i++)
	{
		for (int j=0; j<P_SENSOR_NUM; j++)
		{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 3);
			P_SENSOR[j] += HAL_ADC_GetValue(&hadc1);
		}
	}
    for (int i=0; i<P_SENSOR_NUM; i++)
    {
    	P_SENSOR[i] = P_SENSOR[i]/ADC_Sample_num;
    }
    HAL_ADC_Stop(&hadc1);
    Check_P_sensor();
}



void Check_P_sensor(void){
	for (int i=0; i<P_SENSOR_NUM; i++)
	{
		avg[i] = P_SENSOR[i];
		P_SENSOR[i] = 0;
	}


	/* CHECK adsorption head sensor */
	if (Sensor_current_check == Adsorp_head_sensor)
	{
		Adsorp_head_P = sensor_map(avg[Adsorp_head_sensor_channel], 100, 4200, ADSORP_PRESS_MIN, ADSORP_PRESS_MAX);
		if(Adsorp_head_P < ADSORP_PRESS_MIN)
		{
			Adsorp_head_P = 0;
		}

		if(Adsorp_head_P > ADSORP_PRESS_MAX)
		{
			Adsorp_head_P = ADSORP_PRESS_MAX;
		}

		if (Adsorp_head_P > RESET_P_Threshold)
		{
			base_motor->position = RESETED;
			Stop_motor(base_motor);
		}
	}
	/* CHECK adsorption head sensor */


	/* CHECK Lift sensor */
	else if (Sensor_current_check == Lift_sensor)
	{
		Lift_P = sensor_map(avg[Lift_sensor_channel], 100, 4200, PUSHING_BOOK_PRESS_MIN, PUSHING_BOOK_PRESS_MAX);
		if(Lift_P < PUSHING_BOOK_PRESS_MIN)
		{
			Lift_P = 0;
		}

		if(Lift_P > PUSHING_BOOK_PRESS_MAX)
		{
			Lift_P = PUSHING_BOOK_PRESS_MAX;
		}

		if (Lift_P > LIFT_P_Threshold)
		{
			lift_motor->position = NOT_RESETED;
			Stop_motor(lift_motor);
		}
	}
	/* CHECK Lift sensor */

	/* CHECK Pressing board sensor */
	else if (Sensor_current_check == Pressing_board_sensor)
	{
		Pressing_board_left_P = sensor_map(avg[Pressing_board_left_sensor_channel], 1880, 2350, PRESSING_BOARD_PRESS_MIN, PRESSING_BOARD_PRESS_MAX);
		Pressing_board_right_P = sensor_map(avg[Pressing_board_right_sensor_channel], 1880, 2350, PRESSING_BOARD_PRESS_MIN, PRESSING_BOARD_PRESS_MAX);
		if(Pressing_board_left_P < PRESSING_BOARD_PRESS_MIN)
		{
			Pressing_board_left_P = 0;
		}

		if(Pressing_board_left_P > PRESSING_BOARD_PRESS_MAX)
		{
			Pressing_board_left_P = PRESSING_BOARD_PRESS_MAX;
		}

		if(Pressing_board_right_P < PRESSING_BOARD_PRESS_MIN)
		{
			Pressing_board_right_P = 0;
		}

		if(Pressing_board_right_P > PRESSING_BOARD_PRESS_MAX)
		{
			Pressing_board_right_P = PRESSING_BOARD_PRESS_MAX;
		}

		if (Pressing_board_left_P > PRESSING_BOARD_P_Threshold && Pressing_board_right_P > PRESSING_BOARD_P_Threshold)
		{
			pressing_board_motor->position = NOT_RESETED;
			Stop_motor(pressing_board_motor);
		}
	}
	/* CHECK Pressing board sensor */

	else
	{

	}
 }

void UART_Send_P_Value(void)
{
	Get_P_avg(); // UART send in callback may cause dead lock
	char str[25];
	if (Sensor_current_check == Adsorp_head_sensor)
	{
		itoa(Adsorp_head_P, str, 10);
	}
	else if (Sensor_current_check == Pressing_board_sensor)
	{
		uint16_t send_P;
		if (Pressing_board_left_P <= Pressing_board_right_P)
		{
			send_P = Pressing_board_left_P;
		}
		else
		{
			send_P = Pressing_board_right_P;
		}
		itoa(send_P, str, 10);
	}
	else if (Sensor_current_check == Lift_sensor)
	{
		itoa(Lift_P, str, 10);
	}
	else
	{
		itoa(0, str, 10);
	}
	UART_Send((uint8_t *)str);
}

void UART_Test_P_Value(void)
{
	Get_P_avg(); // UART send in callback may cause dead lock
	char str[25];
	itoa(Lift_P, str, 10);
	//itoa(Lift_P, str, 10);
	UART_Send((uint8_t *)str);
}




