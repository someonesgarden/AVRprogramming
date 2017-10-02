/*
 * IR_Remote_atmega168p.c
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
#include "../lib/remote.h"

//---- Defines ----//
// LCD - ST7032i
//#define LCD_ADDRESS_R			0b011111100
//#define LCD_ADDRESS_W			0b011111101

/* I2C_LCD : ST7032i(I2C character LCD) */

int mega168p_IR_Remote(void)
{
	char LCD_char;
	uint8_t i;
	uint8_t cmd=0;
	
	//--- Initialize ---//
	clock_prescale_set(clock_div_1); /* 8MHz */
	initUSART();
	I2C_LCD_Init();
	I2C_LCD_SendString(0x00, "IRRemote", 8);//I2C_LCD - line1
		
    while(1) 
    {
		RemoteInit(); //IR remote on Time0/Int0
		
		//IR
		cmd=GetRemoteCmd(1);
		if(cmd<28){
			I2C_LCD_SendString(0x40, "IR:",3);//I2C_LCD - line2
			I2C_LCD_SendByte(0x43, cmd);
		}
		_delay_ms(300);
    }
	return(0);
}

