/*
I2C Master Receiver  2013.6.30
クロック：内蔵８ＭＨｚ
デバイス：ATmega328P
*/

#include "../lib/i2c_master.h"
#include "../lib/_servoSerialHelpers.h"
#include "../lib/_servoClockFunctions.h"

volatile uint8_t rdata = 0x00;
char charAry[16]="0000000000000000";
// -------- Global Variables --------- //
volatile uint16_t ticks;
volatile uint8_t hours = 15;                 /* arbitrary default time */
volatile uint8_t minutes = 42;
volatile uint8_t seconds = 57;


int mega168p_i2c_masterReceiver(){
	//usart_buff_init();	// RTC受信バッファ初期化
	initI2C();
	initUSART();
	I2C_LCD_Init();
	I2C_LCD_SendString(0x00, "I2CMastr", 8);//I2C_LCD - line1
	I2C_LCD_SendString(0x40, "Ready OK", 8);//I2C_LCD - line2
	
	printString("\r\n\r\n\r\n\r\n\r\n\r\n");
	printString("\r\n<<<<< MASTER >>>>>\r\n");
	_delay_ms(1000); 	//スレーブ起動待ち
	//========================================	
	
	//RTC
	uint8_t data[16];	// 受信データ格納用
	Rtc_init( data );	// RTCモジュール初期化
	sei();	// (RTC)割り込み許可
	printString("\r\nWelcome to the Servo Sundial.\r\n");
	printString("Type S to set time.\r\n");
	
	while(1){	
		//RTC
		pollSerial();
		_delay_ms(4000);
		Rtc_allread( data );	// (RTC)全データ読み込み
		print_time( data );

		/////LCD
		//printString("\r\n\r\n======== LCD  =========\r\n\r\n");
		I2C_LCD_SendString(0x00, "MASTER STANDBY: ", 16);
		I2C_LCD_SendString(0x40, ".               ", 16);
		_delay_ms(500);
		I2C_LCD_SendString(0x40, "...             ", 16);
		_delay_ms(100);
		I2C_LCD_SendString(0x40, ".....           ", 16);
		_delay_ms(100);
		I2C_LCD_SendString(0x40, ".......         ", 16);
		_delay_ms(100);
		I2C_LCD_SendString(0x40, ".........       ", 16);
		_delay_ms(100);
		I2C_LCD_SendString(0x00, "OK:BEGIN TRANSMT", 16);
		I2C_LCD_SendString(0x40, "...........     ", 16);
		_delay_ms(100);
		I2C_LCD_SendString(0x40, ".............   ", 16);
		_delay_ms(100);
		I2C_LCD_SendString(0x40, "................", 16);
		_delay_ms(100);
				
		///Repeated-Start
		i2cStart();
		if( (TWSR & 0xF8 ) != START )  i2cError();
		//printString("\r\n\r\n======== MASTER TRANSMIT  ========\r\n\r\n");
		i2cSend(SLA_W);		//アドレスパケット送信
		master_transmit("Text from Master to Slave..");
		i2cStart(); //REPEATED START!!			
		//printString("\r\n\r\n======== MASTER RECEIVE  =========\r\n\r\n");
		i2cSend(SLA_R);			//アドレスパケット送信
		twi_1byte_all();
		i2cStop();
		
		/////LCD
		//printString("\r\n\r\n======== LCD  =========\r\n\r\n");
		I2C_LCD_SendString(0x00, "RCVD TXTfrmSLA::", 16);
		I2C_LCD_SendString(0x40, charAry, 16);
		_delay_ms(2000);
		
		//printString("\r\n\r\n++++++++++ I2C_STOP ++++++++++++++++\r\n\r\n");
	}
	printString("\r\nMASTER TERMINATE\r\n");	
}


void master_transmit(const char myString[]){
	
		uint8_t i = 0;
		
		while (myString[i]) {
			//transmitByte(myString[i]);
			//printString(".");
			if(i<=16){
				charAry[i]=myString[i];
			}
			i2cSendAck(myString[i]);
			i++;
			_delay_us(1);
		}
		i2cSendNoAck(ZERO);
	
	//i2cStop(); Repeated Start Condition なのでストップしない!!!!
}

