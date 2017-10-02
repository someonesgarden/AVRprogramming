/*
 * atmega168ShowingOffBits.cpp
 *
 * Created: 2016/08/02 14:00:25
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#define DELAYTIME 85
#define LED_PORT		PORTB
#define LED_DDR			DDRB

#define BV(bit)			(1<<bit)
#define setBit(byte, bit)		(byte |=BV(bit))
#define clearBit(byte, bit)		(byte &=~(BV(bit))
#define toggleBit(byte, bit)	(byte ^=BV(bit))



int mega168p_showingOffBits(void)
{
    clock_prescale_set(clock_div_8);
	
	uint8_t i;
	uint8_t repetitions;
	uint8_t whichLED;
	uint16_t randomNumber = 0x1234;
	
	//----- Inits -----//
	LED_DDR = 0xff;
	
    while (1) 
    {
		
		//Turning ON
		for(i=0;i<8;i++){
			LED_PORT |= (1<<i);
			_delay_ms(DELAYTIME);
		}
		
		//Turning OFF
		for(i=0;i<8;i++){
			LED_PORT &=~(1<<i);
			_delay_ms(DELAYTIME);
		}
		
		for(i=7;i<255;i--){
			LED_PORT |= (1<<i);
			_delay_ms(DELAYTIME);
		}
		
		for (i=7;i<255;i--){
			LED_PORT &= ~(1<<i);
			_delay_ms(DELAYTIME);
		}
		
		_delay_ms(5 * DELAYTIME);
		
		
		for(repetitions=0; repetitions < 75; repetitions++){
			randomNumber =  2053 * randomNumber + 13849;
			
			whichLED = (randomNumber >> 8) & 0b00000111;
			LED_PORT ^= (1<<whichLED);
			_delay_ms(DELAYTIME);
		}
		LED_PORT = 0;
		_delay_ms(DELAYTIME*5);
		
    }
	
	return (0);
}

