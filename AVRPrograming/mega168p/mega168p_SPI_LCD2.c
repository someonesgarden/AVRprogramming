/*
 * SPI_LCD_mega168p.c
 *
 * Created: 2017/01/30 1:44:43
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../lib/timer.h"
#include "../lib/lcd_aqm1248a.h"
#include "../lib/vram.h"


int mega168p_spi_lcd2(void)
{
	
	int ch;
	
	timer_init();
	lcd_init();
	
	
   	
   	//		DDRB |= (1<<5);
   	//		PORTB ^= (1<<5);
   	
   	while(1)
   	{
	   	vram_cls();
	   	vram_line(0,0 ,127 ,47 ,1);
	   	vram_line(127,0 ,0 ,47 ,1);
	   	lcd_sendall();
	   	timer_waitmsec(3000);
	   	
	   	vram_cls();
	   	vram_locate(0,0);
	   	for(ch=0x20;ch<=0x5F;ch++){
		   	vram_putch((char)ch);
	   	}
	   	lcd_sendall();
	   	timer_waitmsec(3000);
   	}
}

