/*
 * timerAudio_mega168.c
 *
 * Created: 2017/01/03 22:53:48
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/pinDefines.h"
#include "../lib/scale8.h"  /* 8-bit scale */

static inline void initTimer(void){
	TCCR0A |=(1<<WGM01); /* CTC mode */
	TCCR0A |=(1<<COM0A0); /* Toggles pin(OC0A,PD6) each cycle through */
	TCCR0B |=(1<<CS00) | (1<<CS01); /* CPU Clock / 64  */
}

static inline void playNote(uint8_t wavelength, uint16_t duration){
	OCR0A = wavelength; /* set Pitch */
	
	SPEAKER_DDR |=(1<<SPEAKER);  /* enable output on speaker */
	/* SPEAKER = PD6; OC0A; */
	while(duration){
		_delay_ms(1);
		duration--;
		/* Variable delay */
	}
	SPEAKER_DDR &= ~(1<<SPEAKER); /*turn speaker OFF */
}

int mega168p_timerAudio(void)
{
	// --------------- Initialize ----//
	initTimer();
	
    while (1) 
    {
		/* Play some notes */
		playNote(C2, 200);
		playNote(E2, 200);
		playNote(G2, 200);
		playNote(C3, 400);
		
		_delay_ms(1000);
		_delay_ms(1000);
		_delay_ms(1000);
	}
	//End event loop */
		return (0);
}

