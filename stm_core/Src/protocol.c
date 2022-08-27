#include "protocol.h"
#include "queue.h"
#include "pump.h"

uint8_t tim6_result_data[4];
uint8_t tim7_result_data[4];
uint8_t extra_result_data[4];
uint8_t feedback_data[4];
uint8_t answer_data[4];
uint8_t type;
uint8_t head;
uint8_t mode;
uint8_t body;

void tim6_result_str(Motor *motor)
{
	tim6_result_data[0] = RESULT;
	tim6_result_data[1] = motor->name;
	tim6_result_data[2] = motor->position;
	tim6_result_data[3] = BAK;
}

void tim7_result_str(Motor *motor)
{
	tim7_result_data[0] = RESULT;
	tim7_result_data[1] = motor->name;
	tim7_result_data[2] = motor->position;
	tim7_result_data[3] = BAK;
}

void extra_result_str(uint8_t name, uint8_t stat)
{
	extra_result_data[0] = RESULT;
	extra_result_data[1] = name;
	extra_result_data[2] = stat;
	extra_result_data[3] = BAK;
}

void motor_pos_feedback_str(Motor *motor)
{
	feedback_data[0] = FEEDBACK;
	feedback_data[1] = motor->name;
	feedback_data[2] = POSITION_CHECK;
	feedback_data[3] = motor->position;
}

void motor_pos_answer_str(Motor *motor)
{
	answer_data[0] = ANSWER;
	answer_data[1] = motor->name;
	answer_data[2] = motor->position;
	answer_data[3] = BAK;
}

float parse_per(uint8_t data)
{
	float per;
	switch (data)
	{
	case '1':
		per = 0.1;
		break;

	case '2':
		per = 0.2;
		break;

	case '3':
		per = 0.3;
		break;

	case '4':
		per = 0.4;
		break;

	case '5':
		per = 0.5;
		break;

	case '6':
		per = 0.6;
		break;
	
	case '7':
		per = 0.7;
		break;

	case '8':
		per = 0.8;
		break;

	case '9':
		per = 0.9;
		break;

	case 'x':
		per = 1;
		break;
	
	default:
		per = 1;
		break;
	}
	return per;
}


void handle_htim6_queue(void)
{
	uint8_t *recv_buf;
	recv_buf = QueueFront(&htim6_queue);
	QueuePop(&htim6_queue);
	type = recv_buf[0];
	head = recv_buf[1];
	mode = recv_buf[2];
	body = recv_buf[3];

	if (type == CHECK)
	{
		if (head == Body_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(body_motor);
				UART_Send(feedback_data);
			}
		}
		else if (head == Head_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(head_motor);
				UART_Send(feedback_data);
			}
		}
		else if (head == Pressing_board_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(pressing_board_motor);
				UART_Send(feedback_data);
			}
		} 
		else if (head == Rotating_shelf_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(rotating_shelf_motor);
				UART_Send(feedback_data);
			}
		} 

	}
	else if (type == RESET)
	{
		if (head == Body_Motor_Msg)
		{
			body_motor->position = mode;
			motor_pos_answer_str(body_motor);
			UART_Send(answer_data);
		}
		else if (head == Head_Motor_Msg)
		{
			head_motor->position = mode;
			motor_pos_answer_str(head_motor);
			UART_Send(answer_data);
		}
		else if (head == Pressing_board_Motor_Msg)
		{
			pressing_board_motor->position = mode;
			motor_pos_answer_str(pressing_board_motor);
			UART_Send(answer_data);
		}
		else if (head == Rotating_shelf_Motor_Msg)
		{
			rotating_shelf_motor->position = mode;
			motor_pos_answer_str(rotating_shelf_motor);
			UART_Send(answer_data);
		}
	}
	else if (type == COMMAND)
	{
		float per;
		if (head == Body_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(body_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(body_motor, per);
			}
		}
		else if (head == Head_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(head_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(head_motor, per);
			}
		}
		else if (head == Pressing_board_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(pressing_board_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(pressing_board_motor, per);
			}
		}
		else if (head == Rotating_shelf_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(rotating_shelf_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(rotating_shelf_motor, per);
			}
		}
	}
}


void handle_htim7_queue(void)
{
	uint8_t *recv_buf;
	recv_buf = QueueFront(&htim7_queue);
	QueuePop(&htim7_queue);
	type = recv_buf[0];
	head = recv_buf[1];
	mode = recv_buf[2];
	body = recv_buf[3];

	if (type == CHECK)
	{
		if (head == Base_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(base_motor);
				UART_Send(feedback_data);
			}
		}
		else if (head == Lift_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(lift_motor);
				UART_Send(feedback_data);
			}
		} 
		else if (head == Pushing_book_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(pushing_book_motor);
				UART_Send(feedback_data);
			}
		} 
		else if (head == Forward_pressing_board_Motor_Msg)
		{
			if (mode == POSITION_CHECK)
			{
				motor_pos_feedback_str(forward_pressing_board_motor);
				UART_Send(feedback_data);
			}
		} 
	}
    else if (type == RESET)
	{
		if (head == Base_Motor_Msg)
		{
			base_motor->position = mode;
			motor_pos_answer_str(base_motor);
			UART_Send(answer_data);
		}
		else if (head == Lift_Motor_Msg)
		{
			lift_motor->position = mode;
			motor_pos_answer_str(base_motor);
			UART_Send(answer_data);
		}
		else if (head == Pushing_book_Motor_Msg)
		{
			pushing_book_motor->position = mode;
			motor_pos_answer_str(pushing_book_motor);
			UART_Send(answer_data);
		}
		else if (head == Forward_pressing_board_Motor_Msg)
		{
			forward_pressing_board_motor->position = mode;
			motor_pos_answer_str(forward_pressing_board_motor);
			UART_Send(answer_data);
		}
	}
	else if (type == COMMAND)
	{
		float per;
		if (head == Base_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(base_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(base_motor, per);
			}
		}
		else if (head == Lift_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(lift_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(lift_motor, per);
			}
		}
		else if (head == Pushing_book_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(pushing_book_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(pushing_book_motor, per);
			}
		}
		else if (head == Forward_pressing_board_Motor_Msg)
		{
			if (mode == FORWARD_Msg)
			{
				per = parse_per(body);
				Forward_motor(forward_pressing_board_motor, per);
			}
			else if (mode == BACKWARD_Msg)
			{
				per = parse_per(body);
				Backward_motor(forward_pressing_board_motor, per);
			}
		}
	}
}

void handle_extra_queue(void)
{
	uint8_t *recv_buf;
	recv_buf = QueueFront(&extra_queue);
	QueuePop(&extra_queue);
	type = recv_buf[0];
	head = recv_buf[1];
	mode = recv_buf[2];
	body = recv_buf[3];

	if (type == COMMAND)
	{
		if (head == Pump_Msg)
		{
			if (mode == POWERON_Msg)
			{
				activate_pump();
			}
			else if (mode == POWEROFF_Msg)
			{
				deactivate_pump();
			}
		}
	}
}
