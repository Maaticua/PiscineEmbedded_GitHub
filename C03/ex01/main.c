#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED (1 << PD3) | (1 << PD5) | (1 << PD6)

#define RED (1 << PD5)
#define GREEN (1 << PD6)
#define BLUE (1 << PD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (BLUE | RED)
#define WHITE (RED | GREEN | BLUE)


int main(void)
{
	DDRD |= LED;

	while (1)
	{
		PORTD ^= RED;
		_delay_ms(1000);
		PORTD ^= RED;

		PORTD ^= GREEN;
		_delay_ms(1000);
		PORTD ^= GREEN;

		PORTD ^= BLUE;
		_delay_ms(1000);
		PORTD ^= BLUE;

		PORTD ^= YELLOW;
		_delay_ms(1000);
		PORTD ^= YELLOW;

		PORTD ^= CYAN;
		_delay_ms(1000);
		PORTD ^= CYAN;

		PORTD ^= MAGENTA;
		_delay_ms(1000);
		PORTD ^= MAGENTA;

		PORTD ^= WHITE;
		_delay_ms(1000);
		PORTD ^= WHITE;
	}
}