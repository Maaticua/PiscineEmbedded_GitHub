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
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // on active le recepteur RXENO pour recevoir des donnees et le recepteur TXENO pour en emettre p185
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

uint8_t check_UserNdPass(char *UserOrPass)
{
	uint8_t i = 0;
	uint8_t isGood = 1;
	char c;

	while (1)
	{
		c = uart_rx();
		if (c == '\r' || c == '\n')
		{
			uart_printstr("\r\n");
			if (UserOrPass[i] != '\0')
			{
				isGood = 0;
			}
			return (isGood);
		}
		else if (c == '\b')
		{
			if (i > 0)
			{
				i--;
				uart_printstr("\b \b");
			}
		}
		else
		{
			if (UserOrPass[i] == '\0' || c != UserOrPass[i])
				isGood = 0;
		}
		i++;
	}
}

int main()
{
	uart_init(MYUBRR);// on initialise L'UART p184
	char *username = "macaruan";
	char *password = "banana";

	uint8_t isUserGood = 1;
	uint8_t isPassGood = 1;

	while (1)
	{
		uart_printstr("Enter your login:\r\n\tusername: ");
		isUserGood = check_UserNdPass("macaruan");
		uart_printstr("\tpassword: ");
		isPassGood = check_UserNdPass("banana");

		if (isUserGood && isPassGood)
		{
			uart_printstr("\rHello macaruan!\r\nShall we play a game?\r\n");
			DDRB |= (1 << PB5)
			for (uint8_t i = 0; i < 8; i++)
			{
				PORTB ^= (1 << PB5);
				_delay_ms(50);
			}
		}
		else
		{
			uart_printstr("\rBad combination username/password\r\n");
		}
		// SREG |= (1 << 7); // interupeur global
	}
}