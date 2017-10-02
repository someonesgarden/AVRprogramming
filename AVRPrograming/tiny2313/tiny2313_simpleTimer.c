/*
 * simpleTimer_tiny2313.c
 *
 * Created: 2017/01/18 15:16:16
 * Author : Someones Garden
 */ 

#include <avr/io.h>


int tiny2313_simpleTimer(void)
{
    unsigned char x;
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);
	
	TCCR0A = 0; //default
	TCCR0B |=(1<<CS02) | (1<<CS00); //clk/1024 = 1m sec
	
    while (1) 
    {
		x = TCNT0; // Count number
		x = x >> 4; //upper 4 bit
		PORTB = x; 
    }
}

