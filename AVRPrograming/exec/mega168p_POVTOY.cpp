/*
 * met168POVTOY.cpp
 *
 * Created: 2016/08/02 12:14:42
 * Author : Someonesgarden
 */ 


#include <avr/io.h>
#include <util/delay.h>

#include "mega168p_POVTOY.h"
#define DELAYTIME 2                                              /* ms */

#define LED_PORT                PORTB
#define LED_DDR                 DDRB


uint8_t invaderData1[] = {
  0b01110000,
  0b00011000,
  0b11111101,
  0b10110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b10110110,
  0b11111101,
  0b00011000,
  0b01110000
};

uint8_t invaderData2[] = {
  0b00001110,
  0b00011000,
  0b10111101,
  0b01110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b01110110,
  0b10111101,
  0b00011000,
  0b00001110
};


void pause(void){
	uint8_t i;
	for (i=0; i<5; i++){
		_delay_ms(DELAYTIME);
	}
}

void POVDisplay(uint8_t povData[], uint8_t numberRows) {
  uint8_t i;
  for (i = 0; i < numberRows; ++i) {
    LED_PORT = povData[i];
    _delay_ms(DELAYTIME);
  }
  pause();
}

int mega168p_POVTOY(void) {

  LED_DDR = 0xff;                            /* set all LEDs to output */ 

  while (1) {                                              /* mainloop */

      POVDisplay(invaderData1, sizeof(invaderData1));
      POVDisplay(invaderData2, sizeof(invaderData2));

  }                                                    /* end mainloop */
  return 0;
}
