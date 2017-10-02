/*
 * SlowScope.c
 *
 * Created: 2016/12/29 10:07:07
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"

#define  SAMPLE_DELAY 20 /* ms to controls the scroll-speed of the scope */

static inline void initFreeRunningADC(void){
	/* setting for FREE MODE */
	ADMUX  |= (1<<REFS0); /* set VOLTAGE REFERENCE : AVCC */
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0); /* set ADC CLOCK PRESCALER : 1/8 */
	ADMUX  |= (1<<ADLAR); /* ADC = ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADC1 ADC0 0 0 0 0 0 0 */
	ADCSRA |= (1<<ADEN); /* ENABLE ADDC */
	ADCSRA |= (1<<ADATE); /* ADC Auto Trigger Enable */
	ADCSRA |=(1<<ADSC); /* ADC Start Conversion (Free mode Start conversion NOW! */
}


int mega168p_slowScope(void)
{
	uint8_t ledValue;
	uint8_t i;
	
	initUSART();
	initFreeRunningADC();
  
	LED_DDR = 0xff;
    
	while (1) 
    {
		printByte(ADCH);
		transmitByte(ADCH);
		
		ledValue = ADCH/16; /* CHECK on LED */
		LED_PORT = 0;
		for(i=0; i<=ledValue;i++){
			LED_PORT |=(1<<i);
		}
		
		_delay_ms(SAMPLE_DELAY);

    }
	
	return(0);
}

