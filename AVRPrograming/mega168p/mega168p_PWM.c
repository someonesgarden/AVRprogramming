/*
 * Pwm_mega168p.c
 *
 * Created: 2017/01/05 11:27:24
 * Author : Someone's garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/pinDefines.h"

#define LED_DELAY 20	/* Milli-seconds */

void pwmAllPins(uint8_t brightness){
	uint8_t i;
	LED_PORT = 0xff;	/* TURN ON */
	for(i=0;i<255;i++){
		if(i>=brightness){
			LED_PORT = 0;	/* TURN OFF */
		}
		_delay_us(LED_DELAY);
	}
}

int mega168p_PWM(void)
{
    uint8_t brightness = 0;
	uint8_t direction = 1;
	
	LED_DDR = 0xff; //SET ALL LED OUTPUT
	
    while (1) 
    {
		if(brightness==0){
			direction = 1;
		}
		if(brightness==255){
			direction = -1;
		}
		brightness += direction;
		pwmAllPins(brightness);
    }
	
	return (0);
}

