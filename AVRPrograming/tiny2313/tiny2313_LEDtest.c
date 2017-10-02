/*
 * attiny2343p_1.c
 *
 * Created: 2017/01/18 3:31:46
 * Author : Someones Garden
 */ 

#include <avr/io.h>


int tiny2313_LEDtest(void)
{
    int i;
	unsigned char x;
	
	DDRB  = 0b00001111; //B0~B3 OUTPUT 
	PORTA = 0b00000011; //A0, A1 Pull Up
	
	for(i=0;i<10;i++){
		PORTB = 0b00001111;
		wait(500);
		
	}
	
    while (1) 
    {
		x = PINA;
		x = (~x) & 0b00000011;
		PORTB = x;
    }
}

void wait(int time){
	int i,j;
	for(i=0;i<time; i++){
		for(j=0;j<55;j++){
			;
		}
	} 
	return;
}

