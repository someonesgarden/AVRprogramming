/*
 * I2C_to_LCD.c
 *
 * Created: 2017/01/24 9:57:54
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#include "../lib/pinDefines.h"
#include "../lib/i2c.h"
#include "../lib/macros.h"

#define MY_ADDRESS 0b10000010 //SLA+R

//---- Defines ----//
// LCD - ST7032i
//#define LCD_ADDRESS_R			0b011111100
//#define LCD_ADDRESS_W			0b011111101
/* I2C_LCD : ST7032i(I2C character LCD) */

int mega168p_i2c_to_LCD(void)
{
	char		 LCD_char, x;
	uint8_t		 i, byte;
	volatile int LCD_start = 0x40;
	
	//--- Initialize ---//
	clock_prescale_set(clock_div_1);		 // 8MHz
	TWAR = MY_ADDRESS;						 //TWI Address Register
	I2C_LCD_Init();
	I2C_LCD_SendString(0x00, "I2CtoLCD", 8); //I2C_LCD - line1
	_delay_ms(1000); //wait for 
		
    while(1) 
    {
		//Wait for I2C to come
		I2C_LCD_SendString(0x00, "WAIT4I2C", 8); //I2C_LCD - line1
		TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
		loop_until_bit_is_set(TWCR, TWINT); // WAIT FOR TWINT
	
		if((TWSR & 0xf8) == 0x60){  // Status Code = 0x60 // 1 to 1 talk
			while(1){
				I2C_LCD_SendString(0x00, "I2C...OK", 8); //I2C_LCD - line1
				byte = i2cReadAck();
				I2C_LCD_SendByte(LCD_start, byte);
				
				if((TWSR & 0xf8)==0xA0) break; //STOP condition RECEIVED
			}
		}
		_delay_ms(10);
    }
	return(0);
}

