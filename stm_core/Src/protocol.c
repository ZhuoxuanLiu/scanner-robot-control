#include "protocol.h"


void handle_protocol(void)
{
	if (type == CHECK)
	{
		if (head == Base_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Base_Position == ROTATED)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Body_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Body_Position == STRETCHED)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Head_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Head_Position == ROTATED)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Lift_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Lift_Position == HIGH)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Pushing_book_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Pushing_book_Position == FRONT)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Forward_pressing_board_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Forward_pressing_board_Position == FRONT)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Pressing_board_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Pressing_board_Position == LOW)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}
		else if (head == Rotating_shelf_Motor_Msg)
		{
			if (body == POSITION_CHECK)
			{
				if (Rotating_shelf_Position == ROTATED)
				{
					UART_Send(NOT_RESETED_str);
				}
				else
				{
					UART_Send(RESETED_str);
				}
			}
		}

	}
	else if (type == FEEDBACK)
	{
		if (head == Base_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Base_Position = ROTATED;
			}
			else if (body == RESETED_Msg)
			{
				Base_Position = RESETED;
			}
		}
		else if (head == Body_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Body_Position = STRETCHED;
			}
			else if (body == RESETED_Msg)
			{
				Body_Position = RESETED;
			}
		}
		else if (head == Head_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Head_Position = ROTATED;
			}
			else if (body == RESETED_Msg)
			{
				Head_Position = RESETED;
			}
		}
		else if (head == Lift_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Lift_Position = HIGH;
			}
			else if (body == RESETED_Msg)
			{
				Lift_Position = LOW;
			}
		}
		else if (head == Pushing_book_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Pushing_book_Position = FRONT;
			}
			else if (body == RESETED_Msg)
			{
				Pushing_book_Position = BACK;
			}
		}
		else if (head == Forward_pressing_board_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Forward_pressing_board_Position = FRONT;
			}
			else if (body == RESETED_Msg)
			{
				Forward_pressing_board_Position = BACK;
			}
		}
		else if (head == Pressing_board_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Pressing_board_Position = LOW;
			}
			else if (body == RESETED_Msg)
			{
				Pressing_board_Position = HIGH;
			}
		}
		else if (head == Rotating_shelf_Motor_Msg)
		{
			if (body == NOT_RESETED_Msg)
			{
				Rotating_shelf_Position = ROTATED;
			}
			else if (body == RESETED_Msg)
			{
				Rotating_shelf_Position = RESETED;
			}
		}
	}
	else if (type == COMMAND)
	{
		if (head == Base_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Flip_Base();
			}
			else if (body == BACKWARD_Msg)
			{
				Reset_Base();
			}
		}
		else if (head == Body_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Stretch_Body();
			}
			else if (body == BACKWARD_Msg)
			{
				Lower_Body();
			}
		}
		else if (head == Head_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Rotate_Head();
			}
			else if (body == BACKWARD_Msg)
			{
				Reset_Head();
			}
		}
		else if (head == Lift_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Lift_book();
			}
			else if (body == BACKWARD_Msg)
			{
				Lower_lifter();
			}
		}
		else if (head == Pushing_book_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Forward_Pushing_book();
			}
			else if (body == BACKWARD_Msg)
			{
				Backward_Pushing_book();
			}
		}
		else if (head == Forward_pressing_board_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Forward_Pressing_board();
			}
			else if (body == BACKWARD_Msg)
			{
				Backward_Pressing_board();
			}
		}
		else if (head == Pressing_board_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Lower_Pressing_board();
			}
			else if (body == BACKWARD_Msg)
			{
				Elevate_Pressing_board();
			}
		}
		else if (head == Rotating_shelf_Motor_Msg)
		{
			if (body == FORWARD_Msg)
			{
				Rotate_shelf();
			}
			else if (body == BACKWARD_Msg)
			{
				Reset_shelf();
			}
		}
		else if (head == Pump_Msg)
		{
			if (body == POWERON_Msg)
			{
				activate_pump();
			}
			else if (body == POWEROFF_Msg)
			{
				deactivate_pump();
			}
		}
	}
	else
	{
		UART_Send("error message");
	}
}



