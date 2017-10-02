/*
 * amRadio_mega168p.c
 *
 * Created: 2017/01/05 9:51:47
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "scale16.h"

#define COUNTER_VALUE 3  /*Determines carrier frequency */

// From f = F_CPU / (2*N*(1+OCRnx))
//8MHz / (2*1*(1+2)) = 8 / 6 MHz = 1333kHz
//8MHz / (2*1*(1+3)) = 8 / 8 MHz = 1000kHz
//8MHz / (2*1*(1+4)) = 8 / 10MHz = 800kHz
//8MHz / (2*1*(1+5)) = 8 / 12MHz = 670kHz
//8MHz / (2*1*(1+6)) = 8 / 14MHz = 570kHz
//8MHz / (2*1*(1+7)) = 8 / 16MHz = 500kHz

static inline void initTimer0(void){
	/*8bit Timer for RADIO SIGNAL WAVE*/
	TCCR0A |=(1<<WGM01);	/*CTC mode */
	TCCR0A |=(1<<COM0B0);	/*Toggles pin(PD5,OC0B) each time through */
	TCCR0B |=(1<<CS00);		/*Clock at CPU frequency, ~8MHz */
	OCR0A = COUNTER_VALUE;	/*carrier frequency */
	/* result is 1000kHz radio frequency */
}

static inline void initTimer1(void){
	TCCR1A |=(1<<WGM12);	/* CTC mode */
	TCCR1B |=(1<<CS11);		/* Clock at CPU/8 frequency, ~ 1MHz */
	TIMSK1 |=(1<<OCIE1A);	/* Enable output compare interrupt */
}

ISR(TIMER1_COMPA_vect){
	//ISR for audio-rate Timer 1 */
	ANTENNA_DDR ^= (1<<ANTENNA); /* Toggle carrier on and off */
}

static inline void transmitBeep(uint16_t pitch, uint16_t duration){
	OCR1A = pitch;	/* set pitch for timer1 */
	sei();			/*turn on interrupts */
	do{
		_delay_ms(1); /* delay for pitch cycles */
		duration--;
	} while(duration > 0);
	cli(); /* Disable ISR so that it stops toggling */
	
	ANTENNA_DDR |=(1<<ANTENNA);  /* back on full carrier (high, 1) */
}


int mega168p_amRadio(void)
{
    //--- Initialize --//
	clock_prescale_set(clock_div_1);
	power_adc_disable();

	initTimer0();
	initTimer1();	
	
    while (1) 
    {
		transmitBeep(E3, 200);
		_delay_ms(100);
		transmitBeep(E3, 200);
		_delay_ms(200);
		transmitBeep(E3, 200);
		_delay_ms(200);
		transmitBeep(C3, 200);
		transmitBeep(E3, 200);
		_delay_ms(200);
		transmitBeep(G3, 400);
		_delay_ms(500);
		transmitBeep(G2, 400);
		
		_delay_ms(2500);
    }
	return (0);
}

