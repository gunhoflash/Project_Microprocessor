/*

	This header file defines methods to delay the system for a while.

	Required PINs             : nothing
	Required external headers : nothing

*/

/*
	Description:
		Delay the system.
	Parameters:
		us: Set the delay in microseconds.
	Return:
		nothing
*/
void delay_us(int us)
{
	for (int i = 0; i < us / 10; i++)
		_delay_us(160);
}

/*
	Description:
		Delay the system.
	Parameters:
		us: Set the delay in milliseconds.
	Return:
		nothing
*/void delay_ms(int ms)
{
	for (int i = 0; i < ms; i++)
		_delay_us(1000);
}