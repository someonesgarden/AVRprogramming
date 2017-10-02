/*
 * SerialIO.c
 *
 * Created: 2016/12/23 20:00:31
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "../lib/USART.h"
#include "../lib/pinDefines.h"
/*
#define LED_PORT                PORTD
#define LED_PIN                 PIND
#define LED_DDR                 DDRD
*/


int mega168p_serialIO2(void)
{
	char serialCharacter;
	
	LED_DDR = 0xff;
	
	initUSART();
	printString("Hello World!\r\n");
	//_delay_ms(10);
	
    while (1) 
    {
		serialCharacter = receiveByte();
		//printBinaryByte(serialCharacter);
		transmitByte(serialCharacter);
		LED_PORT = serialCharacter;
		//_delay_ms(10);
    }
	
	return(0);
}

