#include <avr/io.h>
#include <util/delay.h>


int main()
{
	DDRB |= (1 << 0); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	DDRB |= (1 << 1); //set register pos 1 to 1 (led) to set it on output for the led to recieve information
	DDRB |= (1 << 2); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	DDRB |= (1 << 4); //set register pos 0 to 1 (led) to set it on output for the led to recieve information

	DDRD &= ~(0 << 2); //set register pos 2 to 0 (button) to set it on input
	DDRD &= ~(0 << 4); //set register pos 4 to 0 (button) to set it on input

	PORTD |= (1 << 2); // Active la résistance de pull-up interne
	PORTD |= (1 << 4); // Active la résistance de pull-up interne

	uint8_t counter = 0;
	uint8_t last_state_sw1 = 0;
	uint8_t last_state_sw2 = 0;

	while (1)
	{
		uint8_t current_state_sw1 = !(PIND & (1 << 2)); // assigne to current_state if the button is pressed

		if (current_state_sw1 == 1 && last_state_sw1 == 0) // check if the button is pressed and the last state of the button
		{
			if ( counter < 15 ) // counter to avoid goin more than 15
			{
				counter++;
			}
		}
		_delay_ms(50); // delay to avoid the bounce
		last_state_sw1 = current_state_sw1; // updating de last state to avoid that the button activate  infinitly if still pressed

		uint8_t current_state_sw2= !(PIND & (1 << 4));

		if (current_state_sw2 == 1 && last_state_sw2 == 0)
		{
			if ( counter > 0)
			{
				counter--;
			}
		}
		_delay_ms(50);
		last_state_sw2 = current_state_sw2;

		if (counter < 8) // if under 8 we can assigne portb to the counter value for the led to turn on
		{
			PORTB = counter; // we turn on the led, ex : if counter = 4 = 0100 the 3th led turn on
		}
		else
		{
			PORTB = (counter % 8) | (1 << 4); // above 8 we need to do the modulo of counter by 8, so for exemple we have 11 % 8 = 3 = 0011 so we turn on the first two led and afer that we manualy turn on the 4th led
		}

	}
}