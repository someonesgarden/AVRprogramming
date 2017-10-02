/*
I2C Slave Transmitter  2013.6.30
クロック：内蔵８ＭＨｚ
デバイス：ATmega328P
Serialは８MHZでうごくがsc1602bsを使う場合は、1/8=1MHzにしないと動作しない
INTRCOSC_8MHZ_6CK_14CK_0MS
*/

#include "../lib/i2c_slave.h"

int mega168p_i2c_slave(){
	uint8_t byte;
	initI2C();
	initUSART();
	printString("\r\n<<<<< SLAVE:0b10000000 >>>>>\r\n");
	
	/*Lcd_init();
	Lcd_setstr( "SD1602 work 1MHz" );
	Lcd_setpos(1,0);	// 2行目の1列目(0ベース)
	Lcd_setstr( "================" );*/
	
	//AQM
	timer_init();
	lcd_init();

	while(1){
		//AQM
		TWAR=SLA_GENERAL;
		byte = i2cReadAck();
		
		switch(TWSR & 0xF8){
			//Slave Transmit
			case ST_SLA_ACK:
				printString("\r\n\r\n=========== SLAVE TRANSMIT  ============\r\n\r\n");
				twi_s_send("TEST,from slave to master\r\n");
				
				//char LCDtext[64]="1111111111111111222222222222222233333333333333334444444444444444";
				aqm_String("========SLAVE TRANSMIT MODE : ST_SLA_ACK =======================", 64);			
				/*	Lcd_setstr( "====STATUS======" );
				Lcd_setpos(1,0);
				Lcd_setstr( "---ST_SLA_ACK---" );*/
				break;
			
			//Slave Receive
			case SR_SLA_ACK:
				printString("\r\n\r\n========== SLAVE RECEIVE  ==============\r\n\r\n");
				byte = i2cReadAck();
				transmitByte((char)byte);
				/*Lcd_setstr( "====STATUS======" );
				Lcd_setpos(1,0);
				Lcd_setstr( "---SR_SLA_ACK---" );*/

				//char LCDtext[64]="1111111111111111222222222222222233333333333333334444444444444444";
				aqm_String("222222222:SLAVE TRANSMIT MODE : SR_SLA_ACK =======================",64);
				break;
				
			//Repeated State
			case REP_START:		
				printString("\r\n\r\n----------- [REPEATED START]  ----------\r\n\r\n");
				aqm_String("222222222==:[REPEATED START] : SR_SLA_ACK =======================",64);
				/*Lcd_setstr( "====STATUS======" );
				Lcd_setpos(1,0);
				Lcd_setstr( "---REP_START----" );*/
				break;
				
			case SR_DATA_ACK:
				//byte = i2cReadAck();
				transmitByte((char)byte);
				/*Lcd_setstr( "====STATUS======" );
				Lcd_setpos(1,0);
				Lcd_setstr( "---SR_DATA_ACK---" );*/
				break;
				
			default:
				printString("default\r\n");
				printHexByte((TWSR & 0xF8));
				printString("\r\n");
				/*Lcd_setstr( "====STATUS======" );
				Lcd_setpos(1,0);
				Lcd_setstr( "---OTHER CASE--" );*/
				break;
		}	
		_delay_us(2);
	}
	printString("\r\nSLAVE TERMINATE\r\n");
}


//1byte send to master
void twi_s_send(const char myString[]){
	
	while(1){
		
		if((TWSR & 0xF8 ) == ST_SLA_ACK){
		printString("[TWSR=0x");
		printHexByte((TWSR & 0xF8));
		printString("]\r\n");
		
			//$A8(ST_SLA_ACK)-------------------------------------
			uint8_t i = 0;
			
			while (myString[i]) {
				//transmitByte(myString[i]);
				printString(".");
				i2cSendAck(myString[i]);
				i++;
				_delay_ms(1);
			}
			i2cSendNoAck(ZERO);			
		}
		else if((TWSR & 0xF8 ) == ST_SLA_NOACK ){
			//$C0(ST_SLA_NOACK)-------------------------------------
			printString("\r\nST_SLA_NOACK\r\n");
		}
		else if((TWSR & 0xF8) == ST_ENDP_ACK){
			printString("\r\n++++++++++++++++++++++++++++++++++++++++\r\n\r\n");
			break;
		}
		else{
			printString("\r\nELSE:");
			printHexByte((TWSR & 0xF8));
			break;
		}
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
