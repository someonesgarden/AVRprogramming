/*
 * atmega168CylonEyes.cpp
 *
 * Created: 2016/08/02 13:16:42
 * Author : Someones garden
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define DELAYTIME 85
#define LED_PORT	PORTB
#define LED_PIN		PINB
#define LED_DDR		DDRB


int mega168p_cylonEyes(void)
{
	uint8_t i = 0;
	LED_DDR = 0xff; //all set up for OUTPUT
	
    while (1) 
    {
		while(i < 7){
			LED_PORT  = (1<<i);
			//LED_PORT = _BV(i);
			
			_delay_ms(DELAYTIME);
			i = i + 1;
		}
		
		while(i > 0){
			LED_PORT = (1<<i);
			_delay_ms(DELAYTIME);
			i = i - 1;
		}
    }
	
	return (0);
}

