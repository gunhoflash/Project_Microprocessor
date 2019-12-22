/*

	This header file defines methods to control gyro sensor: MPU-6050.

	Required PINs             : D
	Required external headers : delay.h

*/

// define code-number of data to read
#define GYRO_AX 1
#define GYRO_AY 2
#define GYRO_AZ 3
#define GYRO_GX 4
#define GYRO_GY 5
#define GYRO_GZ 6

/*
	Description:
		Read a data from gyro sensor.
	Parameters:
		addr: Address to access.
	Return:
		data.
*/
unsigned char gyro_read(char addr)
{
	unsigned char data;

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

	delay_ms(1);

	return data;
}

/*
	Description:
		Write a data to gyro sensor.
	Parameters:
		addr: Address to access.
		data: Data to write.
	Return:
		nothing
*/
void gyro_write(unsigned char addr, unsigned char data)
{
	delay_ms(5);

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

	TWDR = data;
	TWCR = 0x84;

	while ((TWCR & 0x80) == 0x00);
	while ((TWSR & 0xF8) != 0x28);

	TWCR |= 0x94;

	delay_ms(5);
}

/*
	Description:
		Get a data from gyro sensor.
	Parameters:
		code_number: Specify a code-number of data to read.
	Return:
		data.
*/
double gyro_get_data(int code_number)
{
	switch (code_number)
	{
		case GYRO_AX: return ((gyro_read(0x3B) << 8) | gyro_read(0x3C)) + 250; // add bias
		case GYRO_AY: return ((gyro_read(0x3D) << 8) | gyro_read(0x3E)) -  10; // add bias
		case GYRO_AZ: return ((gyro_read(0x3F) << 8) | gyro_read(0x40));
		case GYRO_GX: return ((gyro_read(0x43) << 8) | gyro_read(0x44));
		case GYRO_GY: return ((gyro_read(0x45) << 8) | gyro_read(0x46));
		case GYRO_GZ: return ((gyro_read(0x47) << 8) | gyro_read(0x48));
	}

	// handle exception: unknown code_number 
	return 0;
}

/*
	Description:
		Initialize gyro sensor.
	Parameters:
		nothing
	Return:
		nothing
*/
void gyro_init()
{
	TWCR = 0x04; // TWI
	TWSR = 0x00; // Prescalar: 1,
	TWBR = 0x12; // 0000 1100, Fscl = 400kHz

	gyro_write(0x6b, 0x00);
	gyro_write(0x6c, 0x00);
}
