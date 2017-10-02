/*
 * digitalInput_Mega168.c
 *
 * Created: 2016/12/27 22:39:01
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	//SET D AS INPUT
	//#define PD2 2
	PORTD |= (1 << PD2);  /* initialize pullup resistor on our input pin */
	
	//SET B AS OUTPUT
	DDRB = 0xff;
	
    while (1) 
    {
	if(bit_is_clear(PIND, PD2)){
		//Equivalent to  if((PIND & (1 << PD2))==0)
		PORTB = 0b00111100;
    }
	else{
		PORTB = 0b11000011;
	}	
}
return(0);
}

