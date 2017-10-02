#include	<stdio.h>
#include 	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<util/delay.h>
#include	<avr/interrupt.h>
#include	"pinDefines.h"
#include	"i2c.h"
#include	"USART.h"			//Serial 
#include	"timer.h"
#include	"lcd_aqm1248a.h"	//SPI LCD
#include	"vram.h"

#define 	F_CPU 8000000UL  // 8 MHz


// Global variables that take care of clock
#define START_TIME        10                                  /* 10 am */
#define STOP_TIME     22                                      /* 10 pm */
#define HOURS_RANGE        (STOP_TIME - START_TIME - 1)

extern volatile uint16_t ticks;
extern volatile uint8_t hours;                 /* arbitrary default time */
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;

//LCD
void aqm_String(const char myString[], const uint8_t len);
void aqm_String_seq(const char myString[], const uint8_t len);

//RTC
uint8_t Rtc_setdata( uint8_t start, uint8_t *data, uint8_t num);
uint8_t Rtc_getdata( uint8_t start, uint8_t *data, uint8_t num);
uint8_t Rtc_settime( char* time);
void Rtc_init( uint8_t *data);
void Rtc_allwrite( uint8_t *data);
void Rtc_allread( uint8_t *data);
void print_time( uint8_t *data );