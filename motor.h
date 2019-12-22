/*
	L9110
*/
/*

	This header file defines methods to control motor driver: L9110.
	To turn 180 degree, rotate for 4.5 seconds.

	Required PINs             : F
	Required external headers : nothing

*/

// define code-number of command
#define DRIVE_STOP     0
#define DRIVE_FORWARD  1
#define DRIVE_BACKWARD 2
#define DRIVE_LEFT     3
#define DRIVE_RIGHT    4

/*
	Description:
		Drive the motor.
	Parameters:
		addr: Address to access.
	Return:
		data.
*/
void motor_control(int code_number, double time)
{
	int i = 0;

	DDRF = 0xf0;

	switch (code_number)
	{
		case DRIVE_STOP     : PORTF = 0x00; break;
		case DRIVE_FORWARD  : PORTF = 0xA0; break;
		case DRIVE_BACKWARD : PORTF = 0x50; break;
		case DRIVE_LEFT     : PORTF = 0x60; break;
		case DRIVE_RIGHT    : PORTF = 0x90; break;
		default             : PORTF = 0x00; break;
	}

	// delay the system for a while
	for (i = 0; i < time * 10; i++)
	{
		// delay 0.1s
		_delay_ms(1600);
	}
}