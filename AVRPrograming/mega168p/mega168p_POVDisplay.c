/*
 * POVDisplayAll.c
 *
 * Created: 2016/12/27 3:54:17
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>

void POVDisplayMode(uint8_t oneByte, uint8_t mode){
	switch(mode){
		case 0:
			PORTB=oneByte;
			break;
		case 1:
			PORTC=oneByte;
			break;
		case 2:
			PORTD=oneByte;
			break;
		default:
		PORTB=oneByte;
	}
	_delay_ms(2);
}

int mega168p_POVDisplay(void)
{
    
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0xff;
	
    while (1) 
    {
		//B
		POVDisplayMode(0b00000001, 0);
		_delay_ms(200);
		POVDisplayMode(0b00000010, 0);
		_delay_ms(200);
		POVDisplayMode(0b00000100, 0);
		_delay_ms(200);
		POVDisplayMode(0b00001000, 0);
		_delay_ms(200);
		POVDisplayMode(0b00010000, 0);
		_delay_ms(200);
		POVDisplayMode(0b00100000, 0);
		_delay_ms(200);
		POVDisplayMode(0b01000000, 0);
		_delay_ms(200);
		POVDisplayMode(0b10000000, 0);
		_delay_ms(1000);
		PORTB = 0;
		
		//C
		POVDisplayMode(0b00000001, 1);
		_delay_ms(200);
		POVDisplayMode(0b00000010, 1);
		_delay_ms(200);
		POVDisplayMode(0b00000100, 1);
		_delay_ms(200);
		POVDisplayMode(0b00001000, 1);
		_delay_ms(200);
		POVDisplayMode(0b00010000, 1);
		_delay_ms(200);
		POVDisplayMode(0b00100000, 1);
		_delay_ms(200);
		POVDisplayMode(0b01000000, 1);
		_delay_ms(200);
		POVDisplayMode(0b10000000, 1);
		_delay_ms(1000);
		PORTC = 0;
		
		//D
		POVDisplayMode(0b00000001, 2);
		_delay_ms(200);
		POVDisplayMode(0b00000010, 2);
		_delay_ms(200);
		POVDisplayMode(0b00000100, 2);
		_delay_ms(200);
		POVDisplayMode(0b00001000, 2);
		_delay_ms(200);
		POVDisplayMode(0b00010000, 2);
		_delay_ms(200);
		POVDisplayMode(0b00100000, 2);
		_delay_ms(200);
		POVDisplayMode(0b01000000, 2);
		_delay_ms(200);
		POVDisplayMode(0b10000000, 2);
		_delay_ms(1000);
		PORTD = 0;
		
    }
}

