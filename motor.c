#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
void motorControl(char c, int quantity)
{
	int q = quantity;
	switch(c)
	{
		case 'F':
			PORTF=0x50;
			PORTA=0x01;
			break; 
		case 'B':	
			PORTF=0xA0;
			PORTA=0x08;
			break; 
		case 'L':
			PORTF=0x60;
			PORTA=0x04;
			break; 
		case 'R':
			PORTF=0x90;
			PORTA=0x02;
			break; 
		default:
			break;
	}	
	//_delay_ms(16000*q);	
}
int main()
{
	DDRF=0xf0;
	DDRA=0xff;
	int a=2;
	char command[4]={'F','B','L','R'};
	while(1)
	{
		//move forward for 1sec
		motorControl(command[0],1);
		/*
		//move backward for 1sec
		motorControl(command[1],1);
		//turn left for 1sec
		motorControl(command[2],1);
		//turn right for 3sec
		motorControl(command[3],3);
		*/
		_delay_ms(16000*a);
		
		//move backward for 1sec
		motorControl(command[1],1);
		_delay_ms(16000*a);
	}
}
