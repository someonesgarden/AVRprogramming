/*
 * hBridge_mega168p.c
 *
 * Created: 2017/01/12 13:56:51
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib/USART.h"
#include "../lib/pinDefines.h"


static inline void setBridgeState(uint8_t bridgeA, uint8_t bridgeB){
	if(bridgeA){
		PORTD	 |=(1 << PD6);
		LED_PORT |=(1 << LED0);
	}else{
		PORTD	 &= ~(1 << PD6);
		LED_PORT &= ~(1 << LED0);
	}
	
	if(bridgeB){
		PORTD |= (1 << PD5);
		LED_PORT |=(1<<LED1);
	}
	else{
		PORTD &= ~(1<<PD5);
		LED_PORT &= ~(1<<LED1);
	}
}

int mega168p_hBridge(void)
{
	DDRD |= (1<<PD6);
	DDRD |= (1<<PD5);
	LED_DDR |= (1<<LED0);
	LED_DDR |= (1<<LED1);
	
    while (1) 
    {
		setBridgeState(1,0); /* "forward" */
		_delay_ms(2000);
		
		setBridgeState(0,0); /* "Stop" */
		_delay_ms(2000);
		
		setBridgeState(0,1); /* "Reverse" */
		_delay_ms(2000);
		
		setBridgeState(1,1); /* ALSO stop */
		_delay_ms(2000);
		
		setBridgeState(1,0); /* "Backward" */
		_delay_ms(2000);
		
		setBridgeState(0,1);
		_delay_ms(75);
		
		setBridgeState(0,0);//set back
		_delay_ms(2000);
	}
	return (0);
}

