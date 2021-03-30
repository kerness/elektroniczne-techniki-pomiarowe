/*
    ATmega16 microcontroller on STK500

    This code allows to light up diodes on STK500
    Libraries are not included in repo!
*/


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define _INT0 0
#define _INT1 1

volatile int current_interrupt; // for registering interups

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

    // only first diode
	int i = 0;
	PORTB = ~(_BV(0));
	
	while (1)
	{
		sleep_mode();
		switch (current_interrupt)
		{
			case _INT0: // light "upper" diode - detecting switch click
			{
				if(i < 7) {
					i++;
					PORTB = ~(_BV(i));
				}
			}
			break;

			case _INT1: // light "lower" diode - detecting switch click
			{
				if(i > 0) {
					i--;
					PORTB = ~(_BV(i));
				}
			}
			break;
		}
	}
}