/*
 * HelloInterrupt_Mega168.c
 *
 * Created: 2016/12/30 23:15:47
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib/pinDefines.h"

ISR(INT0_vect){
	/*Run every time there is a change on button */
	/*BUTTON_PIN = PIND,  BUTTON = PD2*/
	uint8_t i;
	if(bit_is_set(BUTTON_PIN, BUTTON)){
		for(i=0;i<8;i++){
			LED_PORT |=(1<<i); //LED_PORT = PORTB
			_delay_ms(100);
		}
	}
	else{
		for(i=0;i<8;i++){
			LED_PORT &= ~(1<<i);
			_delay_ms(100);
		}
	}
}

void initInterrupt0(void){
	EIMSK |= (1<<INT0);  /* enable INT0 */
	EICRA |= (1<<ISC00); /* trigger when button changes */
	sei(); // Set (global)interrupt Enable Bit 
}

int mega168p_helloInterrupt(void)
{
	LED_DDR = 0xff; // ALL LEDS Active.
	BUTTON_PORT |= (1<<BUTTON); // BUTTON_PORT = PORTD, for PULL UP
	initInterrupt0();
	
    while (1) 
    {
		_delay_ms(200);
		LED_PORT ^= (1<<LED0);
    }
	
	return(0);
}

