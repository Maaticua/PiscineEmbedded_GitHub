#include <avr/io.h>

int main (void)
{
	DDRB |= (1 << 0); //set register pos 0 to 1 (led)  to set it on output for the led to recieve information
	PORTB |= (1 << 0); //set pin to turn on the led

	while(1){} // loop
}
