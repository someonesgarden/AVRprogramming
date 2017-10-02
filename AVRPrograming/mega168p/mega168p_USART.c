/*
 * atmega168USART1.cpp
 *
 * Created: 2016/08/02 14:24:30
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "../lib/USART.h"
#include "../lib/pinDefines.h"


int mega168p_USART(void)
{
	char serialCharacter;
	
	//----//
	LED_DDR = 0xff;
	
	initUSART();
	printString("Hello World!\r\n");
	_delay_ms(100);
	
    while (1) 
    {
		printString("HelloAgain\r\n");
		serialCharacter = receiveByte();
		printString(serialCharacter+"");
		transmitByte(serialCharacter);
		LED_PORT = serialCharacter;
		
		_delay_ms(60);	
		
		
		
    }
	
	return(0);
}

