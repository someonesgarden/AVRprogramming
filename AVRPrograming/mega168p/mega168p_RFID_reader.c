/*
 * RFID_Card_Reader_atmega168p.c
 *
 * Created: 2017/01/15 1:09:08
 * Author : Someones Garden
 */ 
//750047fdb27d

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

int mega168p_RFID_reader(void)
{
	char RFID_char[12]="000000000000";
	char byte;
	unsigned int i=0;
	
	//--- Initialize ---//
	clock_prescale_set(clock_div_1); /* 8MHz */
	initUSART();
	_delay_ms(50);
	I2C_LCD_Init();
	I2C_LCD_SendString(0x00, "RFIDRead", 8);//I2C_LCD - line1
	
    while(1) 
    {
		//RFID
		/*
		while((UCSR0A)&(1<<RXC0)) //UCSR0A, RXC0
		{
			byte = UDR0;
			
			if(byte !='\0'){
				if(i==0){
					_delay_ms(1);
				}
				
				else if(i<9){
					RFID_charL[i-1]=byte;
					}else{
					RFID_charH[i-9]=byte;
				}
				_delay_ms(1);
				i++;
				if(i==13)
				{
					//RFID_char[i]='';
					I2C_LCD_SendString(0x00, RFID_charL, 8); //I2C_LCD - line2
					I2C_LCD_SendString(0x40, RFID_charH, 8); //I2C_LCD - line2
					i=0;
					
					_delay_ms(100);
				}
			}
			
		}
		*/
		
		
		
		//if(byte !='\0'){
		
		for(uint8_t i = 0; i < 12; i++)
			{
				byte = receiveByte();		
				RFID_char[i]=byte;
				_delay_ms(1);
			}
		//}
			_delay_ms(100);
			I2C_LCD_SendString(0x00, RFID_char, 12); //I2C_LCD - line2
			_delay_ms(200);
			
			//I2C_LCD_Clear();
			//i=0;
			
		
    }
	
	return(0);
}

