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
}


void deactivate_pump(void)
{
	HAL_GPIO_WritePin(Vacuum_pump_GPIO_Port, Vacuum_pump_Pin, GPIO_PIN_RESET);
}
