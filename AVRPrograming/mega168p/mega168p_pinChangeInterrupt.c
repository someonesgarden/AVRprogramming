/*
 * pinChangeInterruptMega168.c
 *
 * Created: 2017/01/02 16:50:38
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"

#define SENSE_TIME	50
#define THRESHOLD	12000

//----- Global Variables ---- //
volatile uint16_t chargeCycleCount;

//----- Functions ------//
void initPinChangeInterrupt(void){
	PCICR |= (1 << PCIE1);  /* enable Pin-change interrupts 1 (bank C) */
	PCMSK1|= (1 << PCINT9);   /* enable specific interrupt for pin PC1 */
}

ISR(PCINT1_vect){
	chargeCycleCount++;
	CAP_SENSOR_DDR |=  (1 << CAP_SENSOR); //output mode 
	_delay_us(1); // tiny delays to charge up the capacitor
	CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR); //input mode
	PCIFR |= (1<<PCIF1); // Resetting pin-change interrupt flag: Clear the pin-change interrupt
}


int mega168p_pinChangeInterrupt(void)
{
    //--- initialize ----//
	
	clock_prescale_set(clock_div_1);
	initUSART();
	printString("==[ Cap Sensor ]==\r\n\r\n");
	
	LED_DDR = 0xff;
	MCUCR |=(1 <<PUD); //Disable all Pull Ups
	CAP_SENSOR_PORT |=(1 << CAP_SENSOR); /* we can leave output HIGH (PORTC,  PC1, ADC1) */
	
	initPinChangeInterrupt();
	
    while (1) 
    {
		chargeCycleCount = 0;
		CAP_SENSOR_DDR |= (1 << CAP_SENSOR);
		sei();
		_delay_ms(SENSE_TIME);
		cli();
		if(chargeCycleCount < THRESHOLD){
			LED_PORT = 0xff; //PORTB
		}
		else{
			LED_PORT = 0; //PORTB
		}
		printWord(chargeCycleCount); /* for fine tuning */
		printString("\r\n");
    }
	return (0);
}

