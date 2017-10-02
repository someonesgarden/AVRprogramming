/*
 * BitTwiddling168P.c
 *
 * Created: 2016/12/27 13:11:26
 * Author : someonesgarden
 * ATMega168P
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#define DELAYTIME 85   /* milliseconds */

void POVDisplayMode(uint8_t oneByte, uint8_t mode){
	switch(mode){
		case 0:
		PORTB=oneByte;
		break;
		case 1:
		PORTC=oneByte;
		break;
		case 2:
		PORTD=oneByte;
		break;
		default:
		PORTB=oneByte;
	}
	_delay_ms(2);
}

int main(void)
{
	
	clock_prescale_set(clock_div_1);
	
	uint8_t i;
	uint8_t repetitions;
	
	uint8_t whichLED;
	uint16_t randomNumber = 0x1234;
	
	// ----- Inits ----//
	
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0xff;
	
	while (1)
	{
		/* Go Left */
		for (i=0; i< 8; i++){
			PORTB |=(1<<i);
			PORTC |=(1<<i);
			PORTD |=(1<<i);
			_delay_ms(DELAYTIME); /* wait */
		}
		for (i=0;i<8;i++){
			PORTB &=~(1<<i);
			PORTC &=~(1<<i);
			PORTD &=~(1<<i);
			_delay_ms(DELAYTIME);
		}
		
		/*Go Right */
		for (i=7;i<255;i--){
			PORTB |=(1<<i);
			PORTC |=(1<<i);
			PORTD |=(1<<i);
			_delay_ms(DELAYTIME);
		}
		for(i=7; i<255;i--){
			PORTB &=~(1<<i);
			PORTC &=~(1<<i);
			PORTD &=~(1<<i);
		}
		
		_delay_ms(5*DELAYTIME);
		
		/* Toggle "random" bits for a while */
		for(repetitions=0; repetitions<75;repetitions++){
			randomNumber = 2053 * randomNumber + 13849;
			whichLED = (randomNumber >> 8) & 0b00000111;
			PORTB ^=(1<<whichLED);
			PORTC ^=(1<<whichLED);
			PORTD ^=(1<<whichLED);
			_delay_ms(DELAYTIME);
		}
		PORTB=0;
		PORTC=0;
		PORTD=0;
		_delay_ms(5*DELAYTIME);
	}
	return(0);
}

