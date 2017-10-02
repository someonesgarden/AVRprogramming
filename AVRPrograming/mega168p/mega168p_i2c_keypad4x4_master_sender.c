/*
 * I2C_keypad4x4.c
 * As "Master:Sender" sending key to other master
 * Created: 2017/01/24 9:36:32
 * Author : Someone's Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#include "../lib/pinDefines.h"
#include "../lib/USART.h"
#include "../lib/i2c.h"
#include "../lib/macros.h"

#define LCDMPU_ADDRESS 0b10000010 //SLA+R

int mega168p_i2c_keypad4x4_master_sender(void)
{
	char LCD_char;
	uint8_t i;
	
	//--- Initialize ---//
	clock_prescale_set(clock_div_1); /* 8MHz */

	//keypad initialization
	int key=0;
	int keypressed=0;
	int first = 0x40;
	
	DDRB  = 0xF0;
	_delay_ms(1);
	PORTB = 0x0F;
	_delay_ms(1);
	
    while(1) 
    {
		i2cStart(); 
		if((TWSR & 0xf8)==0x08){  //Send Enabled
			TWDR = LCDMPU_ADDRESS; //Receiver Address
			TWCR = (1<<TWINT) | (1<<TWEN);
			i2cWaitForComplete();
			
			for(i=0; i<125;i++){
				TWDR  = i; //Set the data in Data Register
				TWCR = (1<<TWINT) | (1<<TWEN); //Send Start
				i2cWaitForComplete();
				_delay_ms(300);
			}
			//TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN); //STOP T2C
			i2cStop();
			_delay_ms(1000);
		}
			
	/*
		if (PINB!=0b11110000)
		{
			_delay_ms(5);
			keypressed = PINB;
			DDRB ^=0b11111111;//making rows as inputs and columns as output
			_delay_ms(1);
			PORTB ^= 0b11111111;//powering columns
			_delay_ms(1);
				
			keypressed |=PINB; //taking row value and OR ing it to column value
				
			if (keypressed==215){
				I2C_LCD_SendString(first+key, "0",1);
				key++;
			}
			if (keypressed==238){
				I2C_LCD_SendString(first+key, "1",1);
				key++;
			}
			if (keypressed==222){
				I2C_LCD_SendString(first+key, "2",1);
				key++;
			}
			if (keypressed==190){
				I2C_LCD_SendString(first+key, "3",1);
				key++;
			}
			if (keypressed==237){
				I2C_LCD_SendString(first+key, "4",1);
				key++;
			}
			if (keypressed==221){
				I2C_LCD_SendString(first+key, "5",1);
				key++;
			}
			if (keypressed==189){
				I2C_LCD_SendString(first+key, "6",1);
				key++;
			}
			if (keypressed==235){
				I2C_LCD_SendString(first+key, "7",1);
				key++;
			}
			if (keypressed==219){
				I2C_LCD_SendString(first+key, "8",1);
				key++;
			}
			if (keypressed==187){
				I2C_LCD_SendString(first+key, "9",1);
				key++;
			}
			if (keypressed==126){
				I2C_LCD_SendString(first+key, "A",1);
				key++;
			}
			if (keypressed==125){
				I2C_LCD_SendString(first+key, "B",1);
				key++;
			}
			if (keypressed==123){
				I2C_LCD_SendString(first+key, "C",1);
				key++;
			}
			if (keypressed==119){
				I2C_LCD_SendString(first+key, "D",1);
				key++;
			}
			if (keypressed==231){
				I2C_LCD_SendString(first+key, "*",1);
				key++;
			}
			if (keypressed==183){
				I2C_LCD_SendString(first+key, "#",1);
				key++;
			}
			else{
				//I2C_LCD_SendByte(0x40, keypressed);
				//key=0;
			}
				
			keypressed=0;
			DDRB ^=0b11111111;
			_delay_ms(1);
			PORTB ^= 0b11111111;
			_delay_ms(220);
		}
		if (key==8)
		{
			key=0;
		}	
		_delay_ms(300);
		*/
    }
	return(0);
}

