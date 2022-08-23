/*
 * protocol.h
 *
 *  Created on: 2022年8月20日
 *      Author: a3352
 */

#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_

#include "main.h"
#include "motor.h"
#include "pump.h"

#define CHECK			'0'
#define FEEDBACK		'1'
#define COMMAND			'2'

#define Base_Motor_Msg						'0'
#define Body_Motor_Msg 						'1'
#define Head_Motor_Msg						'2'
#define Lift_Motor_Msg						'3'
#define Pushing_book_Motor_Msg				'4'
#define Forward_pressing_board_Motor_Msg	'5'
#define Pressing_board_Motor_Msg			'6'
#define Rotating_shelf_Motor_Msg			'7'
#define Pump_Msg							'8'

#define POSITION_CHECK			'0'
#define POWER_CHECK				'1'

#define NOT_RESETED_Msg			'0'
#define RESETED_Msg				'1'

#define FORWARD_Msg			'1'
#define BACKWARD_Msg		'0'

#define POWERON_Msg			'1'
#define POWEROFF_Msg		'0'

#define POWER_OFF_str		"0"
#define POWER_ON_str		"1"

#define NOT_RESETED_str		"0"
#define RESETED_str			"1"

void handle_protocol(void);

#endif /* INC_PROTOCOL_H_ */
