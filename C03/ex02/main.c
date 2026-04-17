#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED (1 << PD3) | (1 << PD5) | (1 << PD6)

#define RED (1 << PD5)
#define GREEN (1 << PD6)
#define BLUE (1 << PD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (BLUE | RED)
#define WHITE (RED | GREEN | BLUE)


void init_rgb()
{
	
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{

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


}