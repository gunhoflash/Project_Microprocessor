/*

	This header file defines a method to control BUZZER.

	Required PINs             : B
	Required external headers : delay.h

*/

// define sound frequencies
#define BUZZER_1 1047/2 // Do
#define BUZZER_2 1319/2 // Mi
#define BUZZER_3 1568/2 // Sol
#define BUZZER_4 1760/2 // La

/*
	Description:
		Sound a buzzer.
	Parameters:
		hz: Specify the frequency(hertz) of the sound.
	Return:
		nothing
*/
void buzzer(int hz)
{
	int i, ms, us;
	ms = (int)(500 / (float)hz);
	us = ((500 / (float)hz) - ms) * 1000;

	DDRB = 0x10;
	TCNT0 = 17;

	for (i = 0; i < 100; i++)
	{
		PORTB = 0x10;
		delay_ms(ms);
		delay_us(us);
		PORTB = 0x00;
		delay_ms(ms);
		delay_us(us);
	}
}