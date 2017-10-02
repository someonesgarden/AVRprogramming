/*
 * SerialIO_Mega168P.c
 *
 * Created: 2016/12/27 13:52:38
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/USART.h"

int mega168p_serialIO(void)
{
	char serialCharacter;
	DDRB = 0xff;
	initUSART();
	printString("Hello World!\r\n");
	
	
    while (1) 
    {
		serialCharacter = receiveByte();
		transmitByte(serialCharacter);
		printBinaryByte(serialCharacter);
		PORTB = serialCharacter;
		
	}
}

