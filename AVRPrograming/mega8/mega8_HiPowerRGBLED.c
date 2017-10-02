/*****************************************************
==============================
Chip type           : ATmega8
Program type        : Application
Clock frequency     : 1,000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
float red, green, blue;		//now colors
unsigned int tred, tgreen, tblue;	//tagert colors
float delta_red, delta_green, delta_blue;
unsigned int time;

void generate_tagert_color(void)
{
	tred = rand();
	tgreen = rand();
	tblue = rand();
	tred = tred & 0xFF; //to max 255
	tgreen = tgreen & 0xFF;
	tblue = tblue & 0xFF;
};

void delay(unsigned int delayt)
{
	while (delayt != 0)
	{
		_delay_ms(1);
		delayt--;
	};
};

void mega8_HiPowerRGBLED(void)
{

	unsigned int temp1, temp2, temp3, temp4;

	// Input/Output Ports initialization
	// Port B initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=In
	// State7=T State6=T State5=T State4=T State3=0 State2=0 State1=0 State0=T
	PORTB=0x00;
	DDRB=0x0E;

	// Port C initialization
	// Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
	// State6=T State5=T State4=T State3=T State2=T State1=T State0=T
	PORTC=0x00;
	DDRC=0x00;

	// Port D initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
	// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
	PORTD=0x00;
	DDRD=0x00;

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: Timer 0 Stopped
	// Mode: Fast PWM top=FFh
	// OC0A output: Disconnected
	// OC0B output: Disconnected
	TCCR0A=0x03;
	TCCR0B=0x00;
	TCNT0=0x00;
	OCR0A=0x00;
	OCR0B=0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 1000,000 kHz
	// Mode: Fast PWM top=00FFh
	// OC1A output: Non-Inv.
	// OC1B output: Non-Inv.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A=0xA1;
	TCCR1B=0x09;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 1000,000 kHz
	// Mode: Fast PWM top=FFh
	// OC2A output: Non-Inverted PWM
	// OC2B output: Disconnected
	ASSR=0x00;
	TCCR2A=0x83;
	TCCR2B=0x01;
	TCNT2=0x00;
	OCR2A=0x00;
	OCR2B=0x00;


	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	MCUCR=0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK1=0x00;

	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	//ACSR=0x80;
	//SFIOR=0x00;

	srand(1);


	while (1)
	{
		do
		{
			generate_tagert_color();
			temp1 = (abs(tred - tgreen));
			temp2 = (abs(tred - tblue));
			temp3 = (abs(tgreen - tblue));
		}
		while (( temp1 < 150 ) && (temp2 < 150) && (temp3 < 150));
		
		if (rand() < 13100)
		{
			do
			{
				temp4 = rand() & 0x000F ;
			}
			while ((temp4 == 0) || (temp4 > 3));
			
			if (temp4 == 1)
			{
				do
				{
					temp4 = rand() & 0x0FFF;
				}
				while ((temp4 == 0) || (temp4 > 3276));
				
				tred = ceil ((float) (temp4 / 32767) * tred);
			};
			
			if (temp4 == 2)
			{
				do
				{
					temp4 = rand() & 0x0FFF;
				}
				while ((temp4 == 0) || (temp4 > 3276));
				
				tgreen = ceil ((float) (temp4 / 32767) * tgreen);
			};
			
			if (temp4 == 3)
			{
				do
				{
					temp4 = rand() & 0x0FFF;
				}
				while ((temp4 == 0) || (temp4 > 3276));
				
				tblue = ceil ((float) (temp4 / 32767) * tblue);
			};
			
		}
		
		
		delta_red = (float) (tred - red) / 255;
		delta_green = (float) (tgreen - green) / 255;
		delta_blue = (float) (tblue - blue) / 255;
		
		
		do
		{
			temp4 = rand() & 0x00FF;
		}
		while ((temp4 < 50) || (temp4 > 100));
		
		time = ceil( ((float) temp4 / 255) * 10);
		
		do
		{
			if (tred != ceil(red))
			red = red + delta_red;
			if (tgreen != ceil(green))
			green = green + delta_green;
			if (tblue != ceil(blue))
			blue = blue + delta_blue;
			
			OCR1AL = ceil(blue);
			OCR1BL = ceil(green);
			OCR2A = ceil(red);
			
			delay(time);
		}
		while ((tred != ceil(red)) || (tgreen != ceil(green)) || (tblue != ceil(blue)));
		
		delay(time * 255);
	};
}


