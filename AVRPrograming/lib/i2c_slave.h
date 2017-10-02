
#include 	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<util/delay.h>


#include    "i2c.h"
#include	"USART.h"
//#include	"lcd_sc1602bs.h" //LCD
//AQM1248A
#include	"timer.h"
#include	"lcd_aqm1248a.h"	//SPI LCD
#include	"vram.h"

#define 	F_CPU			8000000UL  // 8 MHz
#define		SLA_EACH		0b10000000
#define		SLA_GENERAL		0b10000001

//STATUS
//ST:Slave Transmit	/ SR:Slave Receive
#define	ST_SLA_ACK		0xA8	//SLA_ACK OK
#define SR_SLA_ACK		0x60
#define	ST_SLA_NOACK	0xC0	//SLA_NOACK OK
#define	ST_DATA_ACK		0xB8	//DATA_ACK OK
#define SR_DATA_ACK		0x80
#define	ST_ENDP_ACK		0xC8    //ACK END
#define REP_START		0xA0
#define ZERO			0x00

//------------------------------------------------
void twi_s_send(const char myString[]);
void aqm_String(const char myString[], const uint8_t len);

