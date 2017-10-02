/*
 * pwmOnAnyPin_mega168p.c
 *
 * Created: 2017/01/05 19:54:15
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib/pinDefines.h"

#define DELAY 3

volatile uint8_t brightnessA;
volatile uint8_t brightnessB;

static inline void initTimer0(void){
		//TCCR0B |= (1<<CS01) | (1<<CS00);			/*Must be /64 or more for ISR timing */
		TCCR0B |= (1<<CS02);						/*Must be /64 or more for ISR timing */
		TIMSK0 |= (1<<OCIE0A) | (1<<OCIE0B);		/*OUTPUT COMPARE INTERRUPT ENABLE for OC0A, OC0B */
		TIMSK0 |= (1<<TOIE0);						/*OVERFLOW INTERRUPT ENABLE */
		sei();
}

//OVERFLOW INTERRUPT
ISR(TIMER0_OVF_vect){
	LED_PORT = 0xff;	//
	OCR0A = brightnessA;
	OCR0B = brightnessB;
}

//TIMER1 Compare Interrupt 
ISR(TIMER0_COMPA_vect){
	LED_PORT &= 0b11110000;		/*Turn off low four LEDS */
}

ISR(TIMER0_COMPB_vect){
	LED_PORT &= 0b00001111;		/* Turn off high four LEDS */
}

int mega168p_pwmOnAnyPin(void)
{
	// Initialization
	uint8_t i;
	LED_DDR = 0xff;
	initTimer0();
    while (1) 
    {
		for (i=0;i<255;i++){
			_delay_ms(DELAY);
			brightnessA = i;
			brightnessB = 255 -i;
		}
		
		for (i=254; i> 0; i--){
			_delay_ms(DELAY);
			brightnessA = i;
			brightnessB = 255 -i;
		}
    }
	
	return (0);
}

