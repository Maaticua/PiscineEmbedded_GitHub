#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200 // vitesse de com entre board et pc

void INT0_vect(void) __attribute__((signal)); // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

void INT0_vect(void)
{
	static uint8_t button_state = 1;
	if (TCNT1 > (((F_CPU / 256) * 5) / 100)) // si le temps ecouler et > a 50ms
	{
		button_state = !button_state; // on inverser l'etat memoriser, 1 = 0 et 0 = 1
		if (button_state == 0) // si 0 = le boutton est appuyer
		{
			PORTB ^= (1 << PB0); // on inverse l'etat de la led
		}
		TCNT1 = 0; // on reset le timer a 0;
	}
}

int main()
{
	DDRB |= (1 << PB0); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	DDRD &= ~(1 << PD2); //set register pos 0 to 1 (button) to set it on input
	PORTD |= (1 << PD2); // on active la resistance de pull up
	TCCR1B |= (1 << CS12); // prescaler 256 p143
	TCNT1 = 0; // initialise notre timer a 0

	EICRA |= (1 << ISC00); // configuration du falling edge sut INT0 pour generer une interuption p80
	EIMSK |= (1 << INT0); // registre d'external interupt pour le switch 1 p81
	EIFR |= (1 << INTF0); // securise mon demarage p81
	SREG |= (1 << 7);  // Global interuption, remplace sei() p20

	while (1) {}
}