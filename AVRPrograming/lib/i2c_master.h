#include	<stdio.h>
#include 	<avr/io.h>
#include	<util/delay.h>
#include	<avr/interrupt.h>
#define 	F_CPU 8000000UL  // 8 MHz

#include    "i2c.h"
#include	"USART.h"
#define		SLA_W		0b10000000
#define		SLA_R		0b10000001
//#define		RTC_SLA_W	0b10100010
//#define		RTC_SLA_R	0b10100011

//STATUS
//MR : Master Receive
//MT : Master Transmit
#define	START				0x08	//Start
#define	MR_SLA_ACK			0x40	//SLA_ACK OK
#define MT_SLA_ACK			0x18
#define	MR_SLA_NOACK		0x48	//SLA_NOACK OK
#define	MR_DATA_ACK			0x50	//DATA_ACK OK
#define	MR_DATA_NOACK		0x58	//DATA_NOACK OK
#define ZERO				0x00

//RTC
#define		RTC_ADDR_WRITE	0xA2	// RT
#define		RTC_ADDR_READ	0xA3	// RTC
// Global variables that take care of clock
#define START_TIME        10                                  /* 10 am */
#define STOP_TIME     22                                      /* 10 pm */
#define HOURS_RANGE        (STOP_TIME - START_TIME - 1)
extern volatile uint16_t ticks;
extern volatile uint8_t hours;                 /* arbitrary default time */
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;

//------------------------------------------------
void master_transmit(const char myString[]);
void twi_1byte_all(void);

//RTC
uint8_t Rtc_setdata( uint8_t start, uint8_t *data, uint8_t num);
uint8_t Rtc_getdata( uint8_t start, uint8_t *data, uint8_t num);
uint8_t Rtc_settime( char* time);

void Rtc_init( uint8_t *data);
void Rtc_allwrite( uint8_t *data);
void Rtc_allread( uint8_t *data);
void print_time( uint8_t *data );

