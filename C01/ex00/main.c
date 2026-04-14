#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << 0); //set register pos 0 to 1 (led) to set it on output for the led to recieve information

	volatile uint32_t i; // on cree une variable qui peut contenir un grand nombre pour la boucle for + volatile sinon le flag Os du compilateur va ignore la boucle for

	while (1)
	{
		PORTB ^= (1 << 0); // on inverse l'etat de la led
		for (i = 0; i < 250000; i++) {} // notre "timer" :)
	}
}