void twi_1byte_all(void){
	
	while(1){				
		
		//$40(ACK OK)---アドレスパケット送信完了
		if( (TWSR & 0xF8 ) == MR_SLA_ACK){
			rdata = i2cReadAck();
			//transmitByte((char)rdata);
		}	
		else if((TWSR & 0xF8) == MR_DATA_ACK){
			rdata = i2cReadAck();
			if(rdata == '\0' || rdata == 0x00 || rdata == 0xff) break;
			//transmitByte((char)rdata);
		}
		else if( (TWSR & 0xF8 ) == MR_SLA_NOACK){  //SLAVEからの信号が受信できない場合
			//printString("\r\n[No Signal]\r\n");
			break;
		}
		else if((TWSR & 0xF8) == MR_DATA_NOACK){
			//printString("\r\nData NoACK\r\n");	
		}
		else{
			//printString("\r\nELSE:");
			//printString("[TWSR=0x");
			//printHexByte((TWSR & 0xF8));
			//printString("]\r\n");
			break;
		}
		
		_delay_ms(80);
	}//while	
	//i2cStop(); Repeated Start Conditionなのでストップしない!!!!
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
		// 読み出しアドレス設定
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
	Rtc_allread( data );	// 全データ読み込み

	// 初期データ設定
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

	Rtc_allwrite( data );	// 全データ書き込み
}

uint8_t Rtc_settime( char* time )
{
	uint8_t		i		= 0;	// ループ変数
	uint8_t		data[7];		// 設定データ
	uint16_t	year	= 0;	// 年(数字格納用)
	uint16_t	month	= 0;	// 月(数字格納用)
	uint16_t	day		= 0;	// 日(数字格納用)
	uint8_t		uitmp	= 0;	// 数字格納用

	// 入力チェック
	for( i = 0; i < 14; i++ ){
		if( '0' > time[i] || '9' < time[i] ){
			return -1;	// 数字以外のデータが混ざっているので失敗
		}
	}

	// 現在データ取得
	if( 0 > Rtc_getdata( 2, data, 7 )){
		return -1;
	}

	// 年のチェック
	year = ( time[0] - '0' ) * 1000 + ( time[1] - '0' ) * 100 + ( time[2] - '0' ) * 10 + ( time[3] - '0' );
	data[6]	= (( time[2] - '0' ) << 4 ) | ( time[3] - '0' );

	// 月のチェック
	month = ( time[4] - '0' ) * 10 + ( time[5] - '0' );
	if( 12 < month ){
		return -1;
	}
	data[5]	&= ~0x7f;
	data[5]	|= (( time[4] - '0' ) << 4 ) | ( time[5] - '0' );

	// 日のチェック
	day = ( time[6] - '0' ) * 10 + ( time[7] - '0' );
	if( 31 < day ){
		return -1;
	}
	data[3]	= (( time[6] - '0' ) << 4 ) | ( time[7] - '0' );

	// 曜日の計算(ツェラーの公式)
	data[4]	= ( year + ( year >> 2 ) - year / 100 + year / 400 + ( 13 * month + 8 ) / 5 + day ) % 7;

	// 時のチェック
	uitmp = ( time[8] - '0' ) * 10 + ( time[9] - '0' );
	if( 24 < uitmp ){
		return -1;
	}
	data[2]	= (( time[8] - '0' ) << 4 ) | ( time[9] - '0' );

	// 分のチェック
	uitmp = ( time[10] - '0' ) * 10 + ( time[11] - '0' );
	if( 59 < uitmp ){
		return -1;
	}
	data[1]	= (( time[10] - '0' ) << 4 ) | ( time[11] - '0' );

	// 秒のチェック
	uitmp = ( time[12] - '0' ) * 10 + ( time[13] - '0' );
	if( 59 < uitmp ){
		return -1;
	}
	data[0]	&= ~0x7f;
	data[0]	|= (( time[12] - '0' ) << 4 ) | ( time[13] - '0' );

	// 現在時刻設定
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
	uint8_t day = data[5];
	uint8_t week = data[6];
	uint8_t month = data[7];
	uint8_t year = data[8];
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
	
	printString("\r\n====Print Date:====\r\n");
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
