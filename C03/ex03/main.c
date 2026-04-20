#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200 // baude rate
#define MYUBRR FOSC/16/BAUD  // calcul pour synchroniser la board et le pc

#define LED (1 << PD3) | (1 << PD5) | (1 << PD6) // un define pour faciliter initialisation

#define RED (1 << PD5)                   // des define pour une meilleur lisibiliter des couleusr
#define GREEN (1 << PD6)
#define BLUE (1 << PD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (BLUE | RED)
#define WHITE (RED | GREEN | BLUE)

void uart_init( unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);  //setup le baudrate high | p185
	UBRR0L = (unsigned char)ubrr;  // setup le baudrate low      | bauderate separer en deux car cest une valeur 12bits p185
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // on active le recepteur RXENO pour recevoir des donnees et le recepteur TXENO pour en emettre p185
}

void init_rgb()
{
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); //on active le fast PWM avec WGM00 et WGM01 a 1 et le Clear Compare match pour COM0A1 et COM0B1 p113
	TCCR0B |= (1 << CS00); // on choisi No prescaling p116

	TCCR2A |= (1 << COM2B1)| (1 << WGM21) | (1 << WGM20); //on active le fast PWM avec WGM20 et WGM21 a 1 et le Clear Compare match pour COM2B1 p162
	TCCR2B |= (1 << CS20); // on choisi No prescaling p165
}

void uart_tx(char c)
{
	while (!( UCSR0A & (1<<UDRE0))) {} // indicateur pour savoir si le tampon est vide et est pret a recevoir un nouveau character p186
	UDR0 = c;
}

char uart_rx(void)
{
	while ( !(UCSR0A & (1<<RXC0)) ) // attend de recevoir de la data
					;
	return UDR0; // retourne ce qu'il recoit
}

void uart_printstr(const char* str)
{
	uint8_t i = 0;
	while (str[i])
		uart_tx(str[i++]); //ecrit ce qu'il recoit char par char
}

uint8_t hex_to_int(char c)
{
	if (c >= '0' && c<= '9')
		return c - '0';
	if (c >= 'A' && c<= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 0;
}

uint8_t check_color()
{
	char c = uart_rx();

	if (c == '#') // si c'est le debut du format hexa
	{
		uart_tx(c); // affichage du '#'

		char buffer[6];
		for (uint8_t i = 0; i < 6; i++)
		{
			buffer[i] = uart_rx(); // lit les 6 char
			uart_tx(buffer[i]); // on les affiches
		}
		OCR0B = (hex_to_int(buffer[0]) << 4) | hex_to_int(buffer[1]);
		OCR0A = (hex_to_int(buffer[2]) << 4) | hex_to_int(buffer[3]);
		OCR2B = (hex_to_int(buffer[4]) << 4) | hex_to_int(buffer[5]);
	}
	else
	{
		uart_printstr("\r\nBad input: format #RRGGBB\r\n");
	}
}

int main(void)
{
	uart_init(MYUBRR); // on initialise L'uart
	DDRD |= LED; // on intialise les led
	init_rgb();

	while (1)
	{
		uart_printstr("\r\nEnter Hex RGB color format [#RRGGBB]: ");
		check_color();
	}
}