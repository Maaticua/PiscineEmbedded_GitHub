#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << PB1); // on set la LED PB1 en sortie
	TCCR1A |= (1 << COM1A1); // on configure le registre du timer pour activer le toggle on compare match p140

	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12); // on active le mode Compare Output Mode, Fast PWM ICR1 p142
	TCCR1B |= (1 << WGM13);

	TCCR1B |= (1 << CS12); // on choisi notre prescaler, ici 256 pg143
	ICR1 = (F_CPU / 256) - 1; // et ici on divise la frequence du cpu par 256 pour avoir lequivalent de 1sec, puis on divise par 2 pour la demi sec et -1 pour le overflow
	OCR1A = ((F_CPU / 256) -1) /10; // notre match a 10%















	while(1) {}
}