/*
 * footStepDetector_mega168p.c
 *
 * Created: 2017/01/08 0:33:24
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"

#define ON_TIME    2000
#define CYCLE_DELAY  10
#define INITIAL_PADDING 16  /* higher is less sensitive */
#define SWITCH    PB7   /* Attach LED */
#define USE_POT   0	/* define to 1 if using potentiometer */

#if USE_POT
#define POT PC5   /* optional padding pot */
#endif

// --- Functions --- //

void initADC(void){
	ADMUX |= (1<<REFS0);   /* Reference voltage to AVCC */
	ADCSRA |=(1<<ADPS2) | (1<<ADPS1);  /* ADC clock prescale /64 */
	ADCSRA |= (1<<ADEN);   /* Enable ADC */
}

uint16_t readADC(uint8_t channel){
	ADMUX = (0b11110000 & ADMUX) | channel;
	ADCSRA |= (1<<ADSC);	/* ADC Start conversion */
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return(ADC);
}

int mega168p_footStepDetector(void)
{
    // ----- Initialize -------- //
	uint16_t lightsOutTimer = 0;   /* Timer for the switch */
	uint16_t adcValue;
	uint16_t middleValue=511;
	uint16_t highValue = 520;
	uint16_t lowValue  = 500;
	uint16_t noiseVolume = 0;
	uint8_t  padding = INITIAL_PADDING;
	
	LED_DDR = ((1<<LED0) | (1<<LED1) | (1<<SWITCH));
	initADC();
	initUSART();
	
    while (1) 
    {
		adcValue = readADC(PIEZO); 
		/* Moving Average */
		middleValue = adcValue + middleValue - ((middleValue - 8) >>4);
		
		/* Moving Averages for positive and negative parts of signal */
		if(adcValue > (middleValue >> 4)){
			highValue = adcValue + highValue - ((highValue-8) >>4);
		}
		
		if(adcValue < (middleValue >> 4)){
			lowValue = adcValue + lowValue - ((lowValue - 8) >>4);
		}
		
		noiseVolume = highValue - lowValue + padding; /* "Padding" provides a minimum value for noise volume */
		
		/*Now check to see if ADC value above or below thresholds */
		/* Comparison with >> 4 b/c EWMA is on different scale */
		if(adcValue <((middleValue - noiseVolume) >>4)){
			LED_PORT = (1 < LED0) | ( 1<< SWITCH);
			lightsOutTimer = ON_TIME / CYCLE_DELAY;		/* reset timer */
		}else if(adcValue > ((middleValue + noiseVolume) >> 4)){
			
			LED_PORT = (1<<LED1) | ( 1<<SWITCH);
			lightsOutTimer = ON_TIME / CYCLE_DELAY; /*reset timer */
		}
		else{
			/*Nothing seen, turn off ligths */
			LED_PORT &= ~(1<<LED0);
			LED_PORT &= ~(1<<LED1); /*Both off */
			if(lightsOutTimer > 0){
				lightsOutTimer--;
			}else{
				LED_PORT &=~(1<<SWITCH); /*turn switch off */
			}
		}
		
		#if USE_POT
		/* optional padding potentiometer */
		padding = readADC(POT) >> 4; /*Scale down to useful range */
		#endif
		
		/*Serial output and delay */
		/* ADC is 10-bits, recenter around 127 for display purposes */
		transmitByte(adcValue - 512 + 127);
		transmitByte((lowValue>>4) -512 + 127);
		transmitByte((highValue>>4) - 512 + 127);
		_delay_ms(CYCLE_DELAY);
	}
	
	return (0);
}

