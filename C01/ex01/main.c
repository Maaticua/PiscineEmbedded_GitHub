#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << PB1); // on set la LED PB1 en sortie
	TCCR1A |= (1 << COM1A0); // on configure le registre du timer pour activer le toggle on compare match p140
	TCCR1B |= (1 << WGM12); // on active le mode cler timer on compare match (ctc) et p142
	TCCR1B |= (1 << CS12); // on choisi notre prescaler, ici 256 pg143
	OCR1A = ((F_CPU / 256) / 2) - 1; // et ici on divise la frequence du cpu par 256 pour avoir lequivalent de 1sec, ensuite on divise par 2 et -1 pour gere loverflow

	while(1) {}

}