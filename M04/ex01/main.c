#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200 // vitesse de com entre board et pc

void TIMER0_COMPA_vect(void) __attribute__((signal)); // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

void TIMER0_COMPA_vect(void)
{
	static uint8_t Up_Down = 0; // on  fait un bool pour savoir si on monte ou descend
	if (Up_Down == 0)
	{
		if (OCR1A == 255)
		{
			Up_Down = 1;
		}
		else
		{
			OCR1A++;
		}
	}
	else if (Up_Down == 1)
	{
		if (OCR1A == 0)
		{
			Up_Down = 0;
		}
		else
		{
			OCR1A--;
		}
	}
}

int main()
{
	DDRB |= (1 << PB1); //set register pos 0 to 1 (led) to set it on output for the led to recieve information
	TCCR1A |= (1 << COM1A1) | (1 << WGM10); // on set le compare match on set le mode fast PWM 8bit p141
	TCCR1B |= (1 << CS10) | (1 << WGM12); // on choisit no prescaler (sujet :La fréquence du Timer1 doit être assez élevée pour ne plus voir la LED clignoter) et on set le mode fast PWM 8bit mais ils sont sur deux registres different p141
	OCR1A = 0;

	TCCR0A |= (1 << WGM01); // on active le mode ctc sur Timer0 sinon c'est trop lent :c
	TCCR0B |= (1 << CS02); // on set up notre prescaler, 256 ici p116
	TIMSK0 |= (1 << OCIE0A); // on active l'overflow interupt p118
	OCR0A = ((F_CPU / 256) / 510)  ; // on definit le point de bascule : on a 2 x 255 etapes donc ces 510 etapes doivent se faire en 1sec.

	SREG |= (1 << 7); // interuption globale

	while (1) {}
}