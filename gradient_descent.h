/*

	This header file defines methods to fulfill gradient descent method.

	Required PINs             : A
	Required external headers : buzzer.h, delay.h, fnd.h, gyro.h, motor.h, usart.h

*/

// calculated final gradients and inclination
volatile double momentum_x  = 0;
volatile double momentum_y  = 0;
volatile double inclination = 0;

/*
	Description:
		Calculate momentum_x, momentum_y.
	Parameters:
		normalized_gradient_x: Measured gradient.
		normalized_gradient_y: Measured gradient.
		angle: Angle with the steepest direction of the slope.
	Return:
		nothing
*/
void gradient_descent_calculate_xy(double normalized_gradient_x, double normalized_gradient_y)
{
	// set momentum according to setting
	if (switch_selected_value_optimizer == SWITCH_VALUE_OPTIMIZER_GD) inclination  = 0;
	if (switch_selected_value_optimizer == SWITCH_VALUE_OPTIMIZER_MO) inclination *= 0.9;

	momentum_x = normalized_gradient_x + inclination;
	momentum_y = normalized_gradient_y;
}

/*
	Description:
		Run the robot to fulfill the gradient descent method.
	Parameters:
		nothing
	Return:
		nothing
*/
void gradient_descent_run()
{
	// variables for gyro sensor
	double a_x, a_y, a_z, g_x, g_y, g_z;
	double normalized_gradient_x; // 0 ~ 1
	double normalized_gradient_y; // 0 ~ 1
	double angle; // -pi ~ pi
	double inclination; // 0 ~ sqrt(2)

	buzzer(BUZZER_3);
	while (1)
	{
		PORTA = 0x01;

		// stop motor
		motor_control(DRIVE_STOP, 0);

		// wait until the user order the robot to run
		while (switch_selected_value_run != SWITCH_VALUE_RUN_ON);

		PORTA = 0x02;

		// read data from the gyro sensor
		a_x = gyro_get_data(GYRO_AX);
		a_y = gyro_get_data(GYRO_AY);
		a_z = gyro_get_data(GYRO_AZ);
		g_x = gyro_get_data(GYRO_GX);
		g_y = gyro_get_data(GYRO_GY);
		g_z = gyro_get_data(GYRO_GZ);

		PORTA = 0x04;

		// print values for test
		if (is_debug) {
			usart_transmit_init4(a_x); usart_transmit('\t');
			usart_transmit_init4(a_y); usart_transmit('\t');
			usart_transmit_init4(a_z); usart_transmit('\t');
			usart_transmit_init4(g_x); usart_transmit('\t');
			usart_transmit_init4(g_y); usart_transmit('\t');
			usart_transmit_init4(g_z);
			usart_transmit('\n');
			usart_transmit('\r');
		}

		/*

		 3     2            |-x        
		                    |          
		 1 PIN 0            |          
		                    |          
		                    |          
		                    |          
		- - - - - - - - - - o - - - - -
		-y                  |        +y
		                    |+x        

		*/

		// normalize gradient x and y
		normalized_gradient_x = - a_x / 16384.0;
		normalized_gradient_y =   a_y / 16384.0;

		// calculate momentum x and y
		gradient_descent_calculate_xy(normalized_gradient_x, normalized_gradient_y);

		// get angle from x-axis by using atan
		angle = atan2(momentum_y, momentum_x);

		// get inclination by pythagoras-length of gradients
		inclination = sqrt((pow(momentum_x, 2) + pow(momentum_y, 2)));

		PORTA = 0x08;

		if (inclination > 0.05)
		{
			// turn left/right
			if (angle < 0)
				motor_control(DRIVE_LEFT, - angle / 3.141592653589793 * 4.5);
			if (angle > 0)
				motor_control(DRIVE_RIGHT,  angle / 3.141592653589793 * 4.5);
			// move forward
			motor_control(DRIVE_FORWARD, 0.5 + (switch_selected_value_learning_rate * 4 + 2) * inclination);
			motor_control(DRIVE_STOP, 0);
		}
		else
		{
			// just sound the buzzer when inclination is too small
			buzzer(BUZZER_2);
			delay_ms(1000);
		}

		PORTA = 0x10;

		// show the number on FND for test
		if (a_x >= 0 && a_y >= 0) set_fnd1(1, 2);
		if (a_x >= 0 && a_y <  0) set_fnd1(1, 3);
		if (a_x <  0 && a_y >= 0) set_fnd1(1, 0);
		if (a_x <  0 && a_y <  0) set_fnd1(1, 1);

		PORTA = 0x20;

		// print inclination for test
		if (is_debug) {
			usart_transmit_init4(1000 * (switch_selected_value_learning_rate * 1 + 0.5) * inclination);
			usart_transmit('\n');
			usart_transmit('\r');
		}

		PORTA = 0x40;

		delay_ms(10000);
	}
}

