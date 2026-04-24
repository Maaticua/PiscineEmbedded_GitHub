#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEDRGB (1 << PD3) | (1 << PD5) | (1 << PD6) // un define pour faciliter initialisation
#define LED (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)

#define RED (1 << PD5)                   // des define pour une meilleur lisibiliter des couleusr
#define GREEN (1 << PD6)
#define BLUE (1 << PD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (BLUE | RED)
#define WHITE (RED | GREEN | BLUE)

void init_rgb()
{
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); //on active le fast PWM avec WGM00 et WGM01 a 1 et le Clear Compare match pour COM0A1 et COM0B1 p113
	TCCR0B |= (1 << CS00); // on choisi No prescaling p116

	TCCR2A |= (1 << COM2B1)| (1 << WGM21) | (1 << WGM20); //on active le fast PWM avec WGM20 et WGM21 a 1 et le Clear Compare match pour COM2B1 p162
	TCCR2B |= (1 << CS20); // on choisi No prescaling p165
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r; // maj du rapport cyclique de la led p117
	OCR0A = g; // maj du rapport cyclique de la led p117
	OCR2B = b; // maj du rapport cyclique de la led p166
}

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3); //transition R vers B
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3); //transition B vers V
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0); //transition V vers R
	}
}

int main(void)
{
	DDRB |= LED;
	DDRD |= LEDRGB;
	init_rgb();

	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // on allume l'ADC avec ADEN p258 et on setup l'horloge de l'ADC p258 et https://marcjuneau.ca/?p=112
	ADMUX |= (1 << ADLAR) | (1 << REFS0); // ADLAR va nous permettre de "pousser" les 8 plus important bits dans le registre ADCH  p257

	uint8_t i = 0;

	while(1)
	{
		// potentiometre
		ADCSRA |= (1 << ADSC); // on active l'ADC Start Conversion p258
		while (ADCSRA & (1 << ADSC)){} // on attend la fin de la convertion p258
		i = ADCH;
		if (i < 64)
		{
			PORTB &= ~(1 << PB0);
		}
		else
		{
			PORTB |= (1 << PB0);
		}
		if (i < 128)
		{
			PORTB &= ~(1 << PB1);
		}
		else
		{
			PORTB |= (1 << PB1);
		}
		if (i < 192)
		{
			PORTB &= ~(1 << PB2);
		}
		else
		{
			PORTB |= (1 << PB2);
		}
		if (i < 255)
		{
			PORTB &= ~(1 << PB4);
		}
		else
		{
			PORTB |= (1 << PB4);
		}
		wheel(i);
		_delay_ms(20);
	}
}