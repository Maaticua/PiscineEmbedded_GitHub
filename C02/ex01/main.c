#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD  // calcul pour synchroniser la board et le pc

void TIMER1_COMPA_vect(void) __attribute__((signal)); // on declare un vecteur d'interuption et on ajoute des instruction speciale au compilateur, 1er sauvgarder letat et 2e reti p74

void uart_init( unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);  //setup le baudrate high | p185
	UBRR0L = (unsigned char)ubrr;  // setup le baudrate low      | bauderate separer en deux car cest une valeur 12bits p185
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // on active le recepteur RXENO pour recevoir des donnees et le recepteur TXENO pour en emettre p185
}
void uart_tx(char c)
{
	while ( !( UCSR0A & (1<<UDRE0)) ) // indicateur pour savoir si le tampon est vide et est pret a recevoir un nouveau character p186
	;
	UDR0 = c;
}
void uart_printstr(const char* str)
{
	uint8_t i = 0;
	while (str[i])
		uart_tx(str[i++]); // on boucle pour ecrire character pas character
}
void TIMER1_COMPA_vect(void)
{
	uart_printstr("Hello World!\r\n");
}
int main()
{
	uart_init(MYUBRR);// on initialise L'UART p184
	SREG |= (1 << 7); // interupeur global
	TCCR1B |= (1 << WGM12); //  on active le mode clear timer on compare match (ctc) et p142
	TCCR1B |= (1 << CS10); // |
	TCCR1B |= (1 << CS12); // |on choisi notre prescaler, ici 1024 pour contenir les 2sec p143

	TIMSK1 |= (1 << OCIE1A); // on active

	OCR1A = ((F_CPU / 1024) * 2 -1);

	while (1) {}
}