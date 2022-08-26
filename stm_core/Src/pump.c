/*
 * pump.c
 *
 *  Created on: Aug 21, 2022
 *      Author: a3352
 */

#include "pump.h"

void activate_pump(void)
{
	HAL_GPIO_WritePin(Vacuum_pump_GPIO_Port, Vacuum_pump_Pin, GPIO_PIN_SET);
	extra_result_str(Vacuum_pump, ON);
	UART_Send(extra_result_data);
}


void deactivate_pump(void)
{
	HAL_GPIO_WritePin(Vacuum_pump_GPIO_Port, Vacuum_pump_Pin, GPIO_PIN_RESET);
	extra_result_str(Vacuum_pump, OFF);
	UART_Send(extra_result_data);
}
