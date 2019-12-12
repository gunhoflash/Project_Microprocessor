#define F_CPU 16000000UL
void motorControl(char c, double quantity)
{
	int i = 0;
	int q=quantity*10;
	switch(c)
	{
		case 'F':
			PORTF=0x50;
			PORTA=0x01;
			break; 
		case 'B':	
			PORTF=0xA0;
			PORTA=0x02;
			break; 
		case 'L':
			PORTF=0x60;
			PORTA=0x04;
			break; 
		case 'R':
			PORTF=0x90;
			PORTA=0x08;
			break; 
		default:
			break;
	}	
	for(i=0; i<q; i++)
	{
		_delay_ms(1600);
	}
}
int main()
{
	DDRF=0xf0;
	DDRA=0xff;
	int a=8;
	char command[4]={'F','B','L','R'};
	while(1)
	{
		//move forward for 1sec
		motorControl(command[0],0.3);
		//move backward for 1sec
		motorControl(command[1],0.3);
		//turn left for 1sec
		motorControl(command[2],1.5);
		//turn right for 3sec
		motorControl(command[3],3);
	}
}
