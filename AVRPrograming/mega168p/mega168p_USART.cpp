/*
 * atmega168USART1.cpp
 *
 * Created: 2016/08/02 14:24:30
 * Author : someonesgarden
 */ 


#ifndef F_CPU
# warning "F_CPU not defined for <util/delay.h>"
# define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/USART.h"
#include "../lib/pinDefines.h"



int mega168_USART(void)
{
	char serialCharacter;
	
	//----//
	LED_DDR =0xff;
	initUSART();
	printString("Hello World!\r\n");
	
    while (1) 
    {
		serialCharacter = receiveByte();
		transmitByte(serialCharacter);
		LED_PORT = serialCharacter;
    }
	
	return(0);
}

