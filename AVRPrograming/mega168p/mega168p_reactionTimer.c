/*
 * reactionTimer_mega168.c
 *
 * Created: 2017/01/02 22:11:00
 * Author : someonesgarden
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"

#include "support.h"

static inline void initTimer1(void){
	TCCR1B |=(1<<CS11) |(1<<CS10); /* Normal Mode, just counting */
	/*
	Clock Speed : 1MHz / 64(Each tick is 64 microseconds) = 0.015625 MMZ = 15.625 mHZ = 15.625 ticks in ms.
	*/
}


int mega168p_reactionTimer(void)
{
	char byte;
	uint16_t timerValue;
	//------Initialize-----//
	initUSART();
	initTimer1();

	LED_DDR = 0xff; //All LEDs on B is for output.
	BUTTON_PORT |= (1<<BUTTON); //Enable internal pull-up.

	printString("\r\nReaction Timer : \r\n");
	printString("-----------------------------------\r\n");
	printString("Press any key to start.\r\n");


    while (1)
    {
		byte = receiveByte();
		/*press any key */
		printString("\r\nGet ready...");
		randomDelay();

		printString("\r\nGo!\r\n");
		LED_PORT = 0xff;
		/* Light LEDs */
		TCNT1 = 0;
		/* Reset Counter */

		if(bit_is_clear(BUTTON_PIN, BUTTON)){
			printString("You're only cheating yourself.\r\n");
		}
		else{
			loop_until_bit_is_clear(BUTTON_PIN, BUTTON); /* PORTD, PD2 */
			timerValue = TCNT1 >> 4;
			/* each tick is approx 1/16 milliseconds, so we bit-shift divide. */

			printMilliseconds(timerValue);
			printComments(timerValue);
		}

		LED_PORT = 0x00;
		printString("Press any key to try again.\r\n");
    }

	return(0);
}
