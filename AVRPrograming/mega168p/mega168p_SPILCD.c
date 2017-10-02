/*
 * SPILCD_RTC_Timer_168p.c
 *
 * Created: 2017/01/30 20:56:14
 * Author : Someone's Garden
 */ 

#include "spilcd_rtc.h"

//RTC
#define     RTC_ADDR_WRITE	0xA2	// RTC�������݃A�h���X
#define     RTC_ADDR_READ	0xA3	// RTC�ǂݏo���A�h���X


int main(void)
{
	//i2c
	initI2C();
	//Serial
	initUSART();
	//AQM
	timer_init();
	lcd_init();
	
	//RTC
	uint8_t data[16];	// ��M�f�[�^�i�[�p
	Rtc_init( data );	// RTC���W���[��������
	sei();	// (RTC)���荞�݋���
	
	printString("\r\nWelcome to the Servo Sundial.\r\n");
	printString("Type S to set time.\r\n");
	
    while (1) 
    {
		//aqm_String("========SLAVE TRANSMIT MODE : ST_SLA_ACK =======================", 64);	
		
		Rtc_allread( data );	// (RTC)�S�f�[�^�ǂݍ���
			vram_cls();
			vram_locate(0,0);
			print_time( data );
			lcd_sendall();
			_delay_ms(1000);
    }
}

void aqm_String(const char myString[], const uint8_t len){
	uint8_t i = 0;
	vram_cls();
	vram_locate(0,0);
	for(i=0;i<len;i++){
		vram_putch(myString[i]);
	}
	lcd_sendall();
}

void aqm_String_seq(const char myString[], const uint8_t len){
	uint8_t i = 0;
	for(i=0;i<len;i++){
		vram_putch(myString[i]);
	}
}



//RTC FUNCS
uint8_t Rtc_setdata( uint8_t start, uint8_t *data, uint8_t num )
{
	uint8_t	i	= 0;
	if( 16 < start + num || NULL == data ) return -1;
	while( 1 ){
		//Setting Writing Address
		i2cStart();
		i2cSendNoAck(RTC_ADDR_WRITE);
		i2cSendNoAck(start);
		//Data writing
		for( i = 0; i < num; i++ ){
			i2cSendNoAck(data[i]);
		}
		i2cStop();
		break;
	}
	return 0;
}

uint8_t Rtc_getdata( uint8_t start, uint8_t *data, uint8_t num )
{
	uint8_t	i	= 0;

	if( 16 < start + num || NULL == data ) return -1;

	while( 1 ){
		// �ǂݏo���A�h���X�ݒ�
		i2cStart();
		i2cSendNoAck(RTC_ADDR_WRITE);
		i2cSendNoAck(start);
		i2cStart();
		i2cSendNoAck(RTC_ADDR_READ);
		
		for( i = 0; i < num - 1; i++ ){
			data[i]=i2cReadAck();
		}
		if( num - 1 != i ){
			continue;
		}
		data[i]=i2cReadNoAck();
		i2cStop();
		break;
	}
	return 0;
}

void Rtc_allread( uint8_t *data )
{
	Rtc_getdata( 0, data, 16 );
}

void Rtc_allwrite( uint8_t *data )
{
	Rtc_setdata( 0, data, 16 );
}

void Rtc_init( uint8_t *data )
{
	Rtc_allread( data );	// �S�f�[�^�ǂݍ���

	// �����f�[�^�ݒ�
	data[0]		= 0;
	data[1]		= 0;
	data[4] = 4;
	data[5] = 12;
	data[2]		= data[2] & ~0x80;
	data[3]		= data[3] & ~0x80;
	data[4]		= data[4] & ~0xC0;
	data[5]		= data[5] & ~0xC0;
	data[6]		= data[6] & ~0xF8;
	data[7]		= data[7] & ~0xE0;
	//	data[8]		= data[8];
	data[9]		= data[9] & 0x80;
	data[10]	= ( data[10] & ~0xC0 ) | 0x80;
	data[11]	= ( data[11] & ~0xC0 ) | 0x80;
	data[12]	= ( data[12] & ~0xF8 ) | 0x80;
	data[13]	= 0;
	data[14]	= 0;
	data[15]	= 0;

	Rtc_allwrite( data );	// �S�f�[�^��������
}

