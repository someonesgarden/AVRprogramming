/*
 * PWM_tiny2313.c
 *
 * Created: 2017/01/18 15:22:39
 * Author : Someones Garden
 */ 

#include <avr/io.h>


int tiny2313_PWM(void)
{
    unsigned char x, x0, n;
	int i;
	
	DDRB  |= (1<<PB3) | (1<<PB2) | (1<<PB1) | (1<<PB0);
	PORTA |= (1<<PA0) | (1<<PA1);
	
	TCCR0A = 0b10000011;
	TCCR0B = 0b00000011;
	
	
    while (1) 
    {
    }
}

