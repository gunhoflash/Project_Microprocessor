/**************************************
 *                                    *
 *                                    *
 *     Gradient Descent Prototype     *
 *                                    *
 *                                    *
 *     by Team 09                     *
 *      - 2015920003 Kim Geonho       *
 *      - 2015920047 Jeong Woobin     *
 *                                    *
 **************************************/

/*
	import header files and define some constants
*/
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <math.h>

#define F_CPU 16000000UL
#define is_debug 1

#include "delay.h"
#include "fnd.h"
#include "buzzer.h"
#include "switch.h"
#include "gyro.h"
#include "motor.h"
#include "usart.h"
#include "gradient_descent.h"

int main()
{
	// enable LED
	DDRA = 0xff;

	// initialize switch
	switch_init();
	
	// initialize USART communication
	if (is_debug)
		usart_init();

	// initialize gyro sensor
	gyro_init();

	// run the robot
	gradient_descent_run();
}
