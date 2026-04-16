#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD

void USART_RX_vect(void) __attribute__((signal)); // p74

void uart_init( unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);  //setup le baudrate high | p185
	UBRR0L = (unsigned char)ubrr;  // setup le baudrate low      | bauderate separer en deux car cest une valeur 12bits p185
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0); // on active le recepteur RXENO pour recevoir des donnees et le recepteur TXENO pour en emettre p185
}

void uart_tx(char c)
{
	while (!( UCSR0A & (1<<UDRE0))) {} // indicateur pour savoir si le tampon est vide et est pret a recevoir un nouveau character p186
	UDR0 = c;
}

void USART_RX_vect(void)
{
	char c = UDR0;
	uart_tx(c);
}

void uart_printstr(const char* str)
{
	uint8_t i = 0;
	while (str[i])
		uart_tx(str[i++]);
}

uint8_t Username_check(char *username)
{
	uint8_t isUsernameGood = 0;

	/* check du username bit par bit pour eviter une allocation de char* pour comparer */

	uart_printstr("\r\n\tpassword:");
	return (isUsernameGood);
}
uint8_t Password_check(char *password)
{
	uint8_t isPasswordGood = 0;

	/* check du password bit par bit pour eviter une allocation de char* pour comparer */

	return(isPasswordGood);
}

int main()
{
	uart_init(MYUBRR);// on initialise L'UART p184
	uint8_t areBothGood = 0;

	char *username = "macaruan";
	char *password = "banana";

	uart_printstr("Enter your login:\r\n\tusername:");

	areBothGood = Username_check(username);
	areBothGood = Password_check(password);

	if (!areBothGood)
		uart_printstr("\rBad combinasion username/password\r\n");
	else
		uart_printstr("\rHello macaruan\r\n");


	SREG |= (1 << 7); // interupeur global

	while (1) {}
}