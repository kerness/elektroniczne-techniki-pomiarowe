/*
    ATmega16 microcontroller on STK500

    This code allows to manage seven-segment display - display numbers from 0 to 9 and changing them by pressing switch.
    Libraries are not included in repo!
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



#define _INT0 0
#define _INT1 1

volatile int current_interrupt;

ISR(INT0_vect){
	current_interrupt = _INT0;
}

ISR(INT1_vect) {
	current_interrupt = _INT1;
}

int main(void)
{
	
	DDRB  = 0xFF;

	MCUCR = _BV(ISC11) | _BV(ISC01);
	GICR  = _BV(INT0) | _BV(INT1);
	sei();

	int i = 0;
    // array with values - hex representation of numbers from zero to nine
	unsigned char numbers[10] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; 
	// on the begining display zero
    PORTB = numbers[0];
	
	
	while (1)
	{
		sleep_mode();
		switch (current_interrupt)
		{
			case _INT0:  // count up
			{
				if(i < 9) {
					i++;
					PORTB = numbers[i];
				}
			}
			break;

			case _INT1: // count down
			{
				if(i > 0) {
					i--;
					PORTB = numbers[i];
				}
			}
			break;
		}
	}
}