#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200 // vitesse de com entre board et pc
#define LED (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)

volatile uint8_t counter = 0; // on a deux fonction d'interuption qui vont se partager une variable donc on declare une globale et en plus si c'est des interuption il faut la mettre en volatile

void INT0_vect(void) __attribute__((signal)); // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

void INT0_vect(void)
{
	static uint8_t state = 1;
	state = !state;

	if (state == 0 &&counter < 15)
	{
		counter++;
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

void PCINT2_vect(void) __attribute__((signal)); // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

void PCINT2_vect(void)
{
	static uint8_t state = 1;
	state = !state;

	if (state == 0 && counter > 0)
	{
		counter--;
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

int main()
{
	DDRB |= LED; //set register pos 0 to 1 (led) to set it on output for the led to recieve information

	DDRD &= ~(1 << PD2); //set register pos 0 to 1 (button) to set it on input
	PORTD |= (1 << PD2); // on active la resistance de pull up
	EICRA |= (1 << ISC00); // configuration du falling edge sut INT0 pour generer une interuption p80
	EIMSK |= (1 << INT0); // registre d'external interupt pour le switch 1 p81
	EIFR |= (1 << INTF0); // securise mon demarage p81

	DDRD &= ~(1 << PD4); //set register pos 0 to 1 (button) to set it on input
	PORTD |= (1 << PD4); // on active la resistance de pull up
	PCICR |= (1 << PCIE2); // on active le pin change interup cpntrol register pour activer le groupe de PD4 (PCINT20) p82
	PCMSK2 |= (1 << PCINT20); // permet de cibler la broche du switch2 p20

	SREG |= (1 << 7); // interuption globale

	while (1) {}
}