#include <avr/io.h>
#define F_CPU 1000000UL  // set the clock frequency to 1 MHz
#include <util/delay.h>

   // include the avr/io.h header file

#define LED_PIN 13    // define the LED pin as 13

int main(void)
{
	DDRB |= (1 << LED_PIN);  // set the LED pin as an output
	while (1) {              // loop forever
		PORTB |= (1 << LED_PIN); // turn on the LED
		_delay_ms(1000);      // wait for 1 second
		PORTB &= ~(1 << LED_PIN); // turn off the LED
		_delay_ms(1000);      // wait for 1 second
	}
	return 0;
}

