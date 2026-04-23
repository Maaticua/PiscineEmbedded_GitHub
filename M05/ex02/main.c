#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200 // baude rate
#define MYUBRR F_CPU/16/BAUD  // calcul pour synchroniser la board et le pc

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
void	uart_puthexu8nbr(uint8_t n)
{
	char* hex = "0123456789ABCDEF";

	if (n >= 16)
	{
		uart_puthexu8nbr(n / 16);
	}
	uart_tx(hex[n % 16]);
}

void	uart_putdecistr(uint16_t n)
{
	char* hex = "0123456789";

	if (n >= 10)
	{
		uart_putdecistr(n / 10);
	}
	uart_tx(hex[n % 10]);
}

int main(void)
{
	uint16_t i = 0;
	uart_init(MYUBRR);// on initialise L'UART p184

	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // on allume l'ADC avec ADEN p258 et on setup l'horloge de l'ADC p258 et https://marcjuneau.ca/?p=112
	ADMUX |= (1 << REFS0); // on active l'AVCC (le tension de reference) p257

	while(1)
	{
		ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2)); // on met tout a 0 tout le potentiometre
		// potentiometre
		ADCSRA |= (1 << ADSC); // on active l'ADC Start Conversion p258
		while (ADCSRA & (1 << ADSC)){} // on attend la fin de la convertion p258
		i = ADC;
		uart_putdecistr(i);
		uart_printstr(", ");

		// lumiere
		ADMUX &= ~((1 << MUX1) | (1 << MUX2)); // on force mux1 et mux2 a 0 pour la photoresistance
		ADMUX |= (1 << MUX0); // on force mux0 a 1 pour la photoresistance
		ADCSRA |= (1 << ADSC); // on active l'ADC Start Conversion p258
		while (ADCSRA & (1 << ADSC)){} // on attend la fin de la convertion p258
		i = ADC;
		uart_putdecistr(i);
		uart_printstr(", ");

		// temperature
		ADMUX &= ~((1 << MUX0) | (1 << MUX2)); // on force mux0 et mux2 a 0 pour la photoresistance
		ADMUX |= (1 << MUX1);  // on force mux1 a 1 pour la photoresistance
		ADCSRA |= (1 << ADSC); // on active l'ADC Start Conversion p258
		while (ADCSRA & (1 << ADSC)){} // on attend la fin de la convertion p258
		i = ADC;
		uart_putdecistr(i);
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}

	// LDR PC1 NTC PC2
	// MUX3    MUX2    MUX1    MUX0    Pin that will be read
	// 0        0       0       0        PORTC0    Analogic0 RV1 ADC (potentiometre)
	// 0        0       0       1        PORTC1    Analogic1 R14 LDR (lumiere)
	// 0        0       1       0        PORTC2    Analogic2 R20 NTC (temperature)