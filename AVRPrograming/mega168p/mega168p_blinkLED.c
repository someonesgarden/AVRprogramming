/*
 * ATMega168_blinkLED.c
 *
 * Created: 2016/12/22 3:44:55
 * Author : me
 */ 

#include <avr/io.h> /* Defines pins ports, etc */
#include <util/delay.h> /*Functions to waste time */
#include <avr/power.h>

#define DELAYTIME 10 /* milli seconds */
#define LED_PORT PORTD
#define LED_PIN PIND
#define LED_DDR DDRD


int mega168p_blinkLED(void)
{
	clock_prescale_set(clock_div_8);
	
	uint8_t  i;
	uint8_t  repetitions;
	uint8_t  whichLED;
	uint16_t randomNumber = 0x1234;
	
	// ---- Inits ----//
	LED_DDR = 0xff; /* Data Direction Register B */
	// all LEDs configured for output //
	
    while (1) 
    {
		/* Go Left */
		for (i=0; i<8;i++){
			LED_PORT |= (1 <<i);
			/* Turn on i'th Pin */
			_delay_ms(DELAYTIME);
		}
		
		for (i=0;i<8;i++){
			LED_PORT &= ~(1 << i);
			/* Turn off i'th Pin */
			_delay_ms(DELAYTIME);
		}
		
		/*wait */
		_delay_ms(5*DELAYTIME);
		
		/* Go Right */
		for (i=7;i<255;i--){
			LED_PORT |= (1<< i);
			_delay_ms(DELAYTIME);
		}
		
		for (i=7;i<255;i--){
			LED_PORT &= ~(1<<i);
			_delay_ms(DELAYTIME);
		}
		
		_delay_ms(5*DELAYTIME);
		
		/* Toggle "random" bits for a while */
		
		for (repetitions=0; repetitions < 75; repetitions++){
			/* random number generator */
			randomNumber = 2053 * randomNumber + 13849;
			
			/* low three bits  from high byte */
			whichLED = (randomNumber >> 8) & 0b00000111;
			LED_PORT ^= (1 <<whichLED);
			_delay_ms(DELAYTIME);	
		}
		
		LED_PORT = 0;
		_delay_ms(5*DELAYTIME);
		
		
	}
	
	return(0);
}

