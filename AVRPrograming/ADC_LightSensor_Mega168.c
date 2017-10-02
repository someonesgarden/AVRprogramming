/*
 * ADC_LightSensor_Mega168.c
 *
 * Created: 2016/12/29 7:11:40
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"


//----- Functions ---- //
static inline void initADC0(void){
	ADMUX  |= (1 << REFS0); /* set VOLTAGE REFERENCE : AVCC */
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);  /* set ADC CLOCK PRESCALER : 1/8 (ADPS = 0b011) */
	ADCSRA |= (1<<ADEN);  /* Enable ADC */ 
}

int ADC_LightSensor_Mega168(void)
{
    //---- Inits ---//
	uint8_t ledValue;
	uint16_t adcValue;
	uint8_t i;
	
	initADC0();
	LED_DDR = 0xff; /* #define LED_DDR DDRB; */
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);   /* Stard ADC Conversion */
		loop_until_bit_is_clear(ADCSRA, ADSC); /* WAIT until Done */
		adcValue = ADC;  /* read ADV in   */
		/*  #define ADC     _SFR_MEM16(0x78) */
		/* adcValue = 0 0 0 0 0 0 ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADC1 ADC0 */
		/* 10 bits = 0 ~ 1023 values !! */
		
		
		ledValue = (adcValue >> 7); 
		/* ledValue(uint8_t) = (X X X X X X X X) 0 0 0 0 0 ADC9 ADC8 ADC7 */
		/* 3 bits = 0 ~ 7 values!! */
		
		LED_PORT = 0; /* #define LED_PORT PORTB */
		for (i=0;i<=ledValue;i++){
			LED_PORT |=(1<<i);
		}
		_delay_ms(50);
    }
	
	return(0);
}

