/*
 * switchCounter_tiny2343.c
 *
 * Created: 2017/01/18 15:05:02
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>



int tiny2313_switchCounter(void)
{
	
	int i = 0;
	unsigned char x, x0;
	DDRB  |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);
	PORTA |= (1<<PA1) | (1<<PA0);
	
	x0 = PINA & 0b00000011;
	
    while (1) 
    {
		x = PINA & 0b00000011;
		if(x != x0){  //changed!
			if((x & 0b00000001)==0) i++;
			if((x & 0b00000010)==0) i--;
			
			PORTB = i;
			x0 = x;
		}
		_delay_ms(10);
		
	}
}

