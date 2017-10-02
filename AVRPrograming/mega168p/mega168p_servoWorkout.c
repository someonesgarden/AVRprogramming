/*
 * servoWorkout_mega168p.c
 *
 * Created: 2017/01/06 9:38:31
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/pinDefines.h"
#include "../lib/USART.h"

/* Depends on Servo types.. try as */
#define PULSE_MIN 1000
#define PULSE_MAX 2000
#define PULSE_MID 1500

static inline uint16_t getNumber16(void);

static inline void initTimer1Servo(void){
	
	TCCR1A |= (1<<WGM11);				/* Set up Timer1(16bit timer) to give a pulse every 20ms */
	TCCR1B |= (1<<WGM12) | (1<<WGM13);	/* Use Fast PWM Mode, counter max in ICR1 */
	TCCR1B |= (1<<CS11);				/* 1/8 prescaling */
	ICR1	= 20000;						/* Top Value = 20msec */
	TCCR1A |= (1<<COM1A1);				/* Direct output pin on PB1 / OC1A */
	DDRB   |= (1<< PB1);					/* set pin for output */
}

static inline void showOff(void){
	printString("Center\r\n");
	OCR1A = PULSE_MID;
	_delay_ms(1500);
	
	printString("ClockWise MAX\r\n");
	OCR1A = PULSE_MAX;
	_delay_ms(1500);
	
	printString("CounterClockWise MAX\r\n");
	OCR1A = PULSE_MIN;
	_delay_ms(1500);
	
	printString("Center\r\n");
	OCR1A = PULSE_MID;
	_delay_ms(1500);
}

int mega168p_servoWorkout(void)
{
	clock_prescale_set(clock_div_1);  //CPU Clock FULL SPEED(8MHz)
	
	uint16_t servoPulseLength;
	OCR1A = PULSE_MID;		/*set it to middle position initially */
	initTimer1Servo();
	initUSART();
	printString("Welcome to the Servo DEMO");
	
	showOff();
    while (1) 
    {
		printString("\r\nEnter a four-digit pulse length\r\n");
		servoPulseLength = getNumber16();
		
		printString("On my way...\r\n");
		OCR1A = servoPulseLength;
		DDRB |=(1<<PB1); // Re-enable OC1A for OUTPUT
		
		_delay_ms(1000);
		printString("\r\nReleasing...\r\n");
		while(TCNT1<3000){
			;
		}  //delay until pulse part of cycle done 
		DDRB &=~(1<<PB1);
    }
	
	return (0);
}


static inline uint16_t getNumber16(void){
	//GET a PWM value from Serial Port.
	//Reads in Characters, turn them into a number
	char thousands = '0';
	char hundreds = '0';
	char tens = '0';
	char ones = '0';
	char thisChar = '0';
	
	do{
		/* Shift numbers over */
		thousands = hundreds;
		hundreds = tens;
		tens = ones;
		ones = thisChar;
		thisChar = receiveByte();
		transmitByte(thisChar);
	}while(thisChar!='\r');
	transmitByte('\r');
	return(1000*(thousands- '0') + 100*(hundreds - '0') + 10*(tens - '0') + (ones-'0'));
}

