/*
 * atmega168IRtest.c
 *
 * Created: 2016/08/07 3:23:46
 * Author : someonesgarden
 * All IR + AVR is written in : http://extremeelectronics.co.in/code-libraries/using-ir-remote-with-avr-mcus-part-ii/
 */ 

#include <avr/io.h>
#include "../../../Downloads/7.0/atmega168IRtest/atmega168IRtest/remote.h"
#include "../../../Downloads/7.0/atmega168IRtest/atmega168IRtest/lcd.h"


int mega168p_IR
		(void)
{
	uint8_t cmd = 0;
	RemoteInit();
	
	 LCDInit(LS_BLINK);
	 
	 LCDClear();

	 LCDWriteString("IR RemoteDecoder");
	 LCDWriteStringXY(0,1,"Key Code:");

    /* Replace with your application code */
    while (1) 
    {
		cmd=GetRemoteCmd(1);
		LCDWriteIntXY(10,1,cmd,3);
    }
}

