#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD

void uart_init( unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); //setup le baudrate high | p185
	UBRR0L = (unsigned char)ubrr; // setup le baudrate low      | bauderate separer en deux car cest une valeur 12bits p185
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // on active le recepteur RXENO pour recevoir des donnees et le recepteur TXENO pour en emettre p185
}
void uart_tx(char c)
{
	while ( !( UCSR0A & (1<<UDRE0)) ) // indicateur pour savoir si le tampon est vide et est pret a recevoir un nouveau character p186
					;
	UDR0 = c;

}
char uart_rx(void)
{
	while ( !(UCSR0A & (1<<RXC0)) ) // attend de recevoir de la data
					;
	return UDR0; // retourne ce qu'il recoit
}
int main()
{
	uart_init(MYUBRR); // on initialise L'UART p184
	while (1)
	{
		uart_tx(uart_rx()); // on ecrit ce au'on retourne
	}
}