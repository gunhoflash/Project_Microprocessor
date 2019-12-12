#define F_CPU 16000000UL

void motorControl(char c, double quantity)
{    
	DDRF=0xf0;
	DDRA=0xff;
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
