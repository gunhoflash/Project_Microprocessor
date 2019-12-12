/**************************************
 *                                    *
 *                                    *
 *     Gradient Descent Prototype     *
 *                                    *
 *                                    *
 *     by Team 9                      *
 *      - 2015920003 Kim Geonho       *
 *      - 2015920047 Jeong Woobin     *
 *                                    *
 **************************************/

#include <avr/io.h>
#include <avr/delay.h>
#include <math.h>
#include "fnd.h"
#include "gyro.h"
#include "usart.h"
#include "motorL9110.h"

#define F_CPU 16000000UL

void delay(int ms)
{
	volatile int i, j, t = F_CPU / 22000;

	ms -= 5;
	for (i = 0; i < ms; i++)
		for (j = 0; j < t; j++);
}

void run_gradient_descent()
{
	int i, j;

	// variables for gyro sensor: MPU-6050
	double a_x, a_y, a_z, g_x, g_y, g_z;
	double gradient_x, gradient_y;

	while (1)
	{
		// read data
		a_x = gyro_get_data(AX);
		a_y = gyro_get_data(AY);
		a_z = gyro_get_data(AZ);
		g_x = gyro_get_data(GX);
		g_y = gyro_get_data(GY);
		g_z = gyro_get_data(GZ);

		// print values for test
		usart_transmit_init4(a_x);
		usart_transmit('\t');
		usart_transmit_init4(a_y);
		usart_transmit('\t');
		usart_transmit_init4(a_z);
		usart_transmit('\t');
		usart_transmit_init4(g_x);
		usart_transmit('\t');
		usart_transmit_init4(g_y);
		usart_transmit('\t');
		usart_transmit_init4(g_z);
		usart_transmit('\n');
		usart_transmit('\r');

		/*
		 0 PIN 1
		LED
		 2     3

		0: +++ >>> backward right
		1: +-+ >>> backward left
		2: -++ >>> forward  right
		3: --+ >>> forward  left
		
		*/
		i = 0;
		if (a_x < 0) i = 2;
		if (a_y < 0) i++;
		set_fnd1(i, 8);

		gradient_x = a_x / 16384.0;
		gradient_y = a_y / 16384.0;

		// 16384
		j = sqrt((pow(gradient_x, 2) + pow(gradient_y, 2)) / 2) * 8;
		PORTA = 0xff ^ (0xff << j);

		usart_transmit(i);
		usart_transmit('\n');
		usart_transmit('\r');

		delay(500);
	}
}

int main()
{
	DDRA = 0xff; // 1111 1111 : set dda7 ~ dda0 to write LED

	// test motors
	motor_control(DRIVE_FORWARD,  1.5);
	motor_control(DRIVE_BACKWARD, 1.5);
	motor_control(DRIVE_LEFT,     1.5);
	motor_control(DRIVE_RIGHT,    1.5);
	motor_control(DRIVE_STOP,     0.0);

	usart_init();
	delay(5);
	gyro_init();
	delay(5);
	run_gradient_descent();
}