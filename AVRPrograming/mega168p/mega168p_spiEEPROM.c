/*
* spiEEPROM_mega168p.c
* SPI EEPROM 25LC256
* Created: 2017/01/13 9:16:03
* Author : Someones Garden
*/ 

/* SPI EEPROM 25LC256 Demo */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "../lib/pinDefines.h"
#include "../lib/USART.h"
#include "../lib/25LC256.h"

int mega168p_spiEEPROM(void) {
	uint8_t i,j;
	uint8_t address;
	char byte;

	// -------- Inits --------- //
	initSPI();
	initUSART();

	// ------ Event loop ------ //
	while (1) {
		printString("\r\n EEPROM Address =\r\n");
			
		for (i = 0; i < 10; i++) {  /* print out first ten bytes of memory */
			printString("  ");
			printByte(i);
			printString("     ");
			printByte(EEPROM_readByte(i));
			printString("\r\n");
			_delay_ms(10);
		}
		printString("[e] to erase [w] to write \r\n\r\n");
		
		byte =receiveByte();
		//trim(&byte);
		
		if(byte!="\r" || byte !="\n"){
		switch (byte) {                             /* take input */
			case 'e':
			printString("Clearing EEPROM, this could take a few seconds.\r\n");
			EEPROM_clearAll();
			break;
			
			case 'w':
			printString("Which memory slot would you like to write to?\r\n");
			address = getNumber();
			printString("\r\nWhat number would you like to store there?\r\n");
			j = getNumber();
			EEPROM_writeByte(address, j);
			printString("\r\n");
			break;
			
			default:
			printString("What??\r\n");
			break;
		}
			//printString("\r\n");
			_delay_ms(100);
		}
		}                                                  /* End event loop */
		return 0;                            /* This line is never reached */
	}
