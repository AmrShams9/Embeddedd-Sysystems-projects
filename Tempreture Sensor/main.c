#include <avr/io.h>          // Include AVR IO library
#include <util/delay.h>      // Include delay library
#include <stdio.h>           // Include standard I/O library

#define F_CPU 16000000UL     // Define CPU frequency

void USART_Init(unsigned int baud_rate)
{
	UBRRH = (unsigned char)(baud_rate >> 8);  // Set baud rate (high byte)
	UBRRL = (unsigned char)baud_rate;         // Set baud rate (low byte)
	UCSRB = (1 << TXEN) | (1 << RXEN);        // Enable transmitter and receiver
	UCSRC = (1 << UCSZ1) | (1 << UCSZ0) | (1 << URSEL);  // Set frame format: 8 data bits, 1 stop bit, no parity
}

void USART_Transmit(unsigned char data)
{
	while (!(UCSRA & (1 << UDRE)));  // Wait until transmit buffer is empty
	UDR = data;                      // Put data into buffer, sends the data
}

int main(void)
{
	float temperature;   // Declare a variable to hold temperature value
	char buffer[10];     // Declare a character array to hold temperature value as a string

	// Set up ADC
	ADMUX |= (1 << REFS0);                         // Set reference voltage to AVcc
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // Set ADC prescaler to 128
	ADCSRA |= (1 << ADEN);                        // Enable ADC

	// Initialize USART
	USART_Init(103);                              // Set USART baud rate to 9600

	while (1)
	{
		// Read temperature from LM35 sensor
		ADCSRA |= (1 << ADSC);                     // Start ADC conversion
		while (ADCSRA & (1 << ADSC));              // Wait until ADC conversion is complete
		uint16_t adc_value = ADC;                  // Read ADC value
		temperature = adc_value * 0.4887;          // Convert ADC value to temperature

		// Convert temperature to string and transmit over USART
		sprintf(buffer, "%.1f", temperature);      // Convert temperature to string with one decimal place
		USART_Transmit('T');                        // Send start character
		USART_Transmit(':');                        // Send separator character
		USART_Transmit(buffer[0]);                  // Send first character of temperature string
		USART_Transmit(buffer[1]);                  // Send second character of temperature string
		USART_Transmit('.');                        // Send decimal point
		USART_Transmit(buffer[2]);                  // Send third character of temperature string
		USART_Transmit('\n');                       // Send end-of-line character

		_delay_ms(500);                             // Wait for 500 milliseconds before reading temperature again
	}
}
