#define AX 1
#define AY 2
#define AZ 3
#define GX 4
#define GY 5
#define GZ 6

/*

	Gyro Sensor Functions
	- gyro_write
	- gyro_read
	- set_fnd4

*/

void gyro_init()
{
	TWCR = 0x04; // TWI
	TWSR = 0x00; // Prescalar: 1,
	TWBR = 0x12; // 0000 1100, Fscl = 400kHz

	gyro_write(0x6b,0x00);
	gyro_write(0x6c,0x00);
}

/*
	Read data
	USED PIN: B0, B1
*/
unsigned char gyro_read(char addr)
{
	unsigned char data;

	TWCR = 0xA4;

	//while((TWCR & 0x80) == 0x00 || (TWSR & 0xF8) != 0x08);
	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x08);

	TWDR = 0xD0;
	TWCR = 0x84;

	//while((TWCR & 0x80) == 0x00 || (TWSR & 0xF8) != 0x18);
	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x18);

	TWDR = addr;
	TWCR = 0x84;

	//while((TWCR & 0x80) == 0x00 || (TWSR & 0xF8) != 0x28);
	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x28);

	TWCR = 0xA4;

	//while((TWCR & 0x80) == 0x00 || (TWSR & 0xF8) != 0x10);
	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x10);

	TWDR = 0xD1;
	TWCR = 0x84;

	//while((TWCR & 0x80) == 0x00 || (TWSR & 0xF8) != 0x40);
	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x40);

	TWCR = 0x84;

	//while((TWCR & 0x80) == 0x00 || (TWSR & 0xF8) != 0x58);
	while((TWCR & 0x80) == 0x00);
	while((TWSR & 0xF8) != 0x58);

	data = TWDR; 

	TWCR = 0x94;

	delay(1);

	return data;
}

/*
	Write data
	USED PIN: B0, B1
*/
void gyro_write(unsigned char addr,unsigned char dat)
{
	delay(5);

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

	delay(5);
}

double gyro_get_data(int type)
{
	switch (type)
	{
		case AX: return ((gyro_read(0x3B) << 8) | gyro_read(0x3C));
		case AY: return ((gyro_read(0x3D) << 8) | gyro_read(0x3E));
		case AZ: return ((gyro_read(0x3F) << 8) | gyro_read(0x40));
		case GX: return ((gyro_read(0x43) << 8) | gyro_read(0x44));
		case GY: return ((gyro_read(0x45) << 8) | gyro_read(0x46));
		case GZ: return ((gyro_read(0x47) << 8) | gyro_read(0x48));
	}

	// handle exception: unknown type 
	return 0;
}
