//------------------------------------------------------------------
//------------------------------------------------------------------
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include "../lib/i2c.h"
#include "../lib/USART.h"


#define PITCH 100
#define PRESC 64
#define INTRCOUNT (0x10000-((F_CPU / PRESC)/ PITCH))

#define SW_PORT PORTB
#define SW_DDR  DDRB
#define SW_PIN  PINB
#define SW_START 0

volatile long cnt;


//------------------------------------------------------------------
ISR(TIMER1_OVF_vect)
{
	TCNT1 = INTRCOUNT;
	cnt++;
}

//------------------------------------------------------------------
void timer1_init(void)
{
	TCCR1A= 0;
	TCCR1B= 3;
	TIFR1  |= (1 << TOV1);
	TIMSK1 |= (1 << TOIE1);
}

//------------------------------------------------------------------

void sw_init(void)
{
	SW_DDR  &= ~(1<<SW_START);
	SW_PORT |=  (1<<SW_START);
}

//------------------------------------------------------------------
char sw_get(char bitnum)
{
	if((SW_PIN &(1<<bitnum))==0)
	return(1);
	else
	return(0);
}

//------------------------------------------------------------------
int mega168p_stockWatch(void)
{
	char mode;
	char swoff;
	char digit[16]="0000000000000000";
	initI2C();
	initUSART();
	I2C_LCD_Init();
	I2C_LCD_SendString(0x00, "STOCKWATCH!:v0.0", 16);//I2C_LCD - line1
	I2C_LCD_SendString(0x40, "==PRESS BUTTON==", 16);//I2C_LCD - line2
	printString("Stop Watch");

	timer1_init();
	sw_init();

	mode =0;
	swoff=0;
	while(1)
	{
		if(sw_get(SW_START)==1){
			if(swoff==0){
				mode = mode ^ 1;
				if(mode==0){
					cli();
					}else{
					cnt=0;
					TCNT1 = INTRCOUNT;
					sei();
				}
				swoff=1;
			}
			}else{
			swoff=0;
		}
		
		sprintf(digit, "%03d:%03d:%03d",(int)(cnt / 6000),(int)((cnt / 100)% 60),(int)(cnt % 100));
		I2C_LCD_SendString(0x40, digit, 16);//I2C_LCD - line2
		printByte((int)(cnt / 6000));   //��
		printString(":");    
		printByte((int)((cnt / 100)% 60));  //�b
		printString(":");   
		printByte((int)(cnt % 100));        //1/100�b
		printString("\r\n");   
	}
	return 0;
}
