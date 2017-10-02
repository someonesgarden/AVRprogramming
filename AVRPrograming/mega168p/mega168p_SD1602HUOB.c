/*
 * SD1602HUOB_XA_atmega168p.c
 *
 * Created: 2017/01/28 23:13:15
 * Author : someonesgarden
 */ 

#include <avr/io.h>
#include    "../lib/lcd_sc1602bs.h"
//#include "USART.h"


int mega168p_SD1602HUOB(void)
{
	//initUSART();
	//printString("LCD_TEST\r\n");
	
	
	Lcd_init();
	Lcd_setstr( "1234567" );
	Lcd_setpos(1,0);	// 2�s�ڂ�1���(0�x�[�X)
	Lcd_setstr( "ABCDEFGHIJKL+" );

	return 0;
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

