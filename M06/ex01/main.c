/* I2C = bus de communication uti pour connecter des peripherique a un MCU ou PC. Il pertme a plusieurs peripherique
de communiquer qu'avec deux fils : SDA (Data) SCL (Horloge). le protocole utilise un shema maitre-esclave,
le maitre initie la communication et l'esclave recoit les commandes.*/

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define BAUD 115200 // baude rate
#define SCL 100000
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
void	uart_puthexu8nbr(uint16_t n)
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

void print_hex_value(char c)
{
	char* hex = "0123456789ABCDEF";
	uart_tx(hex[(unsigned char)c >> 4]);
	uart_tx(hex[c & 0x0F]);
	uart_tx(' ');
}

void i2c_init(void)
{
	TWCR |= (1 << TWEN); // on active le protocole I2C p226
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // on choisi notre prescaler, ici 1 p 221-222 et p241
	TWBR = (((F_CPU / SCL) - 16) / 2); // le sujet demande un frequence de communication de 100 kHz. la formule ce trouve ici p222.

}

void i2c_start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // on cree une START condition en ecrivant ces valeurs p226
	while (!(TWCR & (1 << TWINT))){} // permet de mettre en pause temps que le bit de TWINT n'est pas = a 1 p225

}

void i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // on cree un STOP condition en ecrivant ces valeurs p226
	while (TWCR & (1 << TWSTO)){} // permet de mettre en pause temps que le bit de TWSTO n'est pas = a 0 p225
}

void i2c_write(unsigned char data)
{
	TWDR = data; // on stock notre parametre dnas le Data Register, c'est ce qui va nous servir de boite aux lettres
	TWCR = (1 << TWINT) | (1 << TWEN); // on declenche l'envoi p226-227
	while (!(TWCR & (1 << TWINT))){} // permet de mettre en pause temps que le bit de TWINT n'est pas = a 1 p225
}
void i2c_read(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // on declenche la reception p226-227
	while (!(TWCR & (1 << TWINT))){} // permet de mettre en pause temps que le bit de TWINT n'est pas = a 1 p225
	print_hex_value(TWDR);
}

int main(void)
{
	uart_init(MYUBRR); // initialisation de l'UART
	i2c_init(); // on initialise le protocole I2C

	while(1)
	{
		i2c_start(); // prise de controle du bus i2c
		i2c_write((0x38 << 1) | 0); // on vient choisir une adress, ici 0x38 est l'adresse du capteur AHT20 et on decale dun bit vers la gauche et on set 0 pour qu'il puisse ecrire p12 de https://datasheet4u.com/pdf/1551700/AHT20.pdf

		// on envoi la commande trigger mesurment et ses deux parametre p12 https://datasheet4u.com/pdf/1551700/AHT20.pdf
		i2c_write(0xAC); // octet de commande principal
		i2c_write(0x33); // param 1
		i2c_write(0x00); // param2

		i2c_stop(); // liberation du bus pour laisser le capteur travailler

		_delay_ms(80); // on laisse 80ms au capteur (preconisation constructeur) p12

		i2c_start(); // nouvelle prise de controle pour lire la reponse
		i2c_write((0x38 << 1) | 1);  // on vient choisir une adress, ici 0x38 est l'adresse du capteur AHT20 et un set 1 pour qu'il puisse lire p12 de https://datasheet4u.com/pdf/1551700/AHT20.pdf
		for (uint16_t i = 0; i < 7; i++)
			i2c_read(); // on lit l'octet, envoi le signal ACK et affiche
		i2c_stop(); // fin de la commuication
		uart_printstr("\r\n");
		_delay_ms(1000);
	}
}