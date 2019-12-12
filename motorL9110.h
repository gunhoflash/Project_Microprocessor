// need to set: #define F_CPU 16000000UL
#define DRIVE_STOP     0
#define DRIVE_FORWARD  1
#define DRIVE_BACKWARD 2
#define DRIVE_LEFT     3
#define DRIVE_RIGHT    4

void motor_control(int type, double quantity)
{
	DDRF = 0xf0;
	int i = 0;
	int q = quantity * 10;
	switch (type)
	{
		case DRIVE_STOP     : PORTF = 0x00; break;
		case DRIVE_FORWARD  : PORTF = 0x50; break;
		case DRIVE_BACKWARD : PORTF = 0xA0; break;
		case DRIVE_LEFT     : PORTF = 0x60; break;
		case DRIVE_RIGHT    : PORTF = 0x90; break;
		default             : PORTF = 0x00; break;
	}

	for (i = 0; i < q; i++)
	{
		// delay 0.1s
		_delay_ms(1600);
	}
}