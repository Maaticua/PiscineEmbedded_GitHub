#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << 0); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	DDRD &= ~(0 << 2); //set register pos 0 to 1 (button) to set it on input

	while (1)
	{
		if (!(PIND &= (1 << 2))) // check with an inverted AND so we can check if the button is pressed
		{
			PINB |= (1 << 0); // turn on the led
		}
		else
		{
			PINB |= (0 << 0); // turn off the led
		}
	}
}