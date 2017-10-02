/*
 * dds_fatSaw_mega168p.c
 *
 * Created: 2017/01/09 0:00:40
 * Author : Someonesgarden
 */ 

#include <avr/io.h>
#include "../lib/fatSaw.h"

int mega168p_dds_fatSaw(void)
{
    uint16_t accumulators[NUMBER_OSCILLATORS];
	uint8_t waveStep;
	int16_t mixer;
	uint8_t i;
	uint16_t randomNumber = 0x1234;
	
	
	//---- Initialize ----//
	initTimer0();
	initUSART();
	
	SPEAKER_DDR |=(1<<SPEAKER);
	LED_DDR |= (1<<LED0);
	
	//Initialize all to same phase 
	for (i=0;i <NUMBER_OSCILLATORS;i++){
		accumulators[i] = 0;
	}
	
	
    while (1) 
    {
		loop_until_bit_is_set(TIFR0, TOV0);
		OCR0A = 128 + mixer;
		TIFR0 |=(1<<TOV0);	//re-set the overflow bit
		mixer = 0;
		for(i = 0; i< NUMBER_OSCILLATORS; i++){
			accumulators[i] += BASEPITCH;
			//randomNumber = 2053*randomNumber + 13849;
			//waveStep = (accumulators[i]+randomNumber) >> 8;
			waveStep = (accumulators[i]) >> 8;
			
			
			if(waveStep == 0){
				accumulators[i] += PHASE_RATE * i;
			}
			
			mixer += fullSaw15[waveStep];
		}
		mixer = mixer >> OSCILLATOR_SHIFT;
    }
	
	return (0);
}

