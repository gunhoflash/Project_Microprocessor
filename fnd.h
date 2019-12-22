/*

	This header file defines methods to control FND.

	Required PINs             : C, G
	Required external headers : delay.h

*/

/*
	Description:
		Turn on only one FND.
	Parameters:
		select: Index of each FND. (0 ~ 3)
		data: A digit number to display. (0 ~ 9)
	Return:
		nothing
*/
void set_fnd1(int select, int data)
{
	/*
	*  HHHH
	* C    G
	* C    G
	*  BBBB
	* D    F
	* D    F
	*  EEEE  A
	*/

	DDRC  = 0xff; // 1111 1111 : set ddc7 ~ ddc0 to write FND DATA
	DDRG |= 0x0f; // 0000 1111 : set ddg3 ~ ddg0 to write FND SELECT

	// select the FND to turn on
	PORTG = 0x8 >> (select % 4);

	// display the digit
	switch (data)
	{
		case 0 : PORTC = 0b00111111; break;
		case 1 : PORTC = 0b00000110; break;
		case 2 : PORTC = 0b01011011; break;
		case 3 : PORTC = 0b01001111; break;
		case 4 : PORTC = 0b01100110; break;
		case 5 : PORTC = 0b01101101; break;
		case 6 : PORTC = 0b01111101; break;
		case 7 : PORTC = 0b00100111; break;
		case 8 : PORTC = 0b01111111; break;
		case 9 : PORTC = 0b01101111; break;
		default: PORTC = 0b10000000; break;
	}
}

/*
	Description:
		Turn on all FNDs.
	Parameters:
		data: An integer to display. (0 ~ 9999)
		select: Index of each FND. (0 ~ 3)
	Return:
		nothing
*/
void set_fnd4(int data, unsigned int ms)
{
	unsigned int time = 5;
	unsigned int max_time = ms * F_CPU / 1777000;

	// calculate digits to display
	int data0 = (data / 1000) % 10;
	int data1 = (data /  100) % 10;
	int data2 = (data /   10) % 10;
	int data3 = (data /    1) % 10;

	// display digits for a time
	while (time < max_time)
	{
		set_fnd1(0, data0); _delay_ms(2);
		set_fnd1(1, data1); _delay_ms(2);
		set_fnd1(2, data2); _delay_ms(2);
		set_fnd1(3, data3); _delay_ms(2);
		time++;
	}
}