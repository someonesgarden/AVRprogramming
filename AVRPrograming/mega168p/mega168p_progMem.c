/*
 * ProgMem_mega168p.c
 *
 * Created: 2017/01/16 19:08:52
 * Author : Someones Garden
 */ 


#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../lib/USART.h"

const char myVeryLongString[] PROGMEM = "\r\nHi there, this is an example of a long string.\r\n The kind that you woundnt want to store in RAM.\r\n";
const char myVeryLongString1[] PROGMEM = "\r\nHi there, this is me.\r\n";
const char myVeryLongString2[] PROGMEM = "\r\nAll works and no play.\r\n";
const uint16_t sixteenBits PROGMEM = 12345;
const uint16_t myData[] PROGMEM = {1111,2222,3333,4444,5555,6666,7777,8888,9999,10000};
const uint16_t myData2[] PROGMEM = {123,456,789,012,345,678,999};
	
void printData_Progmem(const uint16_t * dataPointer, uint8_t length){
	while(length){
		printWord((uint16_t)dataPointer);
		printString(":");
		printWord(pgm_read_word(dataPointer));
		
		printString("\r\n");
		dataPointer++;
		length--;
		_delay_ms(100);
	}
}


void printString_Progmem(const char * stringP){
	char oneLetter;
	while((oneLetter = pgm_read_byte(stringP))){
		transmitByte(oneLetter);
		stringP++;
		_delay_ms(100);
	}
}


int mega168p_progMem(void)
{
    initUSART();
	printString("\r\n==ProgMem Test===\r\n");
	
	const char *stringPointer;
	const uint16_t *wordPointer;
	
	char oneLetter;
	uint8_t i;
	
	stringPointer = &myVeryLongString[0];
	//stringPointer = myVeryLongString;
	wordPointer = &sixteenBits;
	
    while (1) 
    {
		printData_Progmem(myData,sizeof(myData)/2);
		printData_Progmem(myData2, sizeof(myData2)/sizeof(myData2[0]));
		printString("\r\n");
		_delay_ms(1000);
		
		printString_Progmem(myVeryLongString1);
		printString_Progmem(&myVeryLongString2[5]);
		printString_Progmem(&myVeryLongString2[0]);
		_delay_ms(1000);
		
		for(i=0;i<sizeof(myVeryLongString); i++){
			//oneLetter = pgm_read_byte(&(myVeryLongString[i]));
			oneLetter = pgm_read_byte(stringPointer+i);
			transmitByte(oneLetter);
			_delay_ms(100);
		}
		_delay_ms(1000);
		
		printWord(&sixteenBits); /* this throws a compiler warning.. */
		transmitByte('\r');
		transmitByte('\n');
		//printWord(pgm_read_word(&sixteenBits));
		printWord(pgm_read_word(wordPointer));
	}
	return (0);
}

