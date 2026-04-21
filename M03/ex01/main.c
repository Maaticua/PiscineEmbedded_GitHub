#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED (1 << PD3) | (1 << PD5) | (1 << PD6) // simplifie l'initialisation des leds

#define RED (1 << PD5) //                des defines pour rendre les couleur plus lisible
#define GREEN (1 << PD6)
#define BLUE (1 << PD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (BLUE | RED)
#define WHITE (RED | GREEN | BLUE)


int main(void)
{
	DDRD |= LED; // initialisation des led

	while (1)
	{
		PORTD ^= RED;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= RED;// inverse l'etat de la led avec un XOR

		PORTD ^= GREEN;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= GREEN;// inverse l'etat de la led avec un XOR

		PORTD ^= BLUE;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= BLUE;// inverse l'etat de la led avec un XOR

		PORTD ^= YELLOW;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= YELLOW;// inverse l'etat de la led avec un XOR

		PORTD ^= CYAN;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= CYAN;// inverse l'etat de la led avec un XOR

		PORTD ^= MAGENTA;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= MAGENTA;// inverse l'etat de la led avec un XOR

		PORTD ^= WHITE;// inverse l'etat de la led avec un XOR
		_delay_ms(1000); // delay de 1sec
		PORTD ^= WHITE;// inverse l'etat de la led avec un XOR
	}
}