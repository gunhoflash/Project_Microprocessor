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
#include <avr/interrupt.h>
#include <math.h>

#define F_CPU 16000000UL

// not used yet
typedef unsigned char byte;

/*

	Common Functions
	- USART_Transmit
	- USART_Transmit_init4
	- delay
	- set_fnd1
	- set_fnd4

*/

/*
	Transmit data
*/
void USART_init()
{
	// UART
	UCSR0A = 0x00;
	UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0x67; // set speed as 9600
}
void USART_Transmit(unsigned char tx_data)
{ 
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = tx_data; 
}
void USART_Transmit_init4(int data)
{
	if (data < 0)
	{
		data = -data;
		USART_Transmit('-');
	}
	else
		USART_Transmit(' ');

	int temp = 0;
	temp = data / 10000;
	USART_Transmit(temp + 48);
	temp = (data % 10000) / 1000;
	USART_Transmit(temp + 48);
	temp = (data %  1000) / 100;
	USART_Transmit(temp + 48);
	temp = (data %   100) / 10;
	USART_Transmit(temp + 48);
	temp = (data %    10);
	USART_Transmit(temp + 48);
}

void delay(int ms)
{
	volatile int i, j, t = F_CPU / 22000;

	ms -= 5;
	for (i = 0; i < ms; i++)
		for (j = 0; j < t; j++);
}

/*
	Turn on 1 FND
	USED PIN: C G
	select: 0, 1, 2, 3
	data: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
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

	// Select the FND to on
	PORTG = 0x8 >> (select % 4);

	// On FND with data
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
	Turn on 4 FND
	USED PIN: C G
*/
void set_fnd4(int data, unsigned int ms)
{
	unsigned int time = 5;
	unsigned int max_time = ms * F_CPU / 1777000;
	int data0 = (data / 1000) % 10;
	int data1 = (data / 100) % 10;
	int data2 = (data / 10) % 10;
	int data3 = (data / 1) % 10;

	while (time < max_time)
	{
		set_fnd1(0, data0); _delay_ms(2);
		set_fnd1(1, data1); _delay_ms(2);
		set_fnd1(2, data2); _delay_ms(2);
		set_fnd1(3, data3); _delay_ms(2);
		time++;
	}
}

/*

	Gyro Sensor Functions
	- gyro_write
	- gyro_read
	- set_fnd4

*/

/*
	Write data
	USED PIN: B0, B1
*/
void gyro_write(unsigned char addr,unsigned char dat)
{
	//PORTD = 0x02;

	_delay_us(50);

	TWCR = 0xA4;

	while ((TWCR & 0x80) == 0x00);
	while ((TWSR & 0xF8) != 0x08);

	TWDR = 0xD0;
	TWCR = 0x84;

	while ((TWCR & 0x80) == 0x00);
	while ((TWSR & 0xF8) != 0x18);

	TWDR = addr;
	TWCR = 0x84;

	while ((TWCR & 0x80) == 0x00);
	while ((TWSR & 0xF8) != 0x28);

	TWDR = dat;
	TWCR = 0x84;

	while ((TWCR & 0x80) == 0x00);
	while ((TWSR & 0xF8) != 0x28);

	TWCR |= 0x94;

	_delay_us(50);
}

/*
	Write data
	USED PIN: B0, B1
*/
unsigned char gyro_read(char addr)
{
	unsigned char data;

	//PORTD |= 0x02;

	TWCR = 0xA4;

	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x08);

	TWDR = 0xD0;
	TWCR = 0x84;

	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x18);

	TWDR = addr;
	TWCR = 0x84;

	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x28);

	TWCR = 0xA4;

	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x10);

	TWDR = 0xD1;
	TWCR = 0x84;

	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x40);

	TWCR = 0x84;

	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x58);

	data = TWDR; 

	TWCR = 0x94;

	// delay(1);

	return data;
}

int main()
{
	int i, j;

	// variables for gyro sensor: MPU-6050
	volatile unsigned char axl, axh, ayl, ayh, azl, azh, gxl, gxh, gyl, gyh, gzl, gzh; 
	volatile double a_x, a_y, a_z, g_x, g_y, g_z;

	USART_init();
	delay(1);
	
	//*(char *)0x3A = 0xff;
	//*(char *)0x3B = 0xff;
	DDRA = 0xff; // 1111 1111 : set dda7 ~ dda0 to write LED
	DDRC = 0xff; // 1111 1111 : set ddc7 ~ ddc0 to write FND DATA

	DDRG = 0x0f; // 0000 1111 : set ddg3 ~ ddg0 to write FND SELECT
	TWCR = 0x04; // TWI
	TWSR = 0x00; // Prescalar: 1,
	TWBR = 0x12; // 0000 1100, Fscl = 400kHz

	PORTA = 0x80;

	gyro_write(0x6b,0x00);
	gyro_write(0x6c,0x00);

	while (1) {

		axl = gyro_read(0x3B);
		axh = gyro_read(0x3C);
		ayl = gyro_read(0x3D);
		ayh = gyro_read(0x3E);
		azl = gyro_read(0x3F);
		azh = gyro_read(0x40);

		gxl = gyro_read(0x43);
		gxh = gyro_read(0x44);
		gyl = gyro_read(0x45);
		gyh = gyro_read(0x46);
		gzl = gyro_read(0x47);
		gzh = gyro_read(0x48);

		a_x = (axl << 8) | axh;
		a_y = (ayl << 8) | ayh;
		a_z = (azl << 8) | azh;
		g_x = (gxl << 8) | gxh;
		g_y = (gyl << 8) | gyh;
		g_z = (gzl << 8) | gzh;

		PORTA = 0xe0;

		// print values for test
		USART_Transmit_init4(a_x);
		USART_Transmit('\t');
		USART_Transmit_init4(a_y);
		USART_Transmit('\t');
		USART_Transmit_init4(a_z);
		USART_Transmit('\t');
		USART_Transmit_init4(g_x);
		USART_Transmit('\t');
		USART_Transmit_init4(g_y);
		USART_Transmit('\t');
		USART_Transmit_init4(g_z);
		USART_Transmit('\n');
		USART_Transmit('\r');

		/*
		 0 PIN 1
		LED
		 2     3

		0: +++
		1: +-+
		2: -++
		3: --+
		
		*/
		i = 0;
		if (a_x < 0) i = 2;
		if (a_y < 0) i++;
		set_fnd1(i, 8);

		// 16384
		j = sqrt((pow(a_x/16384.0, 2) + pow(a_y/16384.0, 2)) / 2) * 8;
		PORTA = 0xff ^ (0xff << j);

		USART_Transmit(i);
		USART_Transmit('\n');
		USART_Transmit('\r');

		delay(50);
	}
}
