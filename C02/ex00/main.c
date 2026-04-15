#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD

void uart_init( unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void uart_tx(char c)
{
	while ( !( UCSR0A & (1<<UDRE0)) )
					;
	UDR0 = c;

}
int main()
{
	uart_init(MYUBRR);
	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}