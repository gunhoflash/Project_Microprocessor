/*

	This header file defines methods to transmit data using USART communication.

	Required PINs             : nothing
	Required external headers : nothing

*/

/*
	Description:
		Initialize.
	Parameters:
		nothing
	Return:
		nothing
*/
void usart_init()
{
	// UART
	UCSR0A = 0x00;
	UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0x67; // set speed as 9600
}

/*
	Description:
		Transmit a character data.
	Parameters:
		tx_data: Data to transmit.
	Return:
		nothing
*/
void usart_transmit(unsigned char tx_data)
{ 
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = tx_data; 
}

/*
	Description:
		Transmit a integer data.
	Parameters:
		data: Data to transmit.
	Return:
		nothing
*/
void usart_transmit_init4(int data)
{
	if (data < 0)
	{
		data = -data;
		usart_transmit('-');
	}
	else
		usart_transmit(' ');

	int temp = 0;
	temp = data / 10000;
	usart_transmit(temp + 48);
	temp = (data % 10000) / 1000;
	usart_transmit(temp + 48);
	temp = (data %  1000) / 100;
	usart_transmit(temp + 48);
	temp = (data %   100) / 10;
	usart_transmit(temp + 48);
	temp = (data %    10);
	usart_transmit(temp + 48);
}