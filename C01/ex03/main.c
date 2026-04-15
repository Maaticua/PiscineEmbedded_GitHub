#include <avr/io.h>
#include <util/delay.h>


int main()
{
	DDRB |= (1 << PB1); // on set la LED PB1 en sortie
	TCCR1A |= (1 << COM1A1); // on configure le registre du timer pour activer le toggle on compare match p140

	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12); // on active le mode Compare Output Mode, Fast PWM ICR1 p142
	TCCR1B |= (1 << WGM13);

	TCCR1B |= (1 << CS12); // on choisi notre prescaler, ici 256 p143
	ICR1 = (F_CPU / 256) - 1; // et ici on divise la frequence du cpu par 256 pour avoir lequivalent de 1sec et -1 pour le overflow
	OCR1A = ((F_CPU / 256) -1) * 10/100; // notre match a 10%

	DDRD &= ~(0 << 2); //set register pos 2 to 0 (button) to set it on input
	DDRD &= ~(0 << 4); //set register pos 4 to 0 (button) to set it on input

	PORTD |= (1 << 2); // Active la résistance de pull-up interne du switch
	PORTD |= (1 << 4); // Active la résistance de pull-up interne du switch

	uint8_t counter = 10;
	uint8_t last_state_sw1 = 0;
	uint8_t last_state_sw2 = 0;

	while (1)
	{
		uint8_t current_state_sw1 = !(PIND & (1 << 2));

		if (current_state_sw1 == 1 && last_state_sw1 == 0) // on check si le boutton est presse et si il etait deja presse
		{
			if ( counter < 100 ) // un compteur pour ne pas depasser 100% (val max)
			{
				counter+= 10; // on ajoute 10 au conteur pour le calcul final
			}
		}
		_delay_ms(50); // delay to avoid the bounce
		last_state_sw1 = current_state_sw1; // updating de last state to avoid that the button activate infinitly if still pressed

		uint8_t current_state_sw2= !(PIND & (1 << 4));

		if (current_state_sw2 == 1 && last_state_sw2 == 0)
		{
			if ( counter > 10) // un compteur pour ne pas depasser la valeur min
			{
				counter-= 10; // on retire 10 pour le calcul final
			}
		}
		_delay_ms(50);
		last_state_sw2 = current_state_sw2;

		OCR1A = ((F_CPU / 256) -1) * counter/100; // on recalcule notre match avec la variable counter, ce qui nous permets d'augmenter ou diminuer le duty cycle
	}

}