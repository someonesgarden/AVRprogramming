/*
 * pwmTimer_mega169p.c
 *
 * Created: 2017/01/05 11:50:15
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "../lib/pinDefines.h"
#include "../lib/USART.h"

static inline void initTimers(){
	/*Timer 1A, 1B */


	TCCR1A |=(1<<WGM10) | (1<<WGM12);	/*Fast PWM mode, 8bit */
	TCCR1B |=(1<<CS11);					/* PWM Freq = F_CPU /8/256 */
	TCCR1A |= (1<<COM1A1);				/*PWM OUTPUT ON OCR1A(PB1) */
	TCCR1A |= (1<<COM1B1);				/*PWM OUTPUT ON OCR1B(PB2) */
	
	/*Timer 2 */
	TCCR2A |=(1<<WGM20) | (1<<WGM21);	/*Fast PWM mode */
	TCCR2B |=(1<<CS21);					/*PWM Freq = F_CPU /8/256 */
	TCCR2A |=(1<<COM2A1);				/*PWM OUTPUT on OCR2A(PB3) */
}

int mega168p_pwmTimer(void)
{
	uint8_t brightness;
    initTimers();
	initUSART();
	printString("-- LED PWM on Timer Demo --\r\n");
	
	LED_DDR |=(1<<LED1);
	LED_DDR |=(1<<LED2);
	LED_DDR |=(1<<LED3);
	
    while (1) 
    {
		printString("\r\nEnter (0-255)for PWM duty cycle:");
		brightness = getNumber();
		//brightness = 100;
		OCR2A = OCR1B; //PB3 = PB2 
		OCR1B = OCR1A; //PB2 = PB1
		OCR1A = brightness;
    }
	
	return (0);
}

