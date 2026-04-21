#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200 // baude rate
#define MYUBRR FOSC/16/BAUD  // calcul pour synchroniser la board et le pc


void USART_RX_vect(void) __attribute__((signal));  // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

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

void USART_RX_vect(void) // remplace ISR
{
	char c = UDR0;
	uart_tx(c);
}

void uart_printstr(const char* str)
{
	uint8_t i = 0;
	while (str[i])
		uart_tx(str[i++]); //ecrit ce qu'il recoit char par char
}

uint8_t check_UserNdPass(char *UserOrPass, uint8_t who)
{
	uint8_t i = 0; // compteur de char
	uint8_t isGood = 1; // check si le password/user est good, si = 0 c'est pas bon
	char c; // char qu'on recoit

	while (1)
	{
		c = uart_rx(); // je stock ce qu'on recoit
		if (c == '\r' || c == '\n') // check de la toucher entree
		{
			uart_printstr("\r\n");
			if (UserOrPass[i] != '\0') // check de la longeur du User ou Mdp
			{
				isGood = 0;
			}
			return (isGood); // on retourne isGood ici
		}
		else if (c == '\b' || c == 127) // check si on efface
		{
			if (i > 0) // on check si on a plus que 1 char
			{
				i--; // on decremente notre compteur de character des user ou Mdp
				uart_printstr("\b \b"); // on remplace le character par un espace
			}
			continue; // on skip le i++ de fin de boucle sinon le compteur est faux
		}
		else
		{
			if (who != 0) // check de si on rentre le Username ou le Password
				uart_tx('*'); // si password on ecrit * au lieu du character
			else
				uart_tx(c);

			if (c != UserOrPass[i]) // on check si le char stocker et le user ou mdp corresponde
				isGood = 0;
		}
		i++; // incremente le nombre le char recu pour le check de longeur a la fin
	}
}

int main()
{
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // on set le registre des leds pour qu'elles puissent s'allumer
	uart_init(MYUBRR);// on initialise L'UART p184
	char *username = "macaruan"; // on definit un username
	char *password = "banana"; // on definit un mdp

	uint8_t isUserGood = 1; // bool pour check si User bon
	uint8_t isPassGood = 1; // bool pour check si Pass bon

	while (1)
	{
		uart_printstr("Enter your login:\r\n\tusername: ");
		isUserGood = check_UserNdPass(username, 0); // on check si le user est bon, le deuxieme parametre est pour differencier le User du mdp
		uart_printstr("\tpassword: ");
		isPassGood = check_UserNdPass(password, 1); // on check si le mdp est bon, le deuxieme parametre est pour differencier le User du mdp

		if (isUserGood && isPassGood) // on check si les deux sont bon
		{
			uart_printstr("\rHello macaruan!\r\nShall we play a game?\r\n");
			for (uint8_t i = 0; i < 50; i++) // on fait clignoter les led si c'est good dans une boucle for et avec un XOR
			{
				PORTB ^= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
				_delay_ms(100);
			}
		}
		else // si pas bon message d'erreur
		{
			uart_printstr("\rBad combination username/password\r\n");
		}
		// on boucle a l'ifini
	}
}