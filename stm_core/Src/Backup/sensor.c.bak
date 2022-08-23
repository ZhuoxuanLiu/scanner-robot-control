/*
 * sensor.c
 *
 *  Created on: Jun 23, 2022
 *      Author: a3352
 */
#include "sensor.h"

uint8_t Check_P_cpl = CPL;
uint32_t ADC_timeout = 0;
uint32_t Adsorp_head_P = 0;
uint32_t Pressing_board_left_P = 0;
uint32_t Pressing_board_right_P = 0;
uint32_t Lift_P = 0;
uint32_t avg[4];
uint8_t Sensor_current_check;

uint32_t sensor_map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return abs((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}


void Get_P_sensor_avg(void)
{
    Check_P_cpl = NCPL;
	for (int i=0; i<4; i++)
	{
		avg[i] = 0;
	}
	HAL_ADC_Start_IT(&hadc1);
}

void Check_P_sensor(void){
	for (int i=0; i<4; i++)
	{
		avg[i] = P_SENSOR[i];
		P_SENSOR[i] = 0;
	}

	/* CHECK adsorption head sensor */
	if (Sensor_current_check == Adsorp_head_sensor)
	{
		Adsorp_head_P = sensor_map(avg[Adsorp_head_sensor_channel], 1880, 2350, PRESS_MIN, PRESS_MAX);
			if(Adsorp_head_P < PRESS_MIN)
			{
				Adsorp_head_P = 0;
			}

			if(Adsorp_head_P > PRESS_MAX)
			{
				Adsorp_head_P = PRESS_MAX;
			}

		    if (Adsorp_head_P > RESET_P_Threshold)
		    {
		    	Base_Position = RESETED;
				Stop_Base();
		    }
		    Check_P_cpl = CPL;
	}
	/* CHECK adsorption head sensor */


	/* CHECK Pressing board sensor */
	else if (Sensor_current_check == Pressing_board_sensor)
	{
		Pressing_board_left_P = sensor_map(avg[Pressing_board_left_sensor_channel], 1880, 2350, PRESS_MIN, PRESS_MAX);
		Pressing_board_right_P = sensor_map(avg[Pressing_board_right_sensor_channel], 1880, 2350, PRESS_MIN, PRESS_MAX);
			if(Pressing_board_left_P < PRESS_MIN)
			{
				Pressing_board_left_P = 0;
			}

			if(Pressing_board_left_P > PRESS_MAX)
			{
				Pressing_board_left_P = PRESS_MAX;
			}

			if(Pressing_board_right_P < PRESS_MIN)
			{
				Pressing_board_right_P = 0;
			}

			if(Pressing_board_right_P > PRESS_MAX)
			{
				Pressing_board_right_P = PRESS_MAX;
			}

		    if (Pressing_board_left_P > PRESSING_BOARD_P_Threshold && Pressing_board_right_P > PRESSING_BOARD_P_Threshold)
		    {
		    	Pressing_board_Position = LOW;
		    	Stop_Pressing_board();
		    }
		    Check_P_cpl = CPL;
	}
	/* CHECK Pressing board sensor */


	/* CHECK Lift sensor */
	else if (Sensor_current_check == Lift_sensor)
	{
		Lift_P = sensor_map(avg[Lift_sensor_channel], 1880, 2350, PRESS_MIN, PRESS_MAX);
			if(Lift_P < PRESS_MIN)
			{
				Lift_P = 0;
			}

			if(Lift_P > PRESS_MAX)
			{
				Lift_P = PRESS_MAX;
			}

		    if (Lift_P > LIFT_P_Threshold)
		    {
		    	Lift_Position = HIGH;
		    	Stop_Lift();
		    }
		    Check_P_cpl = CPL;
	}
	/* CHECK Lift sensor */

	else
	{
	    Check_P_cpl = CPL;
	}
 }

void UART_Send_P_Value(void)
{
	Get_P_sensor_avg(); // UART send in callback may cause dead lock
	while (Check_P_cpl == NCPL)
	{
		ADC_timeout++;
		if(ADC_timeout>HAL_MAX_DELAY)
		{
			ADC_timeout=0;
			break;
		}
	}
	if (Check_P_cpl == CPL)
	{
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
		UART_Send(str);
	}
}





