/*
 * i2cThermoMeter_atmega168p.c
 *
 * Created: 2017/01/15 1:09:08
 * Author : Someones Garden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "../lib/pinDefines.h"
#include "../lib/USART.h"
#include "../lib/i2c.h"
#include "../lib/macros.h"


//---- Defines ----//
// LM75
#define LM75_ADDRESS_W			0b10010000
#define LM75_ADDRESS_R			0b10010001
#define LM75_TEMP_REGISTER		0b00000000
#define LM75_CONFIG_REGISTER	0b00000001
#define LM75_THYST_REGISTER		0b00000010
#define LM75_TOS_REGISTER		0b00000011

// ADT7410
#define ADT7410_ADDRESS_W		0b10010000
#define ADT7410_ADDRESS_R		0b10010001
#define ADT7410_TEMP_REGISTER	0b00000000
#define ADT7410_CONFIG_REGISTER	0b00000011
#define ADT7410_THYST_REGISTER	0b00001010

// LCD - ST7032i
#define LCD_ADDRESS_R			0b011111100
#define LCD_ADDRESS_W			0b01111101

		
/* I2C_LCD : ST7032i(I2C character LCD) */

int main(void)
{
    uint8_t tempHighByte, tempLowByte;
	volatile char LCD_char;
	
	//--- Initialize ---//
	clock_prescale_set(clock_div_1); /* 8MHz */
	initUSART();
	printString("\r\n==== i2c Thermometer ====\r\n");
	initI2C();
	
    while (1) 
    {
		i2cStart();
		i2cSend(ADT7410_ADDRESS_W);		/* To set Register, address ADT7410 in write mode */
		/* ACK */
		i2cSend(ADT7410_TEMP_REGISTER);
		/* ACK */
		i2cStart();
		i2cSend(ADT7410_ADDRESS_R);		/* Setup and send address with read bit */
		tempHighByte = i2cReadAck();
		tempLowByte  = i2cReadNoAck();
		i2cStop();
		
		I2C_LCD_Init();
		

		printByte(tempHighByte);		//RXTX
		I2C_LCD_SendString(0x00, "ADT7410", 8);		//I2C_LCD - line1
		I2C_LCD_SendByte(0x40, tempHighByte);		//I2C_LCD - line2
		
		if(tempLowByte & BV(7)){
			printString(".5\r\n");	//RXTX
			I2C_LCD_SendString(0x43, ".5C", 3);  //I2C_LCD
		}else{
			printString(".0\r\n");  //RXTX
			I2C_LCD_SendString(0x43, ".0C", 3);  //I2C_LCD
		}		
		_delay_ms(200);
    }
	
	return(0);
}

