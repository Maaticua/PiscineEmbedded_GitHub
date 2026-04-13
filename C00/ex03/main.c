#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB |= (1 << 0); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	DDRD &= ~(0 << 2); //set register pos 0 to 1 (button) to set it on input
	PORTD |= (1 << PD2); // Active la résistance de pull-up interne sur l'entrée PD2

	uint8_t isOn = 0;

	while (1) //loop
	{
		if (!(PIND &= (1 << 2))) // check with an inverted AND so we can check if the button is pressed
		{
			PINB |= (1 << 0); // turn on light
		}
		else
		{
		}
		_delay_ms(100); //delay pour eviter le bonce
	}
}