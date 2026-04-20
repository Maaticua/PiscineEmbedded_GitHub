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
	DDRD |= LED; // configure les leds
	init_rgb(); //initialise les timers

	uint8_t i = 0;
	while (1)
	{
		wheel(i++); // parcours les couleurs smoothe
		_delay_ms(20); // delay pour la visibiliter
	}
}