/*
 * dcMotorWorkout_mega168p.c
 *
 * Created: 2017/01/12 13:42:38
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"

#define SPEED_STEP_DELAY 2 /* milliseconds */

static inline void initTimer0(void){
	TCCR0A |=(1<<WGM00) | (1<<WGM01);	/* Fast PWM mode */
	TCCR0A |=(1<<COM0B1);	/* Output PWM Pin */
	TCCR0B |=(1<<CS02);		/* Clock with /1024 prescaler */
}


int mega168p_dcMotorWorkout(void)
{
	uint8_t updateSpeed;
	
	//----- Inits ----//
	initTimer0();
	OCR0B = 0;
	
	ANTENNA_DDR |= (1<<ANTENNA);  /* OC0B */
	LED_DDR |=(1<<LED0) | (1<<LED1);
	initUSART();
	
	printString("DC Motor Workout\r\n");
	
	
    while (1) 
    {
		updateSpeed = getNumber();
		
		if(OCR0B < updateSpeed){
			LED_PORT |= (1<<LED0);
			while (OCR0B < updateSpeed){
				OCR0B++;
				_delay_ms(SPEED_STEP_DELAY);
			}
		}
		else {
			LED_PORT |=(1<<LED1);
			while(OCR0B > updateSpeed){
				OCR0B--;
				_delay_ms(SPEED_STEP_DELAY);
			}
		}
		LED_PORT = 0;
	}
	
	return (0);
}