uint8_t Rtc_settime( char* time )
{
	uint8_t		i		= 0;	// ���[�v�ϐ�
	uint8_t		data[7];		// �ݒ�f�[�^
	uint16_t	year	= 0;	// �N(�����i�[�p)
	uint16_t	month	= 0;	// ��(�����i�[�p)
	uint16_t	day		= 0;	// ��(�����i�[�p)
	uint8_t		uitmp	= 0;	// �����i�[�p

	// ���̓`�F�b�N
	for( i = 0; i < 14; i++ ){
		if( '0' > time[i] || '9' < time[i] ){
			return -1;	// �����ȊO�̃f�[�^���������Ă���̂Ŏ��s
		}
	}

	// ���݃f�[�^�擾
	if( 0 > Rtc_getdata( 2, data, 7 )){
		return -1;
	}

	// �N�̃`�F�b�N
	year = ( time[0] - '0' ) * 1000 + ( time[1] - '0' ) * 100 + ( time[2] - '0' ) * 10 + ( time[3] - '0' );
	data[6]	= (( time[2] - '0' ) << 4 ) | ( time[3] - '0' );

	// ���̃`�F�b�N
	month = ( time[4] - '0' ) * 10 + ( time[5] - '0' );
	if( 12 < month ){
		return -1;
	}
	data[5]	&= ~0x7f;
	data[5]	|= (( time[4] - '0' ) << 4 ) | ( time[5] - '0' );

	// ���̃`�F�b�N
	day = ( time[6] - '0' ) * 10 + ( time[7] - '0' );
	if( 31 < day ){
		return -1;
	}
	data[3]	= (( time[6] - '0' ) << 4 ) | ( time[7] - '0' );

	// �j���̌v�Z(�c�F���[�̌���)
	data[4]	= ( year + ( year >> 2 ) - year / 100 + year / 400 + ( 13 * month + 8 ) / 5 + day ) % 7;

	// ���̃`�F�b�N
	uitmp = ( time[8] - '0' ) * 10 + ( time[9] - '0' );
	if( 24 < uitmp ){
		return -1;
	}
	data[2]	= (( time[8] - '0' ) << 4 ) | ( time[9] - '0' );

	// ���̃`�F�b�N
	uitmp = ( time[10] - '0' ) * 10 + ( time[11] - '0' );
	if( 59 < uitmp ){
		return -1;
	}
	data[1]	= (( time[10] - '0' ) << 4 ) | ( time[11] - '0' );

	// �b�̃`�F�b�N
	uitmp = ( time[12] - '0' ) * 10 + ( time[13] - '0' );
	if( 59 < uitmp ){
		return -1;
	}
	data[0]	&= ~0x7f;
	data[0]	|= (( time[12] - '0' ) << 4 ) | ( time[13] - '0' );

	// ���ݎ����ݒ�
	if( 0 > Rtc_setdata( 2, data, 7 )){
		return -1;
	}
	return 0;
}


void print_time( uint8_t *data )
{
	uint8_t sec = data[2];
	uint8_t min = data[3];
	uint8_t hour = data[4];
	uint8_t day = data[5];		char dayChar[3];
	uint8_t week = data[6];		
	uint8_t month = data[7];	char monthChar[3];
	uint8_t year = data[8];		char yearChar[3];
	char date[17];
	char weekstr[7][4] = {
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thr",
		"Fri",
		"Sat",
	};
	sec = (( sec >> 4) & 0x07) * 10 + ( sec & 0x0F );
	min = (( min >> 4 ) & 0x07 ) * 10 + ( min & 0x0F );
	hour = (( hour >> 4 ) & 0x03 ) * 10 + ( hour & 0x0F );
	day = (( day >> 4 ) & 0x03 ) * 10 + ( day & 0x0F );
	week = week & 0x07;
	month = (( month >> 4 ) & 0x01 ) * 10 + ( month & 0x0F );
	year = (( year >> 4 ) & 0x0F ) * 10 + ( year & 0x0F );
	sprintf(yearChar,"%d",year);
	sprintf(monthChar, "%d",month);
	sprintf(dayChar,"%d",day);
	
	aqm_String_seq(yearChar,3);
	aqm_String_seq("/",1);
	aqm_String_seq(monthChar,3);
	aqm_String_seq("/",1);
	aqm_String_seq(dayChar,3);
	aqm_String_seq("(",1);
	aqm_String_seq(weekstr[week],3);
	aqm_String_seq(") ",2);
	
	printByte(year);
	printString("/");
	printByte(month);

	printString("/");
	printByte(day);
	printString("(");
	printString(weekstr[week]);
	printString(")  ");
	printByte(hour);
	printString(":");
	printByte(min);
	printString(":");
	printByte(sec);
	printString("\r\n");	
}
