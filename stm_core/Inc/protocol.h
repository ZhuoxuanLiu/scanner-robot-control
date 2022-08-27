/*
 * protocol.h
 *
 *  Created on: 2022年8月20日
 *      Author: a3352
 */

#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_
#include "motor.h"
// recv
#define CHECK			'0'
#define RESET			'1'
#define COMMAND			'2'
// send
#define FEEDBACK		'0'
#define ANSWER		    '1'
#define RESULT		    '2'

#define BAK				'0'

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

#define FORWARD_Msg			'1'
#define BACKWARD_Msg		'0'

#define POWERON_Msg			'1'
#define POWEROFF_Msg		'0'


void tim6_result_str(Motor *motor);
void tim7_result_str(Motor *motor);
void extra_result_str(uint8_t name, uint8_t stat);
void handle_htim6_queue(void);
void handle_htim7_queue(void);
void handle_extra_queue(void);

extern uint8_t tim6_result_data[4];
extern uint8_t tim7_result_data[4];
extern uint8_t extra_result_data[4];
extern uint8_t feedback_data[4];
extern uint8_t answer_data[4];
extern uint8_t type;
extern uint8_t head;
extern uint8_t mode;
extern uint8_t body;

#endif /* INC_PROTOCOL_H_ */
