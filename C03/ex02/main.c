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
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS00);

	TCCR2A |= (1 << COM2B1)| (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS20);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

int main(void)
{
	DDRD |= LED;
	init_rgb();

	uint8_t i = 0;
	while (1)
	{
		wheel(i++);
		_delay_ms(20);
	}
}