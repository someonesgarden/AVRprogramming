/*
 * voltMeterMega168p.c
 *
 * Created: 2017/01/07 17:13:07
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>		/*for ADC sleep mode */
#include <math.h>			/* for round() and floor() */

#include "../../../../Downloads/7.0/voltMeterMega168p/voltMeterMega168p/pinDefines.h"
#include "../../../../Downloads/7.0/voltMeterMega168p/voltMeterMega168p/USART.h"

/* Note: This voltmeter is only as accurate as your reference voltage. */
#define REF_VCC 5.053
/* Measured division by voltage divider */
#define VOLTAGE_DIV_FACTOR 3.114


// ------ Functions ------- //
void initADC(void){
	/* set mux to ADC5 */
	ADMUX |=(1<<MUX2) |(1<<MUX0);
	//ADMUX |=(0b00001111 & PC5);
	//ADMUX = (0b11110000 & ADMUX) | PC5; // 5 = 0101
	ADMUX |=(1<<REFS0);						/* reference voltage on AVCC */
	ADCSRA |= (1<<ADPS1) | (1<<ADPS2);		/* ADC clock presaler /64 */
	ADCSRA |= (1<<ADEN);					/* enable ADC */
}

void setupADCSleepmode(void){
	set_sleep_mode(SLEEP_MODE_ADC);			/* defined in avr/sleep.h */
	ADCSRA |= (1<<ADIE);					/* Enable ADC interrupt */
	sei();									/* Enable Global interrupt */
}

EMPTY_INTERRUPT(ADC_vect);

uint16_t oversample16x(void){
	uint16_t oversampledValue = 0;
	uint8_t i;
	for(i=0;i<16;i++){
		sleep_mode();						/* chip to sleep, takes ADC sample */
		oversampledValue += ADC;			/* Add them up 16x */
	}
	return (oversampledValue >> 2); // Divide back down by four 
}

void printFloat(float number){
	number = round(number * 100) / 100;		/* round off to 2 decimal places */
	transmitByte('0' + number / 10);		// tens
	transmitByte('0' + number - 10 * floor(number/10));	//ones
	transmitByte('.');
	transmitByte('0' + (number * 10) - floor(number) * 10); //tenth
	transmitByte('0' + (number * 100) - floor(number * 10) * 10); //hundredths
	printString('\r\n');
}


int mega168p_voltMeter
		(void)
{
	float voltage;
	
	// ----- Initialize ---- //
	initUSART();
	printString("\r\nDigital Voltmeter\r\n\r\n");
	initADC();
	setupADCSleepmode();
	
    while (1) 
    {
		voltage = oversample16x() * VOLTAGE_DIV_FACTOR * REF_VCC / (4*1024);
		printFloat(voltage);		/* Alternatively, just print it out: 
										* printWord(voltage * 100);
										* but then you have to remember the decimal place 
									*/
		_delay_ms(500);	
    }
	return (0);
}


