//------------------------------------------------------------
// LCD control(AQM1248A)
// by takuya matsubara
// http://nicotak.com
//
//--------------------------------------------------------------------

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "lcd_aqm1248a.h"
#include "vram.h"

#ifndef F_CPU
	#define F_CPU 8000000	// CPU�N���b�N���g��[Hz]
#endif
#include "timer.h"

#define LCD_DDR   DDRC
#define LCD_PORT  PORTC
#define LCD_SDI   (1<<0)
#define LCD_SCLK  (1<<1)
#define LCD_RS    (1<<2)
#define LCD_CS    (1<<3)

#define CS_ON		LCD_PORT&=~LCD_CS;
#define CS_OFF		LCD_PORT|=LCD_CS;
#define SCLK_LOW	LCD_PORT&=~LCD_SCLK;
#define SCLK_HIGH	LCD_PORT|=LCD_SCLK;
#define RS_COMMAND	LCD_PORT&=~LCD_RS;
#define RS_DATA		LCD_PORT|=LCD_RS;

//------------------------------------------------------------
int lcdptr=0;

//------------------------------------------------------------
// 128 x 48dot
//page0
//page5
//------------------------------------------------------------
void lcd_control(void){
	#define SENDBYTES	4
	char i;
	char pagenum;

	if((lcdptr % 128)==0){
		pagenum=lcdptr/128;	//0-5

		RS_COMMAND;
		lcd_setbyte(0xB0 + pagenum);	//set page
		lcd_setbyte(0x00 + 0);		//Y address
		lcd_setbyte(0x10 + 0);		//Y address
		RS_DATA;

		return;
	}
	i=SENDBYTES;
	while(i--){
		lcd_setbyte( vram[lcdptr++] );
	}
	if(lcdptr >= (128*48/8)){
		lcdptr = 0;
	}
}


//------------------------------------------------------------
void lcd_delay(void)
{
#if F_CPU<=8000000
	int loopcnt=8;
#else
	int loopcnt=20;
#endif
	while(loopcnt--){
		asm("nop");
	}
}

//------------------------------------------------------------
void lcd_setbyte(unsigned char dat)
{
	unsigned char mask=0x80;
	CS_ON;
	lcd_delay();
	SCLK_HIGH;

	while(mask)
	{
		SCLK_LOW;
		if(dat & mask)
			LCD_PORT |= LCD_SDI;
		else
			LCD_PORT &= ~LCD_SDI;

		lcd_delay();
		SCLK_HIGH;	//rasing edge
		lcd_delay();
		mask = mask >> 1;
	}
	SCLK_HIGH;
	CS_OFF;
	lcd_delay();
}


//------------------------------------------------------------
void lcd_init(void)
{
	LCD_DDR |= (LCD_SDI | LCD_RS | LCD_SCLK | LCD_CS);	//output
	CS_OFF;
	SCLK_HIGH;

	timer_waitmsec(100);

	RS_COMMAND;

	lcd_setbyte(0xAE);	//Display=off
	lcd_setbyte(0xA0);	//ADC=normal
	lcd_setbyte(0xC8);	//common output = revers
	lcd_setbyte(0xA3);	//bias = 1/7

	lcd_setbyte(0x2C);	//power control 1(Booster circuit: OFF)
	timer_waitmsec(2);
	lcd_setbyte(0x2E);	//power control 2(Voltage regulator circuit: ON)
	timer_waitmsec(2);
	lcd_setbyte(0x2F);	//power control 3(Voltage follower circuit: ON)

	lcd_setbyte(0x20+3);	//Voltage resistor ratio set
	lcd_setbyte(0x81);	//Electronic volume mode set
	lcd_setbyte(0x1C);	//Electronic volume mode set(0-63)

	lcd_setbyte(0xA4);	//display allpoint = normal
	lcd_setbyte(0x40+0);	//display start line(0-63)
	lcd_setbyte(0xA6);	//display = normal
	lcd_setbyte(0xAF);	//Display=on

//	lcd_setbyte(0xB0+page);	//set page address(0-5)
//	lcd_setbyte(0x10+0);	//set column address(0-127)
//	lcd_setbyte(0x00+0);	//
	RS_DATA;
}


//------------------------------------------------------------

void lcd_sendall(void)
{
	char pagenum;
	for(lcdptr=0; lcdptr<128*48/8; lcdptr++){
		if((lcdptr % 128)==0){
			pagenum = lcdptr / 128;

			RS_COMMAND;
			lcd_setbyte(0xB0 + pagenum);	//set page
			lcd_setbyte(0x00 + 0);		//column address
			lcd_setbyte(0x10 + 0);		//column address
			RS_DATA;
		}
		lcd_setbyte( vram[lcdptr] );
	}
}




