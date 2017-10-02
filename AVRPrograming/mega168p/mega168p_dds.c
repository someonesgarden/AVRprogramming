/*
 * dds_mega168p.c
 *
 * Created: 2017/01/08 20:49:21
 * Author : Someones garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/pinDefines.h"
#include "../lib/macros.h"
#include "../lib/fullSine.h"
#include "../lib/USART.h"


static inline void initTimer0(void){
	TCCR0A |=(1<<COM0A1);				/* PWM output on OCR0A */
	SPEAKER_DDR |= (1<<SPEAKER);		/* Enable output on pin */
	TCCR0A |=(1<<WGM00) | (1<<WGM01);	/* Fast PWM mode, 8 bit */
	TCCR0B |=(1<<CS00);					/* prescaler 1 */
}


int mega168p_dds(void)
{
    uint16_t accumulator;
	uint16_t accumulatorSteps = 880;	/* approx 440Hz */
	uint8_t  waveStep;
	int8_t   pwmValue;
	
	//--- Initialize ---//
	initUSART();
	initTimer0();
	BUTTON_PORT |= (1<<BUTTON);			/* Pullup on button */
	printString("\r\n---- DDS -----");
    while (1) 
    {
		if(bit_is_clear(BUTTON_PIN, BUTTON)){
			
			SPEAKER_DDR |=(1<<SPEAKER);			/* Enable Speaker */
			accumulator += accumulatorSteps;	/* Advance accumulator */
			waveStep = accumulator >> 8;		/* which entry in lookup? */
			pwmValue = fullSine[waveStep];		/* lookup voltage */
			
			//printString("\r\nWaveStep");
			//printBinaryByte(waveStep);
			
			loop_until_bit_is_set(TIFR0, TOV0);	/* wait for PWM cycle */
			OCR0A = 128 + pwmValue;				/* set new PWM value */
			TIFR0 |= (1<<TOV0);					/* disable speaker */
			
		}
		else{
			SPEAKER_DDR &=~(1<<SPEAKER);	
		}
    }
	
	return (0);
}

