/*
 * JoyStick2D_Mega168.c
 *
 * Created: 2016/12/29 21:41:34
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"

#define  SAMPLE_DELAY 20 /* ms to controls the scroll-speed of the scope */
#define  SENSOR_WEIGHT 1;

static inline void initADC(void){
	/* SINGLE-CONVERSION MODE */
	ADMUX  |= (1<<REFS0); /* set VOLTAGE REFERENCE : AVCC */
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0); /* set ADC CLOCK PRESCALER : 1/8 */
	ADMUX  |= (1<<ADLAR); /* ADC = ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADC1 ADC0 0 0 0 0 0 0 */
	ADCSRA |= (1<<ADEN); /* ENABLE ADDC */
}

static inline void initFreeRunningADC(void){
	/* FREE^RUNNING MODE */
	ADMUX  |= (1<<REFS0); /* set VOLTAGE REFERENCE : AVCC */
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0); /* set ADC CLOCK PRESCALER : 1/8 */
	ADMUX  |= (1<<ADLAR); /* ADC = ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADC1 ADC0 0 0 0 0 0 0 */
	ADCSRA |= (1<<ADEN); /* ENABLE ADDC */
	ADCSRA |= (1<<ADATE); /* ADC Auto Trigger Enable */
	ADCSRA |=(1<<ADSC); /* ADC Start Conversion (Free mode Start conversion NOW! */
}

uint16_t readADC(uint8_t channel){
	ADMUX = (0xf0 & ADMUX) | channel;
	ADCSRA |=(1<<ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return(ADC);
}

uint8_t readADCH(uint8_t channel){
	ADMUX = (0xf0 & ADMUX) | channel;
	ADCSRA |=(1<<ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return(ADCH);
}

int mega168p_joyStick2D(void)
{
	uint8_t i,j;
	uint8_t sensorValue0;
	uint8_t sensorValue3;
	
	//initUSART();
	initADC();
  
	DDRB = 0xff;
	DDRD = 0xff;
    
	while (1) 
    {
		//Sensor on ADC0(PC0)
		sensorValue0 = readADCH(0);
		sensorValue0 = sensorValue0 * SENSOR_WEIGHT;
		PORTB = 0;
		for(i=0; i<=sensorValue0;i++){
			PORTB |=(1<<i);
		}
		
		//Sensor on ADC3(PC3)
		sensorValue3 = readADCH(3);
		sensorValue3 = sensorValue3 * SENSOR_WEIGHT;
		PORTD = 0;
		for(j=0; j<=sensorValue3;j++){
			PORTD |=(1<<j);
		}
		
		_delay_ms(SAMPLE_DELAY);
    }	
	return(0);
}
