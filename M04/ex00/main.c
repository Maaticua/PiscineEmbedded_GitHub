#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200

void INT0_vect(void) __attribute__((signal)); // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

void TIMER1_COMPA_vect(void) __attribute__((signal));

void TIMER1_COMPA_vect(void)
{
	
}

void INT0_vect(void)
{
	PORTB ^= (1 << PB0);
}

int main()
{
	DDRB |= (1 << PB0); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	DDRD &= ~(1 << PD2); //set register pos 0 to 1 (button) to set it on input
	PORTD |= (1 << PD2); // on active la resistance de pull up

	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	EIFR |= (1 << INTF0);
	SREG |= (1 << 7);  // Global interuption

	while (1) {}
